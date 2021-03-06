#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <parpeoptimization/optimizationOptions.h>

#include "quadraticTestProblem.h"

#include <cmath>
#include <cstdio>

using ::testing::NiceMock;

namespace parpe {

QuadraticTestProblem::QuadraticTestProblem(std::unique_ptr<Logger> logger)
    : OptimizationProblem(
          std::unique_ptr<GradientFunction>(
              new NiceMock<QuadraticGradientFunctionMock>()), std::move(logger)) {

    auto options = getOptimizationOptions();
    options.maxOptimizerIterations = 12;
    options.optimizer = optimizerName::OPTIMIZER_IPOPT;
    setOptimizationOptions(options);
    // will keep ref, but will be passed as unique pointer, so getReporter
    // must only be called once
    reporter = new NiceMock<OptimizationReporterMock>(cost_fun_.get(),
                                            std::make_unique<Logger>());
}

void QuadraticTestProblem::fillParametersMin(gsl::span<double> buffer) const
{
    buffer[0] = -1e5;

}

void QuadraticTestProblem::fillParametersMax(gsl::span<double> buffer) const
{
    buffer[0] = 1e5;
}

std::unique_ptr<OptimizationReporter> QuadraticTestProblem::getReporter() const
{
    getReporterCalled = true;
    return std::unique_ptr<OptimizationReporter>(reporter);
}

std::unique_ptr<OptimizationProblem>
QuadraticOptimizationMultiStartProblem::getLocalProblem(
        int  multiStartIndex) const {
    auto loggerPrefix = std::string("[start ")
            + std::to_string(multiStartIndex) + "]";
    auto p = std::unique_ptr<OptimizationProblem>(
                new QuadraticTestProblem(
                    std::make_unique<Logger>(loggerPrefix)));
    p->setOptimizationOptions(options);
    return p;
}

QuadraticGradientFunctionMock::QuadraticGradientFunctionMock() {
    using ::testing::Invoke;
    using ::testing::_;

    ON_CALL(*this, evaluate_impl(_, _, _, _, _))
        .WillByDefault(Invoke(&fun, &QuadraticGradientFunction::evaluate));
    ON_CALL(*this, numParameters())
        .WillByDefault(Invoke(&fun, &QuadraticGradientFunction::numParameters));
}

FunctionEvaluationStatus QuadraticGradientFunction::evaluate(
        gsl::span<const double> parameters,
        double &fval, gsl::span<double> gradient, Logger */*logger*/,
        double */*cpuTime*/) const
{
    fval = pow(parameters[0] + 1.0, 2) + 42.0;

    if (!gradient.empty()) {
//        mock().actualCall("testObjGrad");
        gradient[0] = 2.0 * parameters[0] + 2.0;
    } else {
//        mock().actualCall("testObj");
    }

    return functionEvaluationSuccess;
}

int QuadraticGradientFunction::numParameters() const
{
//    mock().actualCall("GradientFunction::numParameters");

    return 1;
}

std::vector<std::string> QuadraticGradientFunction::getParameterIds() const
{
    return std::vector<std::string> {"p1"};
}

bool OptimizationReporterTest::starting(gsl::span<const double>  /*parameters*/) const
{
//    mock().actualCall("OptimizationReporterTest::starting");

    return false;
}

bool OptimizationReporterTest::iterationFinished(gsl::span<const double>  /*parameters*/,
                                                 double  /*objectiveFunctionValue*/,
                                                 gsl::span<const double>  /*objectiveFunctionGradient*/) const
{
//    mock().actualCall("OptimizationReporterTest::iterationFinished");

    return false;
}

bool OptimizationReporterTest::beforeCostFunctionCall(gsl::span<const double>  /*parameters*/) const
{
//    mock().actualCall("OptimizationReporterTest::beforeCostFunctionCall");

    return false;
}

bool OptimizationReporterTest::afterCostFunctionCall(gsl::span<const double> parameters,
                                                     double objectiveFunctionValue,
                                                     gsl::span<double const> objectiveFunctionGradient) const
{
//    mock().actualCall("OptimizationReporterTest::afterCostFunctionCall");

    if(printDebug) {
        if (!objectiveFunctionGradient.empty()) {
            printf("g: x: %f f(x): %f f'(x): %f\n", parameters[0],
                    objectiveFunctionValue, objectiveFunctionGradient[0]);
        } else {
            printf("f: x: %f f(x): %f\n", parameters[0], objectiveFunctionValue);
        }
    }


    return false;
}

void OptimizationReporterTest::finished(double  /*optimalCost*/,
                                        gsl::span<const double>  /*parameters*/,
                                        int exitStatus) const
{
//    mock().actualCall("OptimizationReporterTest::finished").withIntParameter("exitStatus", exitStatus);
}

} // namespace parpe
