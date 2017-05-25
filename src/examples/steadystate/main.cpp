#include <stdio.h>
#include "steadystateProblem.h"

int main(int argc, char **argv)
{

    SteadystateProblem problem = SteadystateProblem();

    printf("#########\n");
    printf("# IpOpt #\n");
    printf("#########\n");

    int status = getLocalOptimum(&problem);

    printf("#########\n");
    printf("# CERES #\n");
    printf("#########\n");

    problem.optimizer = OPTIMIZER_CERES;
    status += getLocalOptimum(&problem);

    return status;
}
