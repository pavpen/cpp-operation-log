cmake_minimum_required(VERSION 3.5)

# DESCRIPTION "C++ Library for Logging Operation Tracing Debug Messages"
project(liboperationlog
    VERSION 0.1.0)

# Define variables for `cpack`
SET(MAJOR_VERSION 0)
SET(MINOR_VERSION 1)
SET(PATCH_VERSION 0)

include(GNUInstallDirs)

# Set language version:
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11" )

add_library(operationlog INTERFACE)

target_include_directories(operationlog INTERFACE
    "${PROJECT_SOURCE_DIR}/include")

# We made this a headers-only library, because ABIs, language versions and
# compilers make binary libraries a pain.
#
# Set SOVERSION to major number of VERSION, so, e.g., thelib.so.1 will be a
# symlink to libthelib.so.1.0.0.
#set_target_properties(operationlog PROPERTIES
#    VERSION ${PROJECT_VERSION}
#    SOVERSION 0
#    PUBLIC_HEADER include/operation_log.h)


# When expanding the pkg-config file, don't expand ${VAR}s:
configure_file(operationlog.pc.in operationlog.pc @ONLY)

# Declare files to install:
install(TARGETS operationlog
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES "${PROJECT_SOURCE_DIR}/include/operation_log.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT headers)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/operation_log"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT headers)

# Install the pkg-config file:
install(FILES ${CMAKE_BINARY_DIR}/operationlog.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)


# CPack variables for building a Debian package:
IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
    INCLUDE(InstallRequiredSystemLibraries)

    SET(CPACK_SET_DESTDIR "on")
    SET(CPACK_PACKAGING_INSTALL_PREFIX "/tmp")
    SET(CPACK_GENERATOR "DEB")
    
    SET(CPACK_PACKAGE_DESCRIPTION "C++ Library for Logging Operation Tracing Debug Messages")
    # Multi-line Deb fields have to have subsequent lines indented by 1 space,
    # and empty lines converted to '.':
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY
"It defines macros for debug logging function entry and exit,
 dumping variables, and sending messages to an output stream.
 .
 A plain text formatter is provided for logging the messages.
 Others, can be written.")
    SET(CPACK_PACKAGE_VENDOR "Pavel Penev")
    SET(CPACK_PACKAGE_CONTACT "Pavel Penev <pavpen@gmail.com>")
    SET(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
    SET(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
    SET(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
    SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
    SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")

    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.23), libgcc1 (>= 1:6.0.1), libstdc++6 (>= 5.3.1)")

    SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    SET(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

    SET(CPACK_COMPONENTS_ALL Libraries ApplicationData)
    INCLUDE(CPack)
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
