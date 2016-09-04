# Top-Level CmakeLists
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

PROJECT(RarCrack!)

FIND_PACKAGE(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED gthread-2.0 gobject-2.0 glib-2.0)


# Compiler flags
IF(CMAKE_COMPILER_IS_GNUCC)
	SET(CMAKE_C_FLAGS "-g") # debug
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g") # warnings
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11") # Standard
ENDIF()

ENABLE_TESTING()

# Make sure the compiler can find include files from our src library.
include_directories(
    ${GLIB2_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

# Make sure the linker can find libraries once they are built.
link_directories(
    ${GLIB2_LIBRARY_DIRS}
    ${CMAKE_BINARY_DIR}/src
)

# CPack configuration
SET(CPACK_PACKAGE_NAME "RarCrack!")

## Setting contact and vendor details
SET(CPACK_PACKAGE_VENDOR "David Zoltan Kedves <kedazo@gmail.com>")
SET(CPACK_PACKAGE_CONTACT ${CPACK_PACKAGE_VENDOR})

SET(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE)
SET(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/README)

## Setting package description
SET(
	CPACK_PACKAGE_DESCRIPTION
	"Rar, Zip, 7zip password recovery tool"
)

SET(
	CPACK_PACKAGE_DESCRIPTION_SUMMARY
	"This program uses a brute force algorithm to guess your encrypted "
	"compressed file's password. If you forget your encrypted file password, "
	"this program is the solution. This program can crack zip, 7z and rar file "
	"passwords."
)

## Setting version number
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "3")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_GENERATOR "DEB;RPM;TGZ")

ADD_SUBDIRECTORY(docs)
ADD_SUBDIRECTORY(src)
#ADD_SUBDIRECTORY(tests)


INCLUDE(CPack)
