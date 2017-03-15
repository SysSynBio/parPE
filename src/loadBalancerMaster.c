#include "loadBalancerMaster.h"
#include "misc.h"
#include "queue.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

typedef struct LoadBalancer_tag {
    int numWorkers;
    Queue *queue;
    int lastJobId;
    // one for each worker, index is off by one from MPI rank
    // because no job is sent to master (rank 0)
    MPI_Request *sendRequests;
    MPI_Request *recvRequests;
    JobData **sentJobsData;
    pthread_mutex_t mutexQueue;
    sem_t semQueue;
    pthread_t queueThread;
} LoadBalancer;

#define QUEUEMASTER_QUEUE_INITIALIZER {0, (void*) 0, 0, NULL, NULL, NULL, PTHREAD_MUTEX_INITIALIZER, {}, 0}

static LoadBalancer loadBalancer = QUEUEMASTER_QUEUE_INITIALIZER;

static void *loadBalancerRun(void *unusedArgument);

static void assertMPIInitialized();

static JobData *getNextJob();

static void sendToWorker(int workerIdx, JobData *queueElement);

static void masterQueueAppendElement(JobData *queueElement);

static void receiveFinished(int workerID, int jobID);

static void freeJobData(void *element);

/**
 * @brief initLoadBalancerMaster Intialize load balancer.
 */
void loadBalancerStartMaster() {
    // There can only be one queue
    if(!loadBalancer.queue) {
        assertMPIInitialized();

        loadBalancer.queue = queueInit();

        int mpiCommSize;
        MPI_Comm_size(MPI_COMM_WORLD, &mpiCommSize);

        loadBalancer.numWorkers = mpiCommSize - 1;
        loadBalancer.sendRequests = malloc(loadBalancer.numWorkers * sizeof(MPI_Request));
        loadBalancer.recvRequests = malloc(loadBalancer.numWorkers * sizeof(MPI_Request));
        loadBalancer.sentJobsData = malloc(loadBalancer.numWorkers * sizeof(JobData *));

        for(int i = 0; i < loadBalancer.numWorkers; ++i) // have to initialize before can wait!
            loadBalancer.recvRequests[i] = MPI_REQUEST_NULL;

        // Create semaphore to limit queue length
        // and avoid huge memory allocation for all send and receive buffers
        unsigned int queueMaxLength = mpiCommSize;
#ifdef SEM_VALUE_MAX
        queueMaxLength = SEM_VALUE_MAX < queueMaxLength ? SEM_VALUE_MAX : queueMaxLength;
#endif
        sem_init(&loadBalancer.semQueue, 0, queueMaxLength);
        pthread_create(&loadBalancer.queueThread, NULL, loadBalancerRun, 0);
    }
}

#ifndef QUEUE_MASTER_TEST
static void assertMPIInitialized() {
    int mpiInitialized = 0;
    MPI_Initialized(&mpiInitialized);
    assert(mpiInitialized);
}
#endif

/**
 * @brief masterQueueRun Thread entry point. This is run from initMasterQueue()
 * @param unusedArgument
 * @return 0, always
 */

static void *loadBalancerRun(void *unusedArgument) {

    // dispatch queued work packages
    while(1) {
        // check if any job finished
        MPI_Status status;
        int finishedWorkerIdx = 0;

        // handle all finished jobs, if any
        while(1) {
            int dummy;
            MPI_Testany(loadBalancer.numWorkers, loadBalancer.recvRequests, &finishedWorkerIdx, &dummy, &status);

            if(finishedWorkerIdx >= 0) {
                // dummy == 1 despite finishedWorkerIdx == MPI_UNDEFINED
                // some job is finished
                receiveFinished(finishedWorkerIdx, status.MPI_TAG);
            } else {
                // there was nothing to be finished
                break;
            }
        }

        // getNextFreeWorker
        int freeWorkerIndex = finishedWorkerIdx;

        if(freeWorkerIndex < 0) { // no job finished recently, checked free slots
            for(int i = 0; i < loadBalancer.numWorkers; ++i) {
                if(loadBalancer.recvRequests[i] == MPI_REQUEST_NULL) {
                    freeWorkerIndex = i;
                    break;
                }
            }
        }

        if(freeWorkerIndex < 0) {
            // all workers are busy, wait for next one to finish
            MPI_Status status;
            MPI_Waitany(loadBalancer.numWorkers, loadBalancer.recvRequests, &freeWorkerIndex, &status);

            assert(freeWorkerIndex != MPI_UNDEFINED);
            receiveFinished(freeWorkerIndex, status.MPI_TAG);
        }

        JobData *currentQueueElement = getNextJob();

        if(currentQueueElement) {
            sendToWorker(freeWorkerIndex, currentQueueElement);
            loadBalancer.sentJobsData[freeWorkerIndex] = currentQueueElement;
        }

        pthread_yield();
    };

    return 0;
}

