#ifndef LOCAL_OPTIMIZATION_CERES_H
#define LOCAL_OPTIMIZATION_CERES_H

#include "optimizer.h"
#include <string>

namespace parpe {

class OptimizationProblem;

class OptimizerCeres : public Optimizer {
  public:
    OptimizerCeres();

    /**
     * @brief Determines the local optimum for the provided
     * optimization problem using the Google Ceres optimizer
     * @param problem the optimization problem
     * @return Returns 0 on success.

     */
    int optimize(OptimizationProblem *problem) override;
};

} // namespace parpe

#endif
