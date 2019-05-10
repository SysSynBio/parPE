#!/bin/bash
# build parPE dependencies (run downloadPackages.sh before)

SCRIPT_PATH="`dirname \"$0\"`"
SCRIPT_PATH="`( cd \"$SCRIPT_PATH\" && pwd )`"

# exit on error
set -e

../deps/AMICI/scripts/buildAll.sh

./installCBLAS.sh

./installCeres.sh

./installIpopt.sh
