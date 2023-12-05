# Project dependencies
find_package(Boost REQUIRED COMPONENTS regex)
find_package(Python3 REQUIRED COMPONENTS Development NumPy)

if(BUILD_WITH_UNIREC)
	find_package(UNIREC 3.0.0 REQUIRED)
endif()

# Set define for none depricated API for NUMPY
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION")
