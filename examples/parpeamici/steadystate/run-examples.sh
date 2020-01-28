#!/usr/bin/env bash
# Run steadystate example for testing
# Expects to be run from within the steadystate example build directory
# ARG1: hdf5file
set -e
set -x

HDF5_FILE=$1
HDF5_FILE_TEST=$2
MPIEXEC="mpiexec --oversubscribe -n 5"
# Allow running as root in docker
grep docker /proc/1/cgroup -qa && MPIEXEC="${MPIEXEC} --allow-run-as-root"
# If we are running in docker, we generally don't have SYS_PTRACE permissions
# and thus, cannot use vader. Also disable Infiniband.
grep docker /proc/1/cgroup -qa && mpiexec --version | grep open-mpi && MPIEXEC="${MPIEXEC} --oversubscribe --mca btl_vader_single_copy_mechanism none --mca btl ^openib -mca oob_tcp_if_include lo --mca btl_tcp_if_include lo --mca orte_base_help_aggregate 0"


rm -f test.log

# Run without MPI

# Run gradient check
rm -rf example_steadystate_multi-test-gradient/
./example_steadystate_multi -t gradient_check \
  -o example_steadystate_multi-test-gradient/ ${HDF5_FILE} 2>&1 > test.log
(! grep ERR test.log)

# Run optimization with default settings
rm -rf example_steadystate_multi-test-optimize/
./example_steadystate_multi \
  -o example_steadystate_multi-test-optimize/ ${HDF5_FILE}

# Simulate at optimum
rm -f simulate1.h5
./example_steadystate_multi_simulator \
  example_steadystate_multi-test-optimize/_rank00000.h5 / simulate1.h5 / \
  --at-optimum --nompi 2>&1 > test.log
(! grep ERR test.log)
(! grep WRN test.log)
(! grep exception test.log)
h5dump -d /multistarts/0/yMes/3 simulate1.h5 # test dataset exists
test -f simulate1.h5

#
# Run with MPI
#

# Run optimization with default settings

rm -rf example_steadystate_multi-test-optimize/
${MPIEXEC} ./example_steadystate_multi --mpi \
  -o example_steadystate_multi-test-optimize/ ${HDF5_FILE} 2>&1 >> test.log
(! grep ERR test.log)
(! grep WRN test.log)

# Simulate along trajectory

rm -f simulate2.h5
${MPIEXEC} ./example_steadystate_multi_simulator \
  example_steadystate_multi-test-optimize/_rank00000.h5 / simulate2.h5 / \
  --along-trajectory --mpi 2>&1 >> test.log
(! grep ERR test.log)
(! grep WRN test.log)
(! grep exception test.log)
test -f simulate2.h5


# Simulate on test set

rm -f simulate3.h5
${MPIEXEC} ./example_steadystate_multi_simulator \
  ${HDF5_FILE_TEST} / example_steadystate_multi-test-optimize/_rank00000.h5 / \
  simulate3.h5 / --at-optimum --mpi
h5dump -d /multistarts/0/ySim/3 simulate3.h5 # test dataset exists
(! grep ERR test.log)
(! grep WRN test.log)
(! grep exception test.log)
test -f simulate3.h5