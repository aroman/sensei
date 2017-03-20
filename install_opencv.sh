#!/bin/bash

OPENCV_VERSION="OPENCV_VERSION"

echo "Downloading OpenCV..."
if [ -d "opencv-$OPENCV_VERSION" ]; then
  sudo rm -r "opencv-$OPENCV_VERSION"
fi

curl -sLO https://github.com/opencv/opencv/archive/$OPENCV_VERSION.tar.gz
tar xf $OPENCV_VERSION.tar.gz
rm $OPENCV_VERSION.tar.gz
cd opencv-$OPENCV_VERSION
mkdir build
cd build
echo "Installing OpenCV..."
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D BUILD_SHARED_LIBS=OFF ..
make -j4
sudo make install
cd "../.."
echo "OpenCV installed."
