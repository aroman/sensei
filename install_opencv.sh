#!/bin/bash

OPENCV_VERSION="OPENCV_VERSION"

echo "Downloading OpenCV..."
if [ -d "opencv-$OPENCV_VERSION" ]; then
  sudo rm -r "opencv-$OPENCV_VERSION"
fi

wget https://github.com/Itseez/opencv/archive/$OPENCV_VERSION.zip
unzip $OPENCV_VERSION.zip
rm $OPENCV_VERSION.zip
cd opencv-$OPENCV_VERSION
mkdir build
cd build
echo "Installing OpenCV..."
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D BUILD_SHARED_LIBS=OFF ..
make -j4
sudo make install
cd "../.."
echo "OpenCV installed."
