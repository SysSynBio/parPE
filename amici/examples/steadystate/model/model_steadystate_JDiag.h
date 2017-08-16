#ifndef _am_model_steadystate_JDiag_h
#define _am_model_steadystate_JDiag_h

#include <sundials/sundials_direct.h>
#include <sundials/sundials_nvector.h>
#include <sundials/sundials_sparse.h>
#include <sundials/sundials_types.h>

class UserData;
class ReturnData;
class TempData;
class ExpData;

int JDiag_model_steadystate(realtype t, N_Vector JDiag, N_Vector x,
                            void *user_data);

#endif /* _am_model_steadystate_JDiag_h */
