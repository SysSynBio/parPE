#include "simulationWorkerAmici.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "misc.h"
#include "testingMisc.h"
#include <amici_model.h>

TEST_GROUP(simulationWorkerAmici){void setup(){

}

                                  void teardown(){

                                  }};

TEST(simulationWorkerAmici, testSerializeResultPackageMessage) {
    UserData u(1, 2, 3);
    Model m(1, 2, 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, AMICI_O2MODE_NONE);

    ReturnData r(&u, &m);
    parpe::JobResultAmiciSimulation results(1, &r, 2.1);

    int msgSize = 0;
    char *buffer = serializeToChar<parpe::JobResultAmiciSimulation>(&results, &msgSize);

    parpe::JobResultAmiciSimulation resultsAct = deserializeFromChar<parpe::JobResultAmiciSimulation>(buffer, msgSize);
    CHECK_EQUAL(results.simulationTimeInSec, resultsAct.simulationTimeInSec);

    delete resultsAct.rdata;

    delete[] buffer;
}

TEST(simulationWorkerAmici, testSerializeWorkPackageMessage) {
    // serialize and deserialize workpackage with random content
    int nTheta = randInt(0, 5000);

    // generate data
    parpe::JobAmiciSimulation expWp;
    int lenDataElements = 10;
    expWp.lenData = lenDataElements * sizeof(double);
    expWp.data = alloca(expWp.lenData);
    for (int i = 0; i < lenDataElements; ++i)
        ((double *)expWp.data)[i] = parpe::randDouble(-1e-8, 1e8);
    expWp.sensitivityMethod = AMICI_SENSI_ASA;
    expWp.numSimulationParameters = nTheta;
    expWp.simulationParameters = new double[nTheta];
    for (int i = 0; i < nTheta; ++i) {
        expWp.simulationParameters[i] = parpe::randDouble(-1e-8, 1e8);
    }

    // serialize
    int workPackageLength =
        parpe::JobAmiciSimulation::getLength(nTheta, expWp.lenData);
    char *buffer = new char[workPackageLength];
    expWp.serialize(buffer);

    // deserialize
    parpe::JobAmiciSimulation actWp;
    actWp.data = alloca(expWp.lenData);
    actWp.simulationParameters = new double[nTheta];

    actWp.deserialize(buffer);
    delete[] buffer;

    // check
    CHECK_EQUAL(actWp.sensitivityMethod, expWp.sensitivityMethod);

    for (int i = 0; i < nTheta; ++i) {
        DOUBLES_EQUAL(actWp.simulationParameters[i],
                      expWp.simulationParameters[i], 0);
    }
    for (int i = 0; i < lenDataElements; ++i) {
        DOUBLES_EQUAL(((double *)actWp.data)[i], ((double *)expWp.data)[i], 0);
    }

    delete[] actWp.simulationParameters;
    delete[] expWp.simulationParameters;
}
