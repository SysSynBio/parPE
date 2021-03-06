#!/bin/bash
SCRIPT_DIR="`dirname \"$0\"`"
SCRIPT_DIR="`( cd \"$SCRIPT_DIR\" && pwd )`"

set -e

BUILD_DIR=$1
VENV=${BUILD_DIR}/venv

source $VENV/bin/activate

cd ${BUILD_DIR}/examples/parpeamici/steadystate/steadystate_scaled-prefix/src/steadystate_scaled
$SCRIPT_DIR/create_steadystate_amici_model.py
