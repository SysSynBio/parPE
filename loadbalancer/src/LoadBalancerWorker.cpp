#include "LoadBalancerWorker.h"

#include <alloca.h>
#include <assert.h>
#include <mpi.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

LoadBalancerWorker::LoadBalancerWorker() {}

void LoadBalancerWorker::run() {
    bool terminate = false;

    while (!terminate) {
        terminate = waitForAndHandleJobs();
    }
}

bool LoadBalancerWorker::waitForAndHandleJobs() {
    int rank, err;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

#if QUEUE_WORKER_H_VERBOSE >= 3
    printf("[%d] Waiting for work.\n", rank);
#endif

    // wait for receiving a single job and check for size
    MPI_Status mpiStatus;
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &mpiStatus);
    int msgSize;
    MPI_Get_count(&mpiStatus, MPI_BYTE, &msgSize);
    char *buffer = new char[msgSize];

    // receive message
    int source = 0;
    err = MPI_Recv(buffer, msgSize, MPI_BYTE, source, MPI_ANY_TAG,
                   MPI_COMM_WORLD, &mpiStatus);

#if QUEUE_WORKER_H_VERBOSE >= 3
    printf("W%d: Received job %d\n", rank, mpiStatus.MPI_TAG);
#endif
    if (err != MPI_SUCCESS)
        abort();

    if (mpiStatus.MPI_TAG == MPI_TAG_EXIT_SIGNAL) {
        delete[] buffer;
        return 1;
    }

    messageHandler(&buffer, &msgSize, mpiStatus.MPI_TAG);

#if QUEUE_WORKER_H_VERBOSE >= 2
    printf("[%d] Job done, sending results, %dB.\n", rank, msgSize);
#endif
    MPI_Send(buffer, msgSize, MPI_BYTE, 0, mpiStatus.MPI_TAG, MPI_COMM_WORLD);
    delete[] buffer;

    return 0;
}