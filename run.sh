#!/bin/bash

if [[ "$1" == "--debug" && "$OSTYPE" == "darwin"* ]]; then
  CMD_STR="lldb -o run sensei"
elif  [[ "$1" == "--debug" && "$OSTYPE" == "linux-gnu" ]]; then
  CMD_STR="gdb -ex run sensei"
else
  CMD_STR="./sensei"
fi

if [[ "$OSTYPE" == "linux-gnu" ]]; then
  cd install && LD_LIBRARY_PATH=/home/sensei/Developer/sensei/3rdParty/of/lib-linux/release-x86_64-64 PYTHONPATH=.:/usr/local/lib/python2.7/dist-packages:/usr/local/lib/python2.7/dist-packages/mxnet-0.9.3-py2.7.egg:../3rdParty/mxnet_mtcnn $CMD_STR
elif [[ "$OSTYPE" == "darwin"* ]]; then
  cd install && PYTHONPATH=.:/usr/local/lib/python2.7/site-packages:/usr/local/lib/python2.7/site-packages/mxnet-0.9.3-py2.7-macosx-10.12-x86_64.egg:../3rdParty/mxnet_mtcnn $CMD_STR
else
  echo "Unsupported OS!"
fi
