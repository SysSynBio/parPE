#ifndef _am_model_steadystate_Jrz_h
#define _am_model_steadystate_Jrz_h

#include <sundials/sundials_direct.h>
#include <sundials/sundials_nvector.h>
#include <sundials/sundials_sparse.h>
#include <sundials/sundials_types.h>

class UserData;
class ReturnData;
class TempData;
class ExpData;

int Jrz_model_steadystate(realtype t, int ie, N_Vector x, void *user_data,
                          TempData *tdata, const ExpData *edata,
                          ReturnData *rdata);

#endif /* _am_model_steadystate_Jrz_h */