
#include <include/symbolic_functions.h>
#include <include/amici.h>
#include <include/amici_model.h>
#include <string.h>
#include <include/tdata.h>
#include <include/udata.h>
#include "model_jakstat_adjoint_o2_w.h"

int dwdx_model_jakstat_adjoint_o2(realtype t, N_Vector x, N_Vector dx, void *user_data) {
int status = 0;
TempData *tdata = (TempData*) user_data;
Model *model = (Model*) tdata->model;
UserData *udata = (UserData*) tdata->udata;
realtype *x_tmp = N_VGetArrayPointer(x);
memset(tdata->dwdx,0,sizeof(realtype)*2);
status = w_model_jakstat_adjoint_o2(t,x,NULL,tdata);
  tdata->dwdx[0] = x_tmp[1]*2.0;
  tdata->dwdx[1] = 2.0;
return(status);

}