/**
 * @brief getNextJob Pop oldest element from the queue and return.
 * @return The first queue element.
 */
static JobData *getNextJob() {

    pthread_mutex_lock(&loadBalancer.mutexQueue);

    JobData *nextJob = (JobData *) queuePop(loadBalancer.queue);

    pthread_mutex_unlock(&loadBalancer.mutexQueue);

    return nextJob;
}

/**
 * @brief sendToWorker Send the given work package to the given worker and track requests
 * @param workerIdx
 * @param queueElement
 */
static void sendToWorker(int workerIdx, JobData *data) {
    int tag = data->jobId;
    int workerRank = workerIdx + 1;

#ifdef MASTER_QUEUE_H_SHOW_COMMUNICATION
    printf("\x1b[36mSent job %d to %d.\x1b[0m\n", tag, workerRank);
#endif

    MPI_Isend(data->sendBuffer, data->lenSendBuffer, MPI_BYTE, workerRank, tag, MPI_COMM_WORLD, &loadBalancer.sendRequests[workerIdx]);

    MPI_Irecv(data->recvBuffer, data->lenRecvBuffer, MPI_BYTE, workerRank, tag, MPI_COMM_WORLD, &loadBalancer.recvRequests[workerIdx]);

    sem_post(&loadBalancer.semQueue);
}

/**
 * @brief masterQueueAppendElement Assign job ID and append to queue.
 * @param queueElement
 */

void loadBalancerQueueJob(JobData *data) {
    assert(loadBalancer.queue);

    sem_wait(&loadBalancer.semQueue);

    pthread_mutex_lock(&loadBalancer.mutexQueue);

    if(loadBalancer.lastJobId == INT_MAX) // Unlikely, but prevent overflow
        loadBalancer.lastJobId = 0;

    data->jobId = ++loadBalancer.lastJobId;

    queueAppend(loadBalancer.queue, data);

    pthread_mutex_unlock(&loadBalancer.mutexQueue);

}

void loadBalancerTerminate() {
    pthread_mutex_destroy(&loadBalancer.mutexQueue);
    pthread_cancel(loadBalancer.queueThread);
    sem_destroy(&loadBalancer.semQueue);

    if(loadBalancer.sentJobsData)
        free(loadBalancer.sentJobsData);
    if(loadBalancer.sendRequests)
        free(loadBalancer.sendRequests);
    if(loadBalancer.recvRequests)
        free(loadBalancer.recvRequests);

    queueDestroy(loadBalancer.queue, 0);
    loadBalancer.queue = 0;
}


/**
 * @brief receiveFinished Message received from worker, mark job as done.
 * @param workerID
 * @param jobID
 */
static void receiveFinished(int workerID, int jobID) {

#ifdef MASTER_QUEUE_H_SHOW_COMMUNICATION
    printf("\x1b[32mReceived result for job %d from %d\x1b[0m\n", jobID, workerID + 1);
#endif

    JobData *data = loadBalancer.sentJobsData[workerID];
    ++(*data->jobDone);
    pthread_mutex_lock(data->jobDoneChangedMutex);
    pthread_cond_signal(data->jobDoneChangedCondition);
    pthread_mutex_unlock(data->jobDoneChangedMutex);

    loadBalancer.recvRequests[workerID] = MPI_REQUEST_NULL;
}
