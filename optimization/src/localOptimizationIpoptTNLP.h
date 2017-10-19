#ifndef LOCALOPTIMIZATIONIPOPTTNLP_H
#define LOCALOPTIMIZATIONIPOPTTNLP_H

// This should fix `error "don't have header file for stddef"' with some IpOpt
// versions
#define HAVE_CSTDDEF
#include <IpIpoptApplication.hpp>
#undef HAVE_CSTDDEF

#include <math.h>
#include <pthread.h>

namespace parpe {
using namespace Ipopt;

class OptimizationProblem;

class LocalOptimizationIpoptTNLP : public Ipopt::TNLP {
  public:

    LocalOptimizationIpoptTNLP(OptimizationProblem *problem,
                               pthread_mutex_t *ipoptMutex);

    virtual ~LocalOptimizationIpoptTNLP();

    virtual bool get_nlp_info(Index &n, Index &m, Index &nnz_jac_g,
                              Index &nnz_h_lag,
                              IndexStyleEnum &index_style) override;

    virtual bool get_bounds_info(Index n, Number *x_l, Number *x_u, Index m,
                                 Number *g_l, Number *g_u) override;

    virtual bool get_starting_point(Index n, bool init_x, Number *x,
                                    bool init_z, Number *z_L, Number *z_U,
                                    Index m, bool init_lambda,
                                    Number *lambda) override;

    virtual bool eval_f(Index n, const Number *x, bool new_x,
                        Number &obj_value) override;

    virtual bool eval_grad_f(Index n, const Number *x, bool new_x,
                             Number *grad_f) override;

    virtual bool eval_g(Index n, const Number *x, bool new_x, Index m,
                        Number *g) override;

    virtual bool eval_jac_g(Index n, const Number *x, bool new_x, Index m,
                            Index nele_jac, Index *iRow, Index *jCol,
                            Number *values) override;

    virtual bool intermediate_callback(
        AlgorithmMode mode, Index iter, Number obj_value, Number inf_pr,
        Number inf_du, Number mu, Number d_norm, Number regularization_size,
        Number alpha_du, Number alpha_pr, Index ls_trials,
        const IpoptData *ip_data, IpoptCalculatedQuantities *ip_cq) override;

    virtual void finalize_solution(SolverReturn status, Index n,
                                   const Number *x, const Number *z_L,
                                   const Number *z_U, Index m, const Number *g,
                                   const Number *lambda, Number obj_value,
                                   const IpoptData *ip_data,
                                   IpoptCalculatedQuantities *ip_cq) override;

    OptimizationProblem *problem;
    clock_t timeBegin;

    // for caching
    Number *lastGradient = NULL;
    Number lastCost = INFINITY;
    Number *lastCostP = &lastCost;
    int lastErrors = 0;

    pthread_mutex_t *ipoptMutex;
};

void setIpOptOption(const std::pair<const std::string, const std::string> &pair, SmartPtr<OptionsList>* o);
} // namespace parpe

#endif // LOCALOPTIMIZATIONIPOPTTNLP_H
