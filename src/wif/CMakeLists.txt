add_library(wif SHARED
	classifiers/classifier.cpp
	combinators/averageCombinator.cpp
	combinators/binaryDSTCombinator.cpp
	combinators/sumCombinator.cpp
	flowFeatures.cpp
	wif.cpp
)

target_link_libraries(wif PRIVATE
	Boost::regex
	Python3::Python
	Python3::NumPy
	dstlib::dst
)

target_include_directories(wif PUBLIC ${CMAKE_SOURCE_DIR}/include)

install(TARGETS wif LIBRARY DESTINATION ${INSTALL_DIR_LIB})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${INSTALL_DIR_INCLUDE})
