#!/bin/bash

# This script installs devtoolset-3.
# Execute this script before ScriptCompileGetDICOMdataForCentOS6.sh as superuser,
# if devtoolset-3 is not installed.
#
# Apr 22nd 2019  T.Yamamoto


cd /etc/yum.repos.d/
wget https://copr-fe.cloud.fedoraproject.org/coprs/rhscl/devtoolset-3/repo/epel-6/rhscl-devtoolset-3-epel-6.repo
yum install devtoolset-3-gcc devtoolset-3-binutils
yum install devtoolset-3-gcc-c++ devtoolset-3-gcc-gfortran
