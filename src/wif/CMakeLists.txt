set(LIBWIF_SOURCES
	classifiers/classifier.cpp
	classifiers/regexClassifier.cpp
	classifiers/scikitMlClassifier.cpp
	combinators/averageCombinator.cpp
	combinators/binaryDSTCombinator.cpp
	combinators/sumCombinator.cpp
	filesystem/fileModificationChecker.cpp
	ml/scikitMlWrapper.cpp
	regex/regexPattern.cpp
	storage/flowFeatures.cpp
	storage/ipAddress.cpp
	utils/ipPrefix.cpp
)

set(LIBWIF_LIBS
	Boost::regex
	Python3::Python
	Python3::NumPy
	dstlib::dst
)

if(BUILD_WITH_UNIREC)
	list(APPEND LIBWIF_SOURCES
		classifiers/alfClassifier.cpp
		reporters/unirecReporter.cpp
	)
	list(APPEND LIBWIF_LIBS
		unirec::unirec
		unirec::unirec++
	)
endif()

add_library(wif SHARED ${LIBWIF_SOURCES})
target_link_libraries(wif PRIVATE ${LIBWIF_LIBS})
target_include_directories(wif PUBLIC ${CMAKE_SOURCE_DIR}/include)

install(TARGETS wif LIBRARY DESTINATION ${INSTALL_DIR_LIB})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${INSTALL_DIR_INCLUDE})
