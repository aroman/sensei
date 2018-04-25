FROM ubuntu:16.04

# Enable systemd init system in container
ENV \
INITSYSTEM=on \
LIBFREENECT2_VERSION=6304ac6437045503e382a6aaac524ee5cf4c3048 \
OPENFACE_VERSION=688f1abb07f7fc0b1979ffd40a08a8a9c76774b9 \
OPENCV_VERSION=3.2.0 \
DLIB_VERSION=19.4 \
OF_VERSION=f10c54a713b3bdb36c0bb088964dc1de56c206dc \
MXNET_VERSION=v0.9.3

WORKDIR /usr/src

# Everything at once so we don't generate a ton of big layers (at the cost of caching)
RUN apt-get -q update && apt-get install -yq --no-install-recommends \
  build-essential \
  curl \
  libtbb-dev \
  libboost-all-dev \
  libjpeg-dev \
  libpng-dev \
  libtiff-dev \
  libjasper-dev \
  liblapacke-dev \
  libjpeg-turbo8-dev \
  libturbojpeg \
  opencl-headers \
  ocl-icd-libopencl1 \
  ocl-icd-opencl-dev \
  libusb-1.0-0-dev \
  libusb-1.0-0 \
  pkg-config \
  libopenblas-dev \
  i965-va-driver \
  libva-dev \
  liblapack-dev \
  beignet-dev \
  xorg-dev \
  libudev-dev \
  libgtk-3-dev \
  libcairo2-dev \
  libssl-dev \
  libfreetype6-dev \
  libfontconfig1-dev \
  libglu1-mesa-dev \
  libmpg123-dev \
  libusb-1.0-0-dev \
  libopenal-dev \
  libassimp-dev \
  libopencv-dev \
  libasound2-dev \
  libsndfile1-dev \
  gstreamer1.0-x \
  gstreamer1.0-alsa \
  gstreamer1.0-libav \
  gstreamer1.0-pulseaudio \
  gstreamer1.0-plugins-base \
  gstreamer1.0-plugins-good \
  gstreamer1.0-plugins-bad \
  gstreamer1.0-plugins-ugly \
  libgstreamer1.0-dev \
  libgstreamer-plugins-base1.0-dev \
  python3-pip \
  python-numpy \
  python-setuptools \
  python-pip \
  graphviz \
  git \
  cmake \
  && apt-get clean && rm -rf /var/lib/apt/lists/*

# Install libfreenect2
RUN curl -SLo libfreenect2-${LIBFREENECT2_VERSION}.tar.gz https://github.com/OpenKinect/libfreenect2/archive/${LIBFREENECT2_VERSION}.tar.gz && \
  tar xf libfreenect2-${LIBFREENECT2_VERSION}.tar.gz && \
  cd libfreenect2-${LIBFREENECT2_VERSION} && \
  mkdir build && \
  cd build && \
  cmake -DCMAKE_BUILD_TYPE=RELEASE  -DENABLE_CXX11=TRUE -DENABLE_OPENCL=TRUE -DENABLE_OPENGL=FALSE -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
  make -j$(grep -c processor /proc/cpuinfo) install && \
  cp ../platform/linux/udev/90-kinect2.rules /lib/udev/rules.d/ && \
  cd .. && rm -rf libfreenect2-${LIBFREENECT2_VERSION}*

RUN curl -SLo opencv-${OPENCV_VERSION}.tar.gz https://github.com/opencv/opencv/archive/${OPENCV_VERSION}.tar.gz && \
  tar xf opencv-${OPENCV_VERSION}.tar.gz && \
  cd opencv-${OPENCV_VERSION} && \
  mkdir build && \
  cd build && \
  cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=/usr/local -DWITH_TBB=ON -DBUILD_SHARED_LIBS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF .. && \
  make -j$(grep -c processor /proc/cpuinfo) install && \
  cd .. && rm -rf opencv-${OPENCV_VERSION}*

RUN curl -SLO http://dlib.net/files/dlib-${DLIB_VERSION}.tar.bz2 && \
  tar xf dlib-${DLIB_VERSION}.tar.bz2 && \
  cd dlib-${DLIB_VERSION} && \
  mkdir build && \
  cd build && \
  cmake -DUSE_AVX_INSTRUCTIONS=1 -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
  make -j$(grep -c processor /proc/cpuinfo) install && \
  cd .. && rm -rf dlib-${DLIB_VERSION}* && \

  curl -SLo OpenFace-${OPENFACE_VERSION}.tar.gz https://github.com/TadasBaltrusaitis/OpenFace/archive/${OPENFACE_VERSION}.tar.gz && \
  tar xf OpenFace-${OPENFACE_VERSION}.tar.gz && \
  cd OpenFace-${OPENFACE_VERSION} && \
  mkdir build && \
  cd build && \
  cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=/usr/local .. && \
  make -j$(grep -c processor /proc/cpuinfo) install && \
  cd .. && rm -rf OpenFace-${OPENFACE_VERSION}*

RUN git clone --recursive --branch ${MXNET_VERSION} https://github.com/dmlc/mxnet.git && \
  cd mxnet && \
  sed -i '/add_subdirectory(example.*/d' CMakeLists.txt && \
  mkdir build && \
  cd build && \
  cmake -DUSE_CUDA=OFF -DUSE_OPENMP=ON .. && \
  make -j$(grep -c processor /proc/cpuinfo) && \
  mv libmxnet.so ../python/mxnet/ && \
  cd ../python && \
  python setup.py install && \
  cd .. && rm -rf mxnet && cd ..

RUN git clone --depth=1 https://github.com/ofnode/of && \
  cd of && \
  git checkout ${OF_VERSION} && \
  mkdir build && \
  cd build && \
  cmake -DCMAKE_BUILD_TYPE=Release .. && \
  make -j$(grep -c processor /proc/cpuinfo) && \
  cd ..

# Commit hash of synergylabs/sensei to deploy
env SENSEI_VERSION=54c6178ff26c94c54c10b118baf5c9fdf90d933c

# Build sensei
RUN git clone --depth=1 --recursive https://github.com/synergylabs/sensei.git && \
  cd sensei && \
  git checkout ${SENSEI_VERSION} && \
  mkdir build && \
  cd build && \
  cmake -DCMAKE_INSTALL_PREFIX=../install .. && \
  make -j$(grep -c processor /proc/cpuinfo) install && \
  cd .. && \
  rm -rf build

# Run sensei
CMD cd /usr/src/sensei/install && LD_LIBRARY_PATH=/usr/src/of/lib-linux/release-x86_64-64:/usr/local/lib PYTHONPATH=.:/usr/local/lib/python2.7/dist-packages:/usr/local/lib/python2.7/dist-packages/mxnet-0.9.3-py2.7.egg:../3rdParty/mxnet_mtcnn ./sensei
