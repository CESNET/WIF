set(LIBWIF_SOURCES
	classifiers/classifier.cpp
	classifiers/ipPrefixClassifier.cpp
	classifiers/lightGBMClassifier.cpp
	classifiers/mlpackClassifier.cpp
	classifiers/regexClassifier.cpp
	classifiers/scikitMlClassifier.cpp
	combinators/averageCombinator.cpp
	combinators/binaryDSTCombinator.cpp
	combinators/majorityCombinator.cpp
	combinators/sumCombinator.cpp
	filesystem/fileModificationChecker.cpp
	ml/lightGBMWrapper.cpp
	ml/mlpackModels/decisionTreeModel.cpp
	ml/mlpackModels/hoeffdingTreeModel.cpp
	ml/mlpackModels/linearSVMModel.cpp
	ml/mlpackModels/mlpackModel.cpp
	ml/mlpackModels/naiveBayesModel.cpp
	ml/mlpackModels/perceptronModel.cpp
	ml/mlpackModels/randomForestModel.cpp
	ml/mlpackWrapper.cpp
	ml/scikitMlWrapper.cpp
	regex/regexPattern.cpp
	storage/flowFeatures.cpp
	storage/ipAddress.cpp
	utils/ipPrefix.cpp
)

set(LIBWIF_LIBS
	armadillo
	Boost::regex
	Boost::serialization
	dstlib::dst
	LightGBM::lightgbm
	OpenMP::OpenMP_CXX
	Python3::Python
	Python3::NumPy
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
