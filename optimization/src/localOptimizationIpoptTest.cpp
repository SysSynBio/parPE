#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "localOptimizationIpopt.h"
#include "testingMisc.h"
#include "quadraticTestProblem.h"

TEST_GROUP(localOptimizationIpopt)
{
    void setup() {

    }

    void teardown() {
        mock().checkExpectations();
        mock().clear();
    }
};


TEST(localOptimizationIpopt, testOptimization) {
    QuadraticTestProblem *problem = new QuadraticTestProblem();

    mock().expectOneCall("logFinish").withIntParameter("exitStatus", 0);
    mock().expectNCalls(11, "testObj");
    mock().expectNCalls(12, "testObjGrad");

    getLocalOptimumIpopt(problem);

    DOUBLES_EQUAL(42.0, problem->optimalCost, 1e-12);
    DOUBLES_EQUAL(-1.0, problem->optimalParameter, 1e-12);

    delete problem;
}

