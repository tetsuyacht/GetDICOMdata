#!/bin/bash

# This script compiles getEchoSpacing.cxx and getSampleSpacing with GCC.
# Execute this script after ScriptCompileGetDICOMdataForCentOS6.sh,
# if devtoolset-3 is not installed.
#
# Apr 22nd 2019  T.Yamamoto


# For getEchoSpacing
scl enable devtoolset-3 "c++ -o getEchoSpacing ../src/getEchoSpacing.cxx -std=c++11"

# For getSampleSpacing
scl enable devtoolset-3 "c++ -o getSampleSpacing ../src/getSampleSpacing.cxx -std=c++11"
