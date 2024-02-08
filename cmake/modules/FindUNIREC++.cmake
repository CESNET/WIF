# Find the Unirec++ includes and library
#
# This module defines the following IMPORTED targets:
#
#  unirec::unirec++      - The "unirec++" library, if found.
#
# This module will set the following variables in your project:
#
#  UNIREC++_INCLUDE_DIRS - where to find <unirec++/unirec++.hpp>, etc.
#  UNIREC++_LIBRARIES    - List of libraries when using unirec++.
#  UNIREC++_FOUND        - True if the unirec++ library has been found.

# Use pkg-config (if available) to get the library directories and then use
# these values as hints for find_path() and find_library() functions.
find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
	pkg_check_modules(PC_UNIREC++ QUIET unirec++)
endif()

find_path(
	UNIREC++_INCLUDE_DIR unirec++
	HINTS ${PC_UNIREC++_INCLUDEDIR} ${PC_UNIREC++_INCLUDE_DIRS}
	PATH_SUFFIXES include
)

find_library(
	UNIREC++_LIBRARY NAMES unirec++
	HINTS ${PC_UNIREC++_LIBDIR} ${PC_UNIREC++_LIBRARY_DIRS}
	PATH_SUFFIXES lib lib64
)

if (PC_UNIREC++_VERSION)
	# Version extracted from pkg-config
	set(UNIREC++_VERSION_STRING ${PC_UNIREC++_VERSION})
endif()

# Handle find_package() arguments (i.e. QUIETLY and REQUIRED) and set
# UNIREC++_FOUND to TRUE if all listed variables are filled.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	UNIREC++
	REQUIRED_VARS UNIREC++_LIBRARY UNIREC++_INCLUDE_DIR
	VERSION_VAR UNIREC++_VERSION_STRING
)

set(UNIREC++_INCLUDE_DIRS ${UNIREC++_INCLUDE_DIR})
set(UNIREC++_LIBRARIES ${UNIREC++_LIBRARY})
mark_as_advanced(UNIREC++_INCLUDE_DIR UNIREC++_LIBRARY)

if (UNIREC++_FOUND)
	# Create imported library with all dependencies
	if (NOT TARGET unirec::unirec++ AND EXISTS "${UNIREC++_LIBRARIES}")
		add_library(unirec::unirec++ UNKNOWN IMPORTED)
		set_target_properties(unirec::unirec++ PROPERTIES
			IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
			IMPORTED_LOCATION "${UNIREC++_LIBRARIES}"
			INTERFACE_INCLUDE_DIRECTORIES "${UNIREC++_INCLUDE_DIRS}")
	endif()
endif()
