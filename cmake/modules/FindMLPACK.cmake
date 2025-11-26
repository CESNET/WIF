# Find the mlpack includes and library
#
# This module defines the following IMPORTED targets:
#
#  mlpack::mlpack          - The "mlpack" library, if found.
#
# This module will set the following variables in your project:
#
#  MLPACK_INCLUDE_DIRS - where to find <mlpack/core.hpp>, etc.
#  MLPACK_FOUND        - True if the mlpack library has been found.

# Use pkg-config (if available) to get the library directories and then use
# these values as hints for find_path() and find_library() functions.
find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
	pkg_check_modules(PC_MLPACK QUIET mlpack)
endif()

find_path(
	MLPACK_INCLUDE_DIR mlpack
	HINTS ${PC_MLPACK_INCLUDEDIR} ${PC_MLPACK_INCLUDE_DIRS}
	PATH_SUFFIXES include
)

if (PC_MLPACK_VERSION)
	# Version extracted from pkg-config
	set(MLPACK_VERSION_STRING ${PC_MLPACK_VERSION})
endif()

# Handle find_package() arguments (i.e. QUIETLY and REQUIRED) and set
# MLPACK_FOUND to TRUE if all listed variables are filled.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	MLPACK
	REQUIRED_VARS MLPACK_INCLUDE_DIR
	VERSION_VAR MLPACK_VERSION_STRING
)

set(MLPACK_INCLUDE_DIRS ${MLPACK_INCLUDE_DIR})
mark_as_advanced(MLPACK_INCLUDE_DIR)
