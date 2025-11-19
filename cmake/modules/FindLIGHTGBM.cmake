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

# Find headers
find_path(
    LIGHTGBM_INCLUDE_DIR
    NAMES LightGBM/c_api.h
    HINTS ${PC_LIGHTGBM_INCLUDEDIR} ${PC_LIGHTGBM_INCLUDE_DIRS}
          /usr/local/include
          /usr/include
)

# Find library
find_library(
    LIGHTGBM_LIBRARY
    NAMES lightgbm lib_lightgbm _lightgbm
    HINTS ${PC_LIGHTGBM_LIBDIR} ${PC_LIGHTGBM_LIBRARY_DIRS}
          /usr/local/lib
          /usr/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    LIGHTGBM
    REQUIRED_VARS LIGHTGBM_INCLUDE_DIR LIGHTGBM_LIBRARY
)

if(LIGHTGBM_FOUND)
    set(LIGHTGBM_INCLUDE_DIRS ${LIGHTGBM_INCLUDE_DIR})
    set(LIGHTGBM_LIBRARIES ${LIGHTGBM_LIBRARY})

    add_library(LightGBM::lightgbm SHARED IMPORTED)
    set_target_properties(LightGBM::lightgbm PROPERTIES
        IMPORTED_LOCATION "${LIGHTGBM_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${LIGHTGBM_INCLUDE_DIRS}"
    )
endif()

mark_as_advanced(LIGHTGBM_INCLUDE_DIR LIGHTGBM_LIBRARY)

