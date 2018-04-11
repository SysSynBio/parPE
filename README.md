 [![Run Status](https://api.shippable.com/projects/59463d3e8993d7070010407b/badge?branch=master)](https://app.shippable.com/github/dweindl/parPE)
 [![Coverage Badge](https://api.shippable.com/projects/59463d3e8993d7070010407b/coverageBadge?branch=master)](https://app.shippable.com/github/dweindl/parPE) 

# parPE

The *parPE* library provides functionality for solving large-scale parameter optimization
problems requiring thousands of simulations per objective function evaluation on HPC systems.

Currently, parPE provides interfaces to the
[IpOpt](http://www.coin-or.org/Ipopt/),
[Ceres](http://ceres-solver.org/),
[FFSQP](https://www.isr.umd.edu/news/news_story.php?id=4088) and
[SUMSL (CALGO/TOMS 611)](http://www.netlib.org/toms/index.html)
optimizers. parPE offers easy integration with
[AMICI](https://github.com/ICB-DCM/AMICI)-generated ordinary differential equation (ODE) models.

## Features

parPE offers the following features:

* MPI-based load-balancing of individual simulations
* improved load balancing by intermingling multiple optimization runs (multi-start local optimization)
* simple integration with SBML models via AMICI
* interfaces to Ipopt and Ceres optimizers
* HDF5 I/O compatible with a wide variety of programming languages
* Good parallel scaling to up to several thousand cores (highly problem dependent)

## Dependencies

For full functionality, parPE requires the following libraries:

* CMAKE (>=2.8)
* MPI
* PTHREADS
* IPOPT (>= 1.2.7) (requires coinhsl)
* CERES (>=1.13) (requires Eigen)
* HDF5 (>= 1.10)
* BLAS (CBLAS / Intel MKL)
* [AMICI](https://github.com/ICB-DCM/AMICI) (included in this repository) (uses SuiteSparse, Sundials)

On Debian-based systems, dependencies can be installed via:
```
sudo apt-get install build-essential gfortran libmpich-dev libblas-dev libhdf5-dev cmake \
    libceres-dev coinor-libipopt-dev libcpputest-dev libboost-serialization-dev
```

Scripts to fetch and build the remaining dependencies are provided in `/ThirdParty/` :

```
cd ThirdParty
./downloadPackages.sh
./installDeps.sh
```

## Building

After having taken care of the dependencies listed above, parPE can be built: 

```
./build.sh
```

Other sample build scripts are provided as `/build*.sh`.

## Tested compilers

* GCC 7.2.0 (IpOpt 1.2.9 requires libgfortran.so.4)
* Intel 16.0.4


## Documentation & further information

No extensive full-text documentation is available yet. See `*/examples` and `*/tests` for usage examples. 
Little documentation is available in `doc` and among github issues. 

## FAQ

Q: The program is killed due to memory exhaustion, what should I do?

A: When running with MPI, the master process (rank 0) is consuming more memory than the others.
Consider reserving more memory for this one. For LoadLeveler, this can be done conveniently via
`#@ first_node_tasks`.
