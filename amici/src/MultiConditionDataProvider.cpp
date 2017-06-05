#include "MultiConditionDataProvider.h"
#include "logging.h"
#include "misc.h"
#include <assert.h>
#include "ami_hdf5.h"

UserData getUserData();

void printJobIdentifierifier(JobIdentifier id)
{
    printf("%d.%d.%d.%d\n", id.idxMultiStart, id.idxLocalOptimization, id.idxLocalOptimizationIteration, id.idxConditions);
}

void sprintJobIdentifier(char *buffer, JobIdentifier id)
{
    sprintf(buffer, "%d.%d.%d.%d\n", id.idxMultiStart, id.idxLocalOptimization, id.idxLocalOptimizationIteration, id.idxConditions);
}


/**
 * @brief
 * @param hdf5Filename Filename from where to read data
 */

MultiConditionDataProvider::MultiConditionDataProvider(const char *hdf5Filename) : modelDims(getUserData())
{
    H5_SAVE_ERROR_HANDLER;
    fileId = H5Fopen(hdf5Filename, H5F_ACC_RDONLY, H5P_DEFAULT);

    if(fileId < 0) {
        logmessage(LOGLVL_CRITICAL, "initDataProvider failed to open HDF5 file '%s'.", hdf5Filename);
        printBacktrace(20);
        H5Ewalk2(H5E_DEFAULT, H5E_WALK_DOWNWARD, hdf5ErrorStackWalker_cb, NULL);
    }
    H5_RESTORE_ERROR_HANDLER;

    hdf5MeasurementPath = "/measurements/y";
    hdf5MeasurementSigmaPath = "/measurements/ysigma";
    hdf5ConditionPath = "/fixedParameters/k";
}


/**
 * @brief Get the number of simulations required for objective function evaluation. Currently, this amounts to the number
 * of conditions present in the data.
 * @return
 */
int MultiConditionDataProvider::getNumberOfConditions()
{
    // TODO: add additional layer for selecten of condition indices (for testing and later for minibatch)
    // -> won't need different file for testing/validation splits
    // TODO: cache

    hid_t dset = H5Dopen(fileId, hdf5MeasurementPath.c_str(), H5P_DEFAULT);
    hid_t dspace = H5Dget_space(dset);
    const int ndims = H5Sget_simple_extent_ndims(dspace);
    assert(ndims == 2); // nObservables * nConditions (TODO *nt)
    hsize_t dims[ndims];
    H5Sget_simple_extent_dims(dspace, dims, NULL);

    return dims[1];
}

int MultiConditionDataProvider::getNumConditionSpecificParametersPerSimulation()
{
    return AMI_HDF5_getIntScalarAttribute(fileId, "/parameters", "numConditionSpecificParameters");
}


/**
 * @brief Update the contstants in AMICI UserData object. Reads a slab for the given experimental conditions
 * from fixed parameters matrix.
 * @param conditionIdx Index of the experimental condition for which the parameters should be taken.
 * @param udata The object to be updated.
 * @return
 */
int MultiConditionDataProvider::updateFixedSimulationParameters(int conditionIdx, UserData *udata)
{
    hdf5LockMutex();

    H5_SAVE_ERROR_HANDLER;

    hdf5Read2DDoubleHyperslab(fileId, hdf5ConditionPath.c_str(), udata->nk, 1, 0, conditionIdx, udata->k);

    if(H5Eget_num(H5E_DEFAULT)) {
        logmessage(LOGLVL_CRITICAL, "Problem in readFixedParameters (row %d, nk %d)\n", conditionIdx, udata->nk);
        printBacktrace(20);
        H5Ewalk2(H5E_DEFAULT, H5E_WALK_DOWNWARD, hdf5ErrorStackWalker_cb, NULL);
        abort();
    }

    H5_RESTORE_ERROR_HANDLER;

    hdf5UnlockMutex();

    return H5Eget_num(H5E_DEFAULT);

}


/**
 * @brief Reads data required for simulation of a specific experimental condition. Creates ExpData and updates UserData object.
 * @param dpath
 * @param udata
 * @return
 */

ExpData *MultiConditionDataProvider::getExperimentalDataForExperimentAndUpdateUserData(int conditionIdx, UserData *udata)
{

    updateFixedSimulationParameters(conditionIdx, udata);


    return getExperimentalDataForCondition(conditionIdx);

}

ExpData *MultiConditionDataProvider::getExperimentalDataForCondition(int conditionIdx)
{
    hdf5LockMutex();

    ExpData *edata = new ExpData;
    edata->am_my = new double[modelDims.ny];
    edata->am_ysigma = new double[modelDims.ny];

    // no events
    edata->am_mz = 0; edata->am_zsigma = 0;

    hdf5Read2DDoubleHyperslab(fileId, hdf5MeasurementPath.c_str(),      modelDims.ny, 1, 0, conditionIdx, edata->am_my);
    hdf5Read2DDoubleHyperslab(fileId, hdf5MeasurementSigmaPath.c_str(), modelDims.ny, 1, 0, conditionIdx, edata->am_ysigma);
    hdf5UnlockMutex();

    return edata;

}


void MultiConditionDataProvider::getOptimizationParametersLowerBounds(double *buffer)
{
    // TODO to HDF5
    fillArray(buffer, modelDims.np, -2);
}


void MultiConditionDataProvider::getOptimizationParametersUpperBounds(double *buffer)
{
    // TODO to HDF5
    fillArray(buffer, modelDims.np, 2);

}


int MultiConditionDataProvider::getNumOptimizationParameters()
{
    return getNumCommonParameters() + getNumberOfConditions() * getNumConditionSpecificParametersPerSimulation();

}


int MultiConditionDataProvider::getNumCommonParameters()
{
    return modelDims.np - getNumConditionSpecificParametersPerSimulation();

}

UserData MultiConditionDataProvider::getModelDims()
{
    return modelDims;
}

UserData *MultiConditionDataProvider::getUserDataForCondition(int conditionIdx)
{
    UserData *udata = new UserData(getModelDims());
    // initialize
    return udata;
}

int MultiConditionDataProvider::getIndexOfFirstConditionSpecificOptimizationParameter(int conditionIdx)
{
    return getNumCommonParameters() + conditionIdx * getNumConditionSpecificParametersPerSimulation();
}


MultiConditionDataProvider::~MultiConditionDataProvider()
{
    H5_SAVE_ERROR_HANDLER;
    herr_t status = H5Fclose(fileId);

    if(status< 0) {
        error("closeDataProvider failed to close HDF5 file.");
        printBacktrace(20);
        H5Ewalk2(H5E_DEFAULT, H5E_WALK_DOWNWARD, hdf5ErrorStackWalker_cb, NULL);
    }
    H5_RESTORE_ERROR_HANDLER;
}