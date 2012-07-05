# This script locates the Thor library
# Based on the FindSFML.cmake module
# ------------------------------------
#
# Usage
# -----
#
# By default, the dynamic libraries of Thor will be found. To find the static ones instead,
# you must set the THOR_STATIC_LIBRARIES variable to TRUE before calling find_package(Thor).
# In case of static linking, the THOR_STATIC macro will also be defined by this script.
# example:
#   set(THOR_STATIC_LIBRARIES TRUE)
#   find_package(Thor 2)
#
# If Thor is not installed in a standard path, you can use the THOR_ROOT CMake (or environment) variable
# to tell CMake where Thor is.
#
# Output
# ------
#
# This script defines the following variables:
# - THOR_LIBRARY_DEBUG:   the name of the debug library (set to THOR_LIBRARY_RELEASE is no debug version is found)
# - THOR_LIBRARY_RELEASE: the name of the release library (set to THOR_LIBRARY_DEBUG is no release version is found)
# - THOR_LIBRARY:         the name of the library to link to (includes both debug and optimized names if necessary)
# - THOR_FOUND:           true if either the debug or release library is found
# - THOR_INCLUDE_DIR:     the path where Thor headers are located (the directory containing the Thor/Config.hpp file)
#
# example:
#   find_package(Thor 2 REQUIRED)
#   include_directories(${THOR_INCLUDE_DIR})
#   add_executable(myapp ...)
#   target_link_libraries(myapp ${THOR_LIBRARY})

# define the THOR_STATIC macro if static build was chosen
if(THOR_STATIC_LIBRARIES)
    add_definitions(-DTHOR_STATIC)
endif()

# deduce the libraries suffix from the options
set(FIND_THOR_LIB_SUFFIX "")
if(THOR_STATIC_LIBRARIES)
    set(FIND_THOR_LIB_SUFFIX "${FIND_THOR_LIB_SUFFIX}-s")
endif()

# find the SFML include directory
find_path(THOR_INCLUDE_DIR Thor/Config.hpp
          PATH_SUFFIXES include
          PATHS
          ${THOR_ROOT}
          $ENV{THOR_ROOT}
          /usr/local/
          /usr/
          /sw          # Fink
          /opt/local/  # DarwinPorts
          /opt/csw/    # Blastwave
          /opt/)

# check the version number
set(THOR_VERSION_OK TRUE)
if(THOR_FIND_VERSION AND THOR_INCLUDE_DIR)
    # extract the major and minor version numbers from Thor/Config.hpp
    set(THOR_CONFIG_HPP_INPUT "${THOR_INCLUDE_DIR}/Thor/Config.hpp")
    FILE(READ "${THOR_CONFIG_HPP_INPUT}" THOR_CONFIG_HPP_CONTENTS)
    STRING(REGEX MATCH ".*#define THOR_VERSION_MAJOR ([0-9]+).*#define THOR_VERSION_MINOR ([0-9]+).*" THOR_CONFIG_HPP_CONTENTS "${THOR_CONFIG_HPP_CONTENTS}")
    STRING(REGEX REPLACE ".*#define THOR_VERSION_MAJOR ([0-9]+).*" "\\1" THOR_VERSION_MAJOR "${THOR_CONFIG_HPP_CONTENTS}")
    STRING(REGEX REPLACE ".*#define THOR_VERSION_MINOR ([0-9]+).*" "\\1" THOR_VERSION_MINOR "${THOR_CONFIG_HPP_CONTENTS}")
    math(EXPR THOR_REQUESTED_VERSION "${THOR_FIND_VERSION_MAJOR} * 10 + ${THOR_FIND_VERSION_MINOR}")

    # if we could extract them, compare with the requested version number
    if (THOR_VERSION_MAJOR)
        # transform version numbers to an integer
        math(EXPR THOR_VERSION "${THOR_VERSION_MAJOR} * 10 + ${THOR_VERSION_MINOR}")

        # compare them
        if(THOR_VERSION LESS THOR_REQUESTED_VERSION)
            set(THOR_VERSION_OK FALSE)
        endif()
    else()
        # Thor version is < 2.0
        if (THOR_REQUESTED_VERSION GREATER 19)
            set(THOR_VERSION_OK FALSE)
            set(THOR_VERSION_MAJOR 1)
            set(THOR_VERSION_MINOR x)
        endif()
    endif()
endif()

# find the requested modules
set(THOR_FOUND TRUE) # will be set to false if one of the required modules is not found
set(FIND_THOR_LIB_PATHS
    ${THOR_ROOT}
    $ENV{THOR_ROOT}
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

# debug library
find_library(THOR_LIBRARY_DEBUG
             NAMES thor${FIND_THOR_LIB_SUFFIX}-d
             PATH_SUFFIXES lib64 lib
             PATHS ${FIND_THOR_LIB_PATHS})

# release library
find_library(THOR_LIBRARY_RELEASE
             NAMES thor${FIND_THOR_LIB_SUFFIX}
             PATH_SUFFIXES lib64 lib
             PATHS ${FIND_THOR_LIB_PATHS})

if (THOR_LIBRARY_DEBUG OR THOR_LIBRARY_RELEASE)
    # library found
    set(THOR_FOUND TRUE)

    # if both are found, set THOR_LIBRARY to contain both
    if (THOR_LIBRARY_DEBUG AND THOR_LIBRARY_RELEASE)
        set(THOR_LIBRARY debug     ${THOR_LIBRARY_DEBUG}
                         optimized ${THOR_LIBRARY_RELEASE})
    endif()

    # if only one debug/release variant is found, set the other to be equal to the found one
    if (THOR_LIBRARY_DEBUG AND NOT THOR_LIBRARY_RELEASE)
        # debug and not release
        set(THOR_LIBRARY_RELEASE ${THOR_LIBRARY_DEBUG})
        set(THOR_LIBRARY         ${THOR_LIBRARY_DEBUG})
    endif()
    if (THOR_LIBRARY_RELEASE AND NOT THOR_LIBRARY_DEBUG)
        # release and not debug
        set(THOR_LIBRARY_DEBUG ${THOR_LIBRARY_RELEASE})
        set(THOR_LIBRARY       ${THOR_LIBRARY_RELEASE})
    endif()
else()
    # library not found
    set(THOR_FOUND FALSE)
    set(THOR_LIBRARY "")
    set(FIND_THOR_MISSING "${FIND_THOR_MISSING} THOR_LIBRARY")
endif()

# mark as advanced
MARK_AS_ADVANCED(THOR_LIBRARY
                 THOR_LIBRARY_RELEASE
                 THOR_LIBRARY_DEBUG)

# handle errors
if(NOT THOR_VERSION_OK)
    # Thor version not ok
    set(FIND_THOR_ERROR "Thor found but version too low (requested: ${THOR_FIND_VERSION}, found: ${THOR_VERSION_MAJOR}.${THOR_VERSION_MINOR})")
    set(THOR_FOUND FALSE)
elseif(NOT THOR_FOUND)
    # include directory or library not found
    set(FIND_THOR_ERROR "Could NOT find Thor (missing: ${FIND_THOR_MISSING})")
endif()
if (NOT THOR_FOUND)
    if(THOR_FIND_REQUIRED)
        # fatal error
        message(FATAL_ERROR ${FIND_THOR_ERROR})
    elseif(NOT THOR_FIND_QUIETLY)
        # error but continue
        message("${FIND_THOR_ERROR}")
    endif()
endif()

# handle success
if(THOR_FOUND)
    message("Found Thor: ${THOR_INCLUDE_DIR}")
endif()
