# DST library
#
# DST library is C++ implementation of Dempster-Shafer Theory.
#
# "dstlib" is exposed to be used as a dependency in other CMake targets
# example usage: target_link_libraries(my_target PRIVATE dstlib::dstlib)

# Build static library
set(DSTLIB_BUILD_SHARED OFF)
# Disable generating of installation targets
set(DSTLIB_INSTALL_TARGETS OFF)
# Disable RPM package builder
set(DSTLIB_PACKAGE_BUILDER OFF)

if(DEFINED ENV{DSTLIB_CI_TOKEN})
	set(GIT_REPO https://$ENV{DSTLIB_CI_TOKEN}@gitlab.liberouter.org/feta/wif-group/dst.git)
else()
	set(GIT_REPO https://gitlab.liberouter.org/feta/wif-group/dst.git)
endif()

FetchContent_Declare(
	dstlib
	GIT_REPOSITORY ${GIT_REPO}
	GIT_TAG v1.0.1
	GIT_SHALLOW 1
)

# Make sure that subproject accepts predefined build options without warnings.
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

FetchContent_MakeAvailable(dstlib)
