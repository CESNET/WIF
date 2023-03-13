add_library(wif SHARED
	wif.cpp
)

install(TARGETS wif LIBRARY DESTINATION ${INSTALL_DIR_LIB})
