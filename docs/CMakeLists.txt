
FIND_PACKAGE(Doxygen)

# check if doxygen is even installed
if (DOXYGEN_FOUND)
	# prepare doxygen configuration file
	CONFIGURE_FILE(
		${CMAKE_CURRENT_SOURCE_DIR}/api/Doxyfile.in
		${CMAKE_CURRENT_BINARY_DIR}/api/Doxyfile
		@ONLY
	)

	# Generate new target to build the html documentation
	ADD_CUSTOM_COMMAND(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/api/html/index.html
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/api/Doxyfile
		DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/api/Doxyfile
	)

	# add doc as target
	ADD_CUSTOM_TARGET(
		doc ALL
		DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/api/html/index.html
		COMMENT "Building doxygen documentation"
	)

	# cleanup ${PROJECT_BINARY_DIR}/docs/api on "make clean"
	SET_PROPERTY(
		DIRECTORY APPEND PROPERTY
		ADDITIONAL_MAKE_CLEAN_FILES
		docs/api
	)

	SET(DOC_PATH "share/doc/${CPACK_PACKAGE_NAME}-${VERSION}")

	# install HTML API documentation and manual pages
	INSTALL(
		DIRECTORY   ${CMAKE_CURRENT_BINARY_DIR}/api/html
		DESTINATION ${DOC_PATH}
	)

	# install man pages into packages, scope is now project root..
	INSTALL(
		DIRECTORY   ${CMAKE_CURRENT_BINARY_DIR}/api/man/man3
		DESTINATION share/man/man3/
	)

else (DOXYGEN_FOUND)
  MESSAGE(STATUS "Doxygen not found, we cannot generate the documentation")
endif (DOXYGEN_FOUND)

