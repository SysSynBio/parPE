#include "testingMisc.h"

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <fcntl.h> // O_WRONLY
#include <cassert>
#include <algorithm>
#include <random>

#include <gsl/gsl-lite.hpp>
#include <gtest/gtest.h>

namespace parpe {

bool withinTolerance(double expected, double actual, double atol, double rtol,
                     int index) {
    bool withinTol =  fabs(expected - actual) <= atol
            || fabs((expected - actual) / (rtol + expected)) <= rtol;

    if(!withinTol && std::isnan(expected) && std::isnan(actual))
        withinTol = true;

    if(!withinTol && std::isinf(expected) && std::isinf(actual))
        withinTol = true;

    if(!withinTol) {
        fprintf(stderr, "ERROR: Expected value %e, but was %e at index %d.\n",
                expected, actual, index);
        fprintf(stderr, "       Relative error: %e (tolerance was %e)\n",
                fabs((expected - actual) / (rtol + expected)), rtol);
        fprintf(stderr, "       Absolute error: %e (tolerance was %e)\n",
                fabs(expected - actual), atol);
        //printBacktrace(12);
    }

    return withinTol;
}

void checkEqualArray(const double *expected, const double *actual, int length,
                     double atol, double rtol) {
    if(!expected && !actual)
        return;

    EXPECT_TRUE(expected && actual);
    if(!(expected && actual)) {
        // in case EXPECT_TRUE does not exit
        return;
    }

    for(int i = 0; i < length; ++i)
    {
        bool withinTol = withinTolerance(expected[i], actual[i], atol, rtol, i);
        EXPECT_TRUE(withinTol);
    }
}

int randInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);

    return dis(gen);
}

std::string captureStreamToString(const std::function<void()>& f,
                                  std::ostream &os) {
    std::streambuf* oldOStreamBuf = os.rdbuf();
    os.flush();

    std::ostringstream strOs;
    os.rdbuf(strOs.rdbuf());

    f();

    strOs.flush();
    os.rdbuf( oldOStreamBuf );

    return strOs.str();
}

std::string captureStreamToString(const std::function<void()>& f,
                                  std::FILE* captureStream,
                                  int captureStreamFd) {
    char tempFileName [] = "parpeTestCaptureXXXXXX";
    int newStreamFd = mkstemp(tempFileName);
    Expects(newStreamFd >= 0);

    int oldStreamFd = dup(captureStreamFd);
    Expects(oldStreamFd >= 0);
    fflush(captureStream);

    dup2(newStreamFd, captureStreamFd); // replace original fd by tmp file
    close(newStreamFd);  // close remaining copy

    f();
    fflush(captureStream);

    dup2(oldStreamFd, captureStreamFd); // restore (closes tmp file)
    close(oldStreamFd); // close remainingv copy

    std::ifstream ifs(tempFileName);

    return std::string ((std::istreambuf_iterator<char>(ifs)),
                     std::istreambuf_iterator<char>());
}

double getLogLikelihoodOffset(int n) {
    const double pi = atan(1) * 4.0;
    return - n * 0.5 * log(2.0 * pi);
}

} // namespace parpe
