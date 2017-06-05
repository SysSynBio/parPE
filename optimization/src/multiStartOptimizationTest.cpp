#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "localOptimizationIpopt.h"
#include "testingMisc.h"
#include "quadraticTestProblem.h"
#include "multiStartOptimization.h"
#include <stdio.h>

TEST_GROUP(multiStartOptimization)
{
    void setup() {
    }

    void teardown() {

    }
};


TEST(multiStartOptimization, testMultiStartOptimization) {
    mock().disable();
    int numStarts = 10;

    mock().expectNCalls(numStarts, "logFinish").withIntParameter("exitStatus", 0 );
    mock().ignoreOtherCalls();

    runParallelMultiStartOptimization(quadraticOptimizationProblemGeneratorForMultiStart, numStarts, true, NULL);
    fflush(stdout);
    mock().checkExpectations();
    mock().clear();
}

