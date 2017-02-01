# FindOpenFace.cmake
# Uses environment variable OpenFace_ROOT as backup
# - OpenFace_FOUND
# - OpenFace_INCLUDE_DIRS
# - OpenFace_LIBRARIES

# message(${DEPENDS_DIR})

FIND_PATH(OpenFace_INCLUDE_DIRS
  LandmarkCoreIncludes.h
  DOC "Found OpenFace include directory"
  PATHS
    "/usr/local/include/OpenFace"
    ENV OpenFace_ROOT
  # PATH_SUFFIXES
  #   include
)

FIND_LIBRARY(OpenFace_LandmarkDetector_LIB
  NAMES libLandmarkDetector.a
  DOC "Found OpenFace library path"
  PATHS
    "/usr/local/lib"
    ENV OpenFace_ROOT
  # PATH_SUFFIXES
  #   lib
  #   lib64
)

FIND_LIBRARY(OpenFace_FaceAnalyser_LIB
  NAMES libFaceAnalyser.a
  DOC "Found OpenFace library path"
  PATHS
    "/usr/local/lib"
    ENV OpenFace_ROOT
  # PATH_SUFFIXES
  #   lib
  #   lib64
)

SET(OpenFace_LIBRARIES ${OpenFace_FaceAnalyser_LIB} ${OpenFace_LandmarkDetector_LIB})

# IF(WIN32)
# FIND_FILE(TurboJPEG_DLL
#   turbojpeg.dll
#   DOC "Found TurboJPEG DLL path"
#   PATHS
#     "${DEPENDS_DIR}/libjpeg_turbo"
#     "${DEPENDS_DIR}/libjpeg-turbo64"
#     "C:/libjpeg-turbo64"
#     ENV TurboJPEG_ROOT
#   PATH_SUFFIXES
#     bin
# )
# ENDIF()

IF(OpenFace_INCLUDE_DIRS AND OpenFace_LIBRARIES)
INCLUDE(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_INCLUDES ${OpenFace_INCLUDE_DIRS})
set(CMAKE_REQUIRED_LIBRARIES ${OpenFace_LIBRARIES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# check_cxx_source_compiles("#include <LandmarkCoreIncludes.h>\nint main(void) { return 0; }" OPENFACE_WORKS)
set(OPENFACE_WORKS TRUE)
set(CMAKE_REQUIRED_DEFINITIONS)
set(CMAKE_REQUIRED_INCLUDES)
set(CMAKE_REQUIRED_LIBRARIES)
ENDIF()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenFace FOUND_VAR OpenFace_FOUND
  REQUIRED_VARS OpenFace_LIBRARIES OpenFace_INCLUDE_DIRS OPENFACE_WORKS)
