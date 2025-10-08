# Find the LightGBM includes and library
#
# This module defines the following IMPORTED targets:
#
#  LightGBM::lightgbm   - The LightGBM library, if found.
#
# This module will set the following variables in your project:
#
#  LIGHTGBM_INCLUDE_DIRS - where to find <LightGBM/c_api.h>, etc.
#  LIGHTGBM_FOUND        - True if the LightGBM library has been found.

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_LIGHTGBM QUIET lightgbm)
endif()

find_path(
    LIGHTGBM_INCLUDE_DIR lightgbm
    NAMES LightGBM/c_api.h
    HINTS ${PC_LIGHTGBM_INCLUDEDIR} ${PC_LIGHTGBM_INCLUDE_DIRS}
    PATH_SUFFIXES include
)

if(PC_LIGHTGBM_VERSION)
    # Version extracted from pkg-config
    set(LIGHTGBM_VERSION_STRING ${PC_LIGHTGBM_VERSION})
endif()

# Handle find_package() arguments (i.e. QUIETLY and REQUIRED) and set
# LIGHTGBM_FOUND to TRUE if all listed variables are filled.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    LIGHTGBM
    REQUIRED_VARS LIGHTGBM_INCLUDE_DIR
    VERSION_VAR LIGHTGBM_VERSION_STRING
)

set(LIGHTGBM_INCLUDE_DIRS ${LIGHTGBM_INCLUDE_DIR})
mark_as_advanced(LIGHTGBM_INCLUDE_DIR)

