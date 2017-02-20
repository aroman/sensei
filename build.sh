#!/bin/bash

bold=$(tput bold)
normal=$(tput sgr0)

mkdir -p build \
&& cd build \
&& cmake .. -DCMAKE_INSTALL_PREFIX=../install \
&& make -j4 install

if [ $? -eq 0 ]; then
    echo -e "\nBuild complete! Launch with ${bold}./run.sh${normal} or debug with ${bold}./run${normal} --debug"
else
  echo -e "\nBuild failed! Try ./run.sh --debug."
    echo FAIL
fi
