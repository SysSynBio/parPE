#ifndef LOCAL_OPTIMIZATION_H
#define LOCAL_OPTIMIZATION_H

#include "optimizationProblem.h"
#include "optimizer.h"

namespace parpe {

class OptimizerIpOpt : public Optimizer {
  public:
    OptimizerIpOpt();

    /**
     * @brief getLocalOptimum Get local optimum using Ipopt Optimizer
     * @param problem
     * @return Returns 0 on success.
     */

    std::tuple<int, double, std::vector<double> > optimize(OptimizationProblem *problem) override;
};

} // namespace parpe

#endif
