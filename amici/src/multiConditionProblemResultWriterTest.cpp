#include "multiConditionProblemResultWriter.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "testingMisc.h"
#include "multiConditionProblem.h"
#include "MultiConditionDataProvider.h"

#include "hdf5Misc.h"

TEST_GROUP(resultWriter)
{
    void setup() {
        int status = initResultHDFFile("testFileDeleteMe.h5", true);
        CHECK_EQUAL(0, status);

    }

    void teardown() {
        closeResultHDFFile();
    }
};

IGNORE_TEST(resultWriter, testResultWriter) {

    // TODO steadystate example
    UserData udata(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,AMI_SCALING_LN, AMI_O2MODE_NONE);
    MultiConditionDataProvider dataprovider("********.h5") ;
    OptimizationProblem *problem = new MultiConditionProblem(&dataprovider);

    double parameters[problem->numOptimizationParameters];
    double objectiveFunctionValue = 3.0;
    int numFunctionCalls = 12;
    double timeElapsed = 123.4;

    logLocalOptimizerObjectiveFunctionEvaluation(problem, parameters, objectiveFunctionValue, NULL, numFunctionCalls, timeElapsed);

    // TODO check

    double objectiveFunctionGradient[problem->numOptimizationParameters];

    logLocalOptimizerObjectiveFunctionEvaluation(problem, parameters, objectiveFunctionValue, objectiveFunctionGradient,
                                                                  numFunctionCalls, timeElapsed);

    // TODO check


    flushResultWriter();

    int numIterations = 13;
    JobIdentifier path = {1};
    logLocalOptimizerIteration(path, numIterations, parameters,
                                    objectiveFunctionValue,
                                    objectiveFunctionGradient,
                                    timeElapsed,
                                    problem->numOptimizationParameters,
                                    1,//int alg_mod,
                                    1,2,//double inf_pr, double inf_du,
                                    1,//double mu,
                                    1,//double d_norm,
                                    1,//double regularization_size,
                                    1,1,//double alpha_du, double alpha_pr,
                                    1);//int ls_trials);

    logSimulation(path, parameters, objectiveFunctionValue, objectiveFunctionGradient,
                       timeElapsed, problem->numOptimizationParameters, 0, NULL, NULL, NULL, 1, 10);

    double totalTime = 1234;
    saveTotalWalltime(totalTime);

    saveLocalOptimizerResults(problem, path, objectiveFunctionValue,
                                   parameters,
                                   totalTime,
                                   10);

    delete problem;
}