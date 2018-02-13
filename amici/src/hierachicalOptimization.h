#ifndef HIERACHICALOPTIMIZATION_H
#define HIERACHICALOPTIMIZATION_H

#include <optimizationProblem.h>
#include <multiConditionProblem.h>
#include <misc.h>
#include <parpeException.h>

#include <memory>
#include <cmath>
#include <numeric>

#include <H5Cpp.h>

namespace parpe {

enum class ParameterTransformation { none, log10 };
enum class ErrorModel { normal }; // TODO logNormal, laplace

class ScalingFactorHdf5Reader;

/**
 * @brief The HierachicalOptimizationWrapper class is a wrapper for hierarchical optimization of
 * scaling parameters.
 *
 * Parameters with the given indices are hidden by the wrapper and computed analytically internally.
 */
class HierachicalOptimizationWrapper : public GradientFunction
{
public:
    HierachicalOptimizationWrapper(std::unique_ptr<AmiciSummedGradientFunction<int>> fun,
                                   std::unique_ptr<ScalingFactorHdf5Reader> reader,
                                   int numConditions,
                                   int numObservables,
                                   int numTimepoints,
                                   ParameterTransformation parameterTransformation,
                                   ErrorModel errorModel);

    /**
     * @brief See base class
     * @param parameters
     * @param fval
     * @param gradient
     * @return
     */
    virtual FunctionEvaluationStatus evaluate(
            const double* const parameters,
            double &fval,
            double* gradient) const override;

    /**
     * @brief Run simulations with scaling parameters set to 1.0 and collect model outputs
     * @param reducedParameters parameter vector for `fun` without scaling parameters
     * @return Vector of double vectors containing AMICI ReturnData::y (nt x ny, column-major)
     */
    std::vector <std::vector<double>> getUnscaledModelOutputs(double const * const reducedParameters) const;


    /**
     * @brief Compute proportionality factors
     * @param modelOutputs Model outputs as provided by getModelOutputs
     * @return the computed scaling factors
     */
    std::vector<double> computeAnalyticalScalings(std::vector <std::vector<double>>& modelOutputs) const;

    void applyOptimalScalings(std::vector<double> const& proportionalityFactors, std::vector<std::vector<double> > &modelOutputs) const;

    void applyOptimalScaling(int scalingIdx, double scaling, std::vector <std::vector<double>>&  modelOutputs) const;


    /**
     * @brief Compute the proportionality factor for the given observable.
     *
     * See Supplement 1.1 of [1].
     *
     * [1] Loos, Krause, Hasenauer. Hierarchical optimization for the efficient parametrization of ODE models.
     * @param
     * @return
     */

    virtual double computeAnalyticalScalings(int scalingIdx, std::vector <std::vector<double>> const& modelOutputsUnscaled,
                                  std::vector <std::vector<double>> const& measurements) const;

    virtual FunctionEvaluationStatus evaluateWithScalings(const double* const reducedParameters, std::vector<double> const &scalings,
            std::vector<std::vector<double> > &modelOutputsUnscaled,
            double &fval,
            double* gradient) const;


    /**
     * @brief Compute loglikelihood for normal distribution based on the model outputs and measurements.
     * @param modelOutputsScaled
     * @return
     */
    double computeNegLogLikelihood(std::vector <std::vector<double>> const& modelOutputsScaled) const;


    /**
     * @brief Assemble full parameter vector of wrapped problem from scaling parameters and numerically optimized parameters
     * @param reducedParameters
     * @param scalingFactors
     * @return Full parameter vector for `fun`
     */
    virtual std::vector<double> getFullParameters(double const * const reducedParameters, std::vector<double> const& scalingFactors) const;

    virtual int numParameters() const override;

    int numScalingFactors() const;

    std::vector<int> const& getProportionalityFactorIndices() const;

    std::unique_ptr<AmiciSummedGradientFunction<int>> fun;

private:
    /** Sorted list of the indices of the scaling parameters
      * (sorting makes it easier to splice scaling and remaining parameters in getFullParameters) */
    std::unique_ptr<ScalingFactorHdf5Reader> reader;
    std::vector<int> proportionalityFactorIndices;
    int numConditions;
    int numObservables;
    int numTimepoints;

    ParameterTransformation parameterTransformation;
    ErrorModel errorModel;
};


/**
 * @brief The ScalingFactorHdf5Reader class reads the dependencies of experimental conditions
 * and observables on scaling factors from an HDF5 file.
 *
 * TODO interface
 */
class ScalingFactorHdf5Reader {
public:
    ScalingFactorHdf5Reader(H5::H5File file, std::string rootPath = "/");

    /**
     * @brief Get vector of condition indices for which the scaling factor with the given index is used.
     * @param scalingIdx
     * @return
     */
    std::vector<int> getConditionsForScalingParameter(int scalingIdx) const;

    /**
     * @brief Get vector of observable indices for the specified condition for which the specified scaling factor is used.
     * @param scalingIdx
     * @return
     */
    std::vector<int> const& getObservablesForScalingParameter(int scalingIdx, int conditionIdx) const;

    /**
     * @brief Vector with indices of the proportionality factors with the overall parameter vector
     * @return
     */
    std::vector<int> getProportionalityFactorIndices();

private:
    void readFromFile();

    H5::H5File file;
    std::string rootPath;
    std::string mapPath;
    std::string scalingParameterIndicesPath;

    // x[scalingIdx][conditionIdx] -> std::vector of observableIndicies
    std::vector<std::map<int, std::vector<int>>> mapping;
    std::vector<int> proportionalityFactorIndices;
};


/**
 * @brief The HierachicalOptimizationProblemWrapper class wraps an OptimizationProblem and hides the analytically optimizated parameters (from starting point, parameter bounds, ...)
 *
 */
class HierachicalOptimizationProblemWrapper : public OptimizationProblem {
public:
    HierachicalOptimizationProblemWrapper() = default;
    HierachicalOptimizationProblemWrapper(std::unique_ptr<OptimizationProblem> problemToWrap, MultiConditionDataProvider const* dataProvider);

    virtual ~HierachicalOptimizationProblemWrapper();

    virtual void fillInitialParameters(double *buffer) const override;

    virtual void fillParametersMin(double *buffer) const override;

    virtual void fillParametersMax(double *buffer) const override;

    void fillFilteredParams(std::vector<double> const& fullParams, double *buffer) const;

    OptimizationOptions const& getOptimizationOptions() const override { return wrappedProblem->getOptimizationOptions(); }
    void setOptimizationOptions(OptimizationOptions const& options) override { wrappedProblem->setOptimizationOptions(options); }

    // TODO: need to ensure that this will work with the reduced number of parameters
    virtual std::unique_ptr<OptimizationReporter> getReporter() const override { return wrappedProblem->getReporter(); }

    std::unique_ptr<OptimizationProblem> wrappedProblem;
};

void fillFilteredParams(std::vector<double> const& fullParams, const std::vector<int> &sortedFilterIndices, double *buffer);

} //namespace parpe

#endif // HIERACHICALOPTIMIZATION_H
