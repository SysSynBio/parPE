#ifndef _am_model_neuron_o2_dwdp_h
#define _am_model_neuron_o2_dwdp_h

#include <sundials/sundials_types.h>
#include <sundials/sundials_nvector.h>
#include <sundials/sundials_sparse.h>
#include <sundials/sundials_direct.h>

class UserData;
class ReturnData;
class TempData;
class ExpData;

int dwdp_model_neuron_o2(realtype t, N_Vector x, N_Vector dx, void *user_data);


#endif /* _am_model_neuron_o2_dwdp_h */