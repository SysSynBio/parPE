#include "optimizationResultWriter.h"
#include <logging.h>
#include <assert.h>
#include "hdf5Misc.h"
#include <sys/stat.h>
#include "misc.h"

OptimizationResultWriter::OptimizationResultWriter()
{

}

OptimizationResultWriter::OptimizationResultWriter(hid_t file_id)
{
    this->file_id = file_id;
    logParPEVersion();
}

OptimizationResultWriter::OptimizationResultWriter(const char *filename, bool overwrite)
{
    logmessage(LOGLVL_DEBUG, "Writing results to %s.", filename);
    mkpathConstChar(filename, 0755);
    initResultHDFFile(filename, overwrite);
    logParPEVersion();
}

void OptimizationResultWriter::logParPEVersion()
{
    hdf5WriteStringAttribute(file_id, rootPath.c_str(), "PARPE_VERSION", GIT_VERSION);
}

int OptimizationResultWriter::initResultHDFFile(const char *filename, bool overwrite)
{
    H5_SAVE_ERROR_HANDLER;;

    if(!overwrite) {
        struct stat st = {0};
        bool fileExists = stat(filename, &st) == 0;

        assert(!fileExists);
    }

    // overwrites TODO: backup
    file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    if(file_id < 0) {
        H5Eprint(H5E_DEFAULT, stderr);
    }

    H5_RESTORE_ERROR_HANDLER;;

    return file_id < 0;

}

void OptimizationResultWriter::closeResultHDFFile()
{
    H5_SAVE_ERROR_HANDLER;
    herr_t status = H5Fclose(file_id);

    if(status< 0) {
        error("closeResultHDFFile failed to close HDF5 file.");
        H5Eprint(H5E_DEFAULT, stderr);
    }
    H5_RESTORE_ERROR_HANDLER;
}

void OptimizationResultWriter::logLocalOptimizerObjectiveFunctionEvaluation(const double *parameters, int numParameters, double objectiveFunctionValue, const double *objectiveFunctionGradient, int numFunctionCalls, double timeElapsedInSeconds)
{
    const char *fullGroupPath = rootPath.c_str();

    hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "costFunCost", &objectiveFunctionValue, 1);

    if(objectiveFunctionGradient)
        hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "costFunGradient", objectiveFunctionGradient, numParameters);

    if(parameters)
        hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "costFunParameters", parameters, numParameters);

    hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "costFunWallTimeInSec", &timeElapsedInSeconds, 1);

    flushResultWriter();
}

void OptimizationResultWriter::logLocalOptimizerIteration(int numIterations, double *theta, int numParameters, double objectiveFunctionValue, const double *gradient, double timeElapsedInSeconds, int alg_mod, double inf_pr, double inf_du, double mu, double d_norm, double regularization_size, double alpha_du, double alpha_pr, int ls_trials)
{
    const char *fullGroupPath = rootPath.c_str();

    hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "iterCostFunCost", &objectiveFunctionValue, 1);
    if(gradient)
        hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "iterCostFunGradient", gradient, numParameters);
    if(theta)
        hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "iterCostFunParameters", theta, numParameters);
    hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "iterCostFunWallTimeInSec", &timeElapsedInSeconds, 1);

    hdf5CreateOrExtendAndWriteToInt2DArray(   file_id, fullGroupPath, "iterIndex", &numIterations, 1);
    hdf5CreateOrExtendAndWriteToInt2DArray(   file_id, fullGroupPath, "iterIpopt_alg_mod", &alg_mod, 1);
    hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "iterIpopt_inf_pr", &inf_pr, 1);
    hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "iterIpopt_inf_du", &inf_du, 1);
    hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "iterIpopt_mu", &mu, 1);
    hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "iterIpopt_d_norm", &d_norm, 1);
    hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "iterIpopt_regularization_size", &regularization_size, 1);
    hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "iterIpopt_alpha_du", &alpha_du, 1);
    hdf5CreateOrExtendAndWriteToDouble2DArray(file_id, fullGroupPath, "iterIpopt_alpha_pr", &alpha_pr, 1);
    hdf5CreateOrExtendAndWriteToInt2DArray(   file_id, fullGroupPath, "iterIpopt_ls_trials", &ls_trials, 1);

    flushResultWriter();
}


void OptimizationResultWriter::flushResultWriter()
{
    hdf5LockMutex();

    H5Fflush(file_id, H5F_SCOPE_LOCAL);

    hdf5UnlockMutex();

}

void OptimizationResultWriter::saveTotalCpuTime(const double timeInSeconds)
{
    hsize_t dims[1] = {1};

    hdf5LockMutex();
    // TODO respect rootPath
    //H5LTmake_dataset(file_id, (rootPath + "/totalWallTimeInSec").c_str(), 1, dims, H5T_NATIVE_DOUBLE, &timeInSeconds);
    H5LTmake_dataset(file_id, "/totalTimeInSec", 1, dims, H5T_NATIVE_DOUBLE, &timeInSeconds);

    hdf5UnlockMutex();

}

void OptimizationResultWriter::saveLocalOptimizerResults(double finalNegLogLikelihood, const double *optimalParameters, int numParameters, double masterTime, int exitStatus)
{
    std::string fullGroupPath;
    hsize_t dimensions[1] = {1};

    hdf5LockMutex();

    fullGroupPath = (rootPath + "/finalCost");
    H5LTmake_dataset(file_id, fullGroupPath.c_str(), 1, dimensions, H5T_NATIVE_DOUBLE, &finalNegLogLikelihood);

    fullGroupPath = (rootPath + "/wallTimeInSec");
    H5LTmake_dataset(file_id, fullGroupPath.c_str(), 1, dimensions, H5T_NATIVE_DOUBLE, &masterTime);

    fullGroupPath = (rootPath + "/exitStatus");
    H5LTmake_dataset(file_id, fullGroupPath.c_str(), 1, dimensions, H5T_NATIVE_INT, &exitStatus);

    if(optimalParameters) {
        fullGroupPath = (rootPath + "/finalParameters");
        dimensions[0] = numParameters;
        H5LTmake_dataset(file_id, fullGroupPath.c_str(), 1, dimensions, H5T_NATIVE_DOUBLE, optimalParameters);
    }

    hdf5UnlockMutex();

    flushResultWriter();
}

OptimizationResultWriter::~OptimizationResultWriter()
{
    closeResultHDFFile();
}