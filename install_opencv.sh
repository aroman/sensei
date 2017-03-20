#!/bin/bash

set -x # echo commands

OPENCV_VERSION="3.1.0"

echo "Downloading OpenCV..."
curl -SLO https://github.com/opencv/opencv/archive/$OPENCV_VERSION.tar.gz
tar xf $OPENCV_VERSION.tar.gz
rm $OPENCV_VERSION.tar.gz
cd opencv-$OPENCV_VERSION
mkdir build
cd build
echo "Installing OpenCV..."
cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=/usr/local -DWITH_TBB=ON -DBUILD_SHARED_LIBS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF ..
make -j4 install
cd "../.."
rm -rf opencv-$OPENCV_VERSION
echo "OpenCV installed."
