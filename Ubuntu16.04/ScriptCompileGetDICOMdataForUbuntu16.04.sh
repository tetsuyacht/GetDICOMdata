#!/bin/bash

# This script compiles getEchoSpacing.cxx and getSampleSpacing with GCC.
#
# Apr 22nd 2019  T.Yamamoto


# For getEchoSpacing
c++ -o getEchoSpacing ../src/getEchoSpacing.cxx -std=c++11

# For getSampleSpacing
c++ -o getSampleSpacing ../src/getSampleSpacing.cxx -std=c++11
