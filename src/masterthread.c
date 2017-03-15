#include <alloca.h>
#include <unistd.h>

#include "masterthread.h"
#include "../objectiveFunctionBenchmarkModel/problem.h"
#include "../objectiveFunctionBenchmarkModel/dataprovider.h"
#include "localOptimizationIpopt.h"
#include "../objectiveFunctionBenchmarkModel/objectiveFunction.h"
#include "misc.h"

#include "loadBalancerMaster.h"

typedef struct newLocalOptimizationOption_tag {
    int multiStartIdx;
    int localOptimizationIdx;
    optimizerEnum optimizer;
} newLocalOptimizationOption;

int getLocalOptimum(Datapath dataPath);


void startParameterEstimation(optimizerEnum optimizer) {
    loadBalancerStartMaster();

    // create threads for multistart batches
    int numMultiStartRuns = getNumMultiStartRuns();
    pthread_t *multiStartThreads = alloca(numMultiStartRuns * sizeof(pthread_t));

    pthread_attr_t threadAttr;
    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);

    newLocalOptimizationOption childOptions[numMultiStartRuns]; // need to keep, since passed by ref to new thread
    for(int k = 0; k < numMultiStartRuns; ++k) {
        childOptions[k].multiStartIdx = k;
        childOptions[k].optimizer = optimizer;
        pthread_create(&multiStartThreads[k], &threadAttr, newMultiStartOptimization, (void *)&childOptions[k]);
    }
    pthread_attr_destroy(&threadAttr);

    // wait for finish
    for(int k = 0; k < numMultiStartRuns; ++k) {
        pthread_join(multiStartThreads[k], NULL);
        logmessage(LOGLVL_DEBUG, "Thread k %d finished", k);
    }
    logmessage(LOGLVL_DEBUG, "All k threads finished.");

    loadBalancerTerminate();
}

void *newMultiStartOptimization(void *pOptions) {
    newLocalOptimizationOption options = *(newLocalOptimizationOption *)pOptions;

    logmessage(LOGLVL_DEBUG, "Spawning thread for global optimization #%d", options.multiStartIdx);

    int numLocalOptimizations = getNumLocalOptimizationsForMultiStartRun(options.multiStartIdx);

    pthread_t *localOptimizationThreads = alloca(numLocalOptimizations * sizeof(pthread_t));

    pthread_attr_t threadAttr;
    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);

    newLocalOptimizationOption childOptions[numLocalOptimizations]; // need to keep, since passed by ref to new thread

    int lastStartIdx = -1;

    for(int ms = 0; ms < numLocalOptimizations; ++ms) {
        childOptions[ms].multiStartIdx = options.multiStartIdx;
        childOptions[ms].optimizer = options.optimizer;
        childOptions[ms].localOptimizationIdx = ++lastStartIdx;
        logmessage(LOGLVL_DEBUG, "Spawning thread for local optimization #%d.%d (%d)", options.multiStartIdx, lastStartIdx, ms);
        pthread_create(&localOptimizationThreads[ms], &threadAttr, newLocalOptimization, (void *)&childOptions[ms]);
    }
    pthread_attr_destroy(&threadAttr);

    int numCompleted = 0;

    while(numCompleted < numLocalOptimizations) {
        for(int ms = 0; ms < numLocalOptimizations; ++ms) {
            if(!localOptimizationThreads[ms])
                continue;

            void *threadStatus = 0;
            int joinStatus = pthread_tryjoin_np(localOptimizationThreads[ms], &threadStatus);
            if(joinStatus == 0) { // joined successful
                if(*(int*)threadStatus == 0) {
                    logmessage(LOGLVL_DEBUG, "Thread ms #%d finished successfully", ms);
                    localOptimizationThreads[ms] = 0;
                    ++numCompleted;
                } else {
                    logmessage(LOGLVL_WARNING, "Thread ms #%d finished unsuccessfully... trying new starting point", ms);
                    childOptions[ms].localOptimizationIdx = ++lastStartIdx;
                    logmessage(LOGLVL_DEBUG, "Spawning thread for local optimization #%d.%d (%d)", options.multiStartIdx, lastStartIdx, ms);
                    pthread_create(&localOptimizationThreads[ms], &threadAttr, newLocalOptimization, (void *)&childOptions[ms]);
                }
                free(threadStatus);
            }
        }

        sleep(1);
    }

    logmessage(LOGLVL_DEBUG, "Leaving thread for global optimization #%d", options.multiStartIdx);

    return 0;
}


void *newLocalOptimization(void *pOptions) {
    newLocalOptimizationOption options = *(newLocalOptimizationOption *)pOptions;
    Datapath path = {INT_MIN};

    path.idxMultiStart = options.multiStartIdx;
    path.idxLocalOptimization = options.localOptimizationIdx;

    // TODO pass options object, also add IpOpt options to config file
    logmessage(LOGLVL_DEBUG, "Starting newLocalOptimization #%d.%d", path.idxMultiStart, path.idxLocalOptimization);
    int *status = malloc(sizeof(int));

    switch (options.optimizer) {
    case OPTIMIZER_CERES:
        logmessage(LOGLVL_CRITICAL, "Not yet implemented.");
        abort();
        break;
    default:
        getLocalOptimum(path);
    }

    logmessage(LOGLVL_DEBUG, "Finished newLocalOptimization #%d.%d", path.idxMultiStart, path.idxLocalOptimization);

    return status;
}

void startObjectiveFunctionGradientCheck()
{
    loadBalancerStartMaster();

    int lenTheta = getLenTheta();
    double *theta = malloc(sizeof(double) * lenTheta);

    Datapath path;
    path.idxMultiStart = 0;
    path.idxLocalOptimization = 0;
    path.idxLocalOptimizationIteration = 0;
    path.idxGenotype = 0;
    path.idxExperiment = 0;

    double epsilon = 1e-6;

    int *parameterIndices = malloc(sizeof(int) * lenTheta);
    for(size_t i = 0; i < lenTheta; ++i)
        parameterIndices[i] = i;
    shuffle(parameterIndices, lenTheta);

    objectiveFunctionGradientCheck(theta, lenTheta, path, AMI_SCALING_LOG10, parameterIndices, 50, epsilon);

    free(theta);
    free(parameterIndices);

    loadBalancerTerminate();
}

int getLocalOptimum(Datapath dataPath) {
    OptimizationProblem *problem = getBenchmarkOptimizationProblem(dataPath);

    int status = getLocalOptimumIpopt(problem);

    freeBenchmarkProblem(problem);

    return status;
}
