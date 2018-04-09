#include "testingMisc.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include <misc.h>
#include <multiConditionProblem.h>
#include <hierarchicalOptimization.h>

#include "wrapfunctions.h"
#undef pi

#include <amici/amici.h>

/* Tests using model model_steadystate_scaled */

// clang-format off
TEST_GROUP(steadystateProblemTests){
    /*
    const std::vector<double> t { 1.0e8 };
    const std::vector<double> k { 0.1, 0.4, 0.7, 1.0 };
    const std::vector<double> p { 1.0, 0.5, 0.4, 2.0, 0.1 };
    const std::vector<double> xSteadystateExp {0.456644592142075,
                                            0.437977375496898,
                                            0.033333333333333};
    */
    const int scalingParameterIdx = 6;
    const int scaledObservableIdx = 3;
    const std::vector<double> t { 1.0e8 };
    const std::vector<double> k { };
    const std::vector<double> p { 1.0, 0.5, 0.4, 2.0, 0.1, 1.0, 1.0, 0.0 };
    const std::vector<double> x0 { 0.1, 0.4, 0.7 };
    const std::vector<double> xSteadystateExp {0.456644592142075,
                                               0.437977375496898,
                                               0.033333333333333};
    const std::vector<double> yExp {0.456644592142075,
                                    0.437977375496898,
                                    0.033333333333333,
                                    0.456644592142075,
                                    0.437977375496898};
    void setup(){

    }

    void teardown(){
    }
};
// clang-format on


TEST(steadystateProblemTests, testSteadystate) {
    // verify steadystate

    auto model = getModel();
    model->setFixedParameters(k);
    model->setInitialStates(x0);
    model->setParameters(p);
    model->setTimepoints(t);

    auto solver = model->getSolver();
    auto rdata = amici::runAmiciSimulation(*solver, nullptr, *model);

    // verify steadystate concentrations
    parpe::checkEqualArray(xSteadystateExp.data(),
                           rdata->x.data(),
                           xSteadystateExp.size(), 1e-5, 1e-5);

    // verify likelihood for matching measurement / simulation
    amici::ExpData edata {*model};
    edata.my = yExp;
    edata.sigmay.assign(edata.my.size(), 1.0);
    rdata = amici::runAmiciSimulation(*solver, &edata, *model);
    CHECK_EQUAL(rdata->status, AMICI_SUCCESS);
    DOUBLES_EQUAL(parpe::getLogLikelihoodOffset(edata.my.size()), rdata->llh, 1e-5);
}

TEST(steadystateProblemTests, testSteadystateMultiCond) {
    auto model = getModel();
    auto modelNonOwning = model.get();
    model->setTimepoints(t);
    model->setInitialStates(x0);
    model->setParameters(p);

    parpe::MultiConditionDataProviderDefault dp(std::move(model));

    dp.k.push_back(k);
    dp.edata.push_back(amici::ExpData(*modelNonOwning));
    dp.edata[0].my = yExp;
    dp.edata[0].sigmay.assign(dp.edata[0].my.size(), 1.0);

    //parpe::AmiciSummedGradientFunction<int>(&dp, nullptr);
    parpe::MultiConditionProblem problem(&dp);
    double cost;
    problem.costFun->evaluate(p.data(), cost, nullptr);
    DOUBLES_EQUAL(-parpe::getLogLikelihoodOffset(dp.edata[0].my.size()), cost, 1e-5);
}


TEST(steadystateProblemTests, testSteadystateHierarchical) {
    // introduce scaling parameters
    auto model = getModel();
    //model->setFixedParameters(k);
    model->setInitialStates(x0);
    model->setParameters(p);
    model->setTimepoints(t);
    auto modelNonOwning = model.get();

    const double scalingExp = 2.0; // scaling parameter
    const std::vector<double> pReduced { 1.0, 0.5, 0.4, 2.0, 0.1, 1.0, 0.0 };
    auto yScaledExp = yExp;
    yScaledExp[scaledObservableIdx] *= scalingExp;
    parpe::MultiConditionDataProviderDefault dp(std::move(model));
    dp.k.push_back(k);
    // x0?
    dp.edata.push_back(amici::ExpData(*modelNonOwning));
    dp.edata[0].my = yScaledExp;
    dp.edata[0].sigmay.assign(dp.edata[0].my.size(), 1.0);

    //parpe::MultiConditionProblem problem(&dp);

    auto scalings = std::make_unique<parpe::AnalyticalParameterProviderDefault>();
    scalings->conditionsForParameter.push_back({0});
    scalings->optimizationParameterIndices.push_back(scalingParameterIdx);
    // x[scalingIdx][conditionIdx] -> std::vector of observableIndicies
    scalings->mapping.resize(1);
    scalings->mapping[0][0] = {scaledObservableIdx};

    auto offsets = std::make_unique<parpe::AnalyticalParameterProviderDefault>();

    auto gradFun = std::make_unique<parpe::AmiciSummedGradientFunction<int>>(&dp, nullptr);
    parpe::HierachicalOptimizationWrapper hier(std::move(gradFun),
                                               std::move(scalings),
                                               std::move(offsets),
                                               dp.getNumberOfConditions(),
                                               modelNonOwning->nytrue,
                                               modelNonOwning->nt(),
                                               parpe::ErrorModel::normal);
    double cost;
    hier.evaluate(pReduced.data(), cost, nullptr);
    DOUBLES_EQUAL(-parpe::getLogLikelihoodOffset(dp.edata[0].my.size()), cost, 1e-5);

    const std::vector<double> pFull { 1.0, 0.5, 0.4, 2.0, 0.1, 1.0, 2.0, 0.0 };
    hier.fun->evaluate(pFull.data(), {0}, cost, nullptr);
    DOUBLES_EQUAL(-parpe::getLogLikelihoodOffset(dp.edata[0].my.size()), cost, 1e-5);
}
