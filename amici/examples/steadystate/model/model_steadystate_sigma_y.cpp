
#include "model_steadystate_w.h"
#include <include/amici.h>
#include <include/amici_model.h>
#include <include/symbolic_functions.h>
#include <include/tdata.h>
#include <include/udata.h>
#include <string.h>

int sigma_y_model_steadystate(realtype t, TempData *tdata) {
    int status = 0;
    Model *model = (Model *)tdata->model;
    UserData *udata = (UserData *)tdata->udata;
    memset(tdata->sigmay, 0, sizeof(realtype) * 3);
    tdata->sigmay[0] = 1.0;
    tdata->sigmay[1] = 1.0;
    tdata->sigmay[2] = 1.0;
    return (status);
}
