# Project dependencies
find_package(Armadillo REQUIRED)
find_package(Boost REQUIRED COMPONENTS regex serialization)
find_package(MLPACK REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Development NumPy)

if(BUILD_WITH_UNIREC)
	find_package(UNIREC 3.0.0 REQUIRED)
	find_package(UNIREC++ 3.2.0 REQUIRED)
endif()

# Set define for none depricated API for NUMPY
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION")

if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    add_compile_options(${OpenMP_CXX_FLAGS})
endif()

