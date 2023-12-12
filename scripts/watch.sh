#!/bin/bash
export PROJ_ROOT=/root/target-aid
ls ${PROJ_ROOT}/CMakeLists.txt ${PROJ_ROOT}/cpp/* | entr -d "./autobuild.sh"
