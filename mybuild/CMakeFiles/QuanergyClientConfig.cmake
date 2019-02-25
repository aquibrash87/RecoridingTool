# - Config file for the QuanergyClient package
#  It defines the following variables
#  QUANERGY_CLIENT_INCLUDE_DIRS - include directories for QuanergyClient
#  QUANERGY_CLIENT_LIBRARIES    - libraries to link against

# Compute paths
get_filename_component(QUANERGY_CLIENT_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(QUANERGY_CLIENT_INCLUDE_DIRS "${QUANERGY_CLIENT_CMAKE_DIR}/../../include;/usr/include/pcl-1.7;/usr/include/eigen3;/usr/include;/usr/include/ni;/usr/include/openni2;/usr/include/vtk-5.8")

# Our library dependencies (contains definitions for IMPORTED targets)
if((NOT TARGET quanergy_client) AND NOT QuanergyClient_BINARY_DIR)
  include("${QUANERGY_CLIENT_CMAKE_DIR}/QuanergyClientTargets.cmake")
endif()

# These are IMPORTED targets created by QuanergyClientTargets.cmake
set(QUANERGY_CLIENT_LIBRARIES quanergy_client)
