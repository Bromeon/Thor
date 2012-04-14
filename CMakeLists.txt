#################################################################################
##
## Thor C++ Library
## Copyright (c) 2011-2012 Jan Haller
## 
## This software is provided 'as-is', without any express or implied
## warranty. In no event will the authors be held liable for any damages
## arising from the use of this software.
## 
## Permission is granted to anyone to use this software for any purpose,
## including commercial applications, and to alter it and redistribute it
## freely, subject to the following restrictions:
## 
## 1. The origin of this software must not be misrepresented; you must not
##    claim that you wrote the original software. If you use this software
##    in a product, an acknowledgment in the product documentation would be
##    appreciated but is not required.
## 
## 2. Altered source versions must be plainly marked as such, and must not be
##    misrepresented as being the original software.
## 
## 3. This notice may not be removed or altered from any source distribution.
##
#################################################################################

# Directory Thor


cmake_minimum_required(VERSION 2.8)

# Specify default build type if none provided (before project() command)
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# For generators with multiple configurations (like VS), only allow Debug and Release
if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited configurations" FORCE)
	mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
endif()

project(thor)


# Global preprocessor and include options
add_definitions(-DTHOR_EXPORTS)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/extlibs/aurora/include)

# Predefined configuration options
set(THOR_SHARED_LIBS TRUE CACHE BOOL "Build shared libraries (use shared SFML librares)")
set(THOR_BUILD_EXAMPLES FALSE CACHE BOOL "Build example projects")

# Windows: Choose to link CRT libraries statically or dynamically
if(WIN32)
    set(THOR_STATIC_STD_LIBS FALSE CACHE BOOL "Use statically linked standard libraries? This option must match the one used for SFML.")
    
    # Modify compiler flags globally
	if(THOR_STATIC_STD_LIBS)
		if(MSVC)
			foreach(flag CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
				if(${flag} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
				endif()
			endforeach()
		elseif(CMAKE_COMPILER_IS_GNUCXX)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
		endif()
	endif()
endif()


# C++11 support
add_definitions(-DAURORA_HAS_CPP11)
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()



# Variable for install directory
if(UNIX)
	set(THOR_EXAMPLE_INSTALL_DIR share/Thor)
	set(THOR_DOC_INSTALL_DIR share/doc/Thor)
else()
	set(THOR_EXAMPLE_INSTALL_DIR .)
	set(THOR_DOC_INSTALL_DIR .)
endif()


# Find SFML
if(NOT THOR_SHARED_LIBS)
	set(SFML_STATIC_LIBRARIES TRUE)
endif()
find_package(SFML 2 COMPONENTS audio graphics window system)

if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
else()
	set(SFMLDIR "" CACHE PATH "SFML top-level directory")
	message(FATAL_ERROR "SFML directory not found. Set SFMLDIR to SFML's top-level path (containing \"include\" and \"lib\" directories).")
endif()


# Macro for easy linkage of SFML
macro(thor_link_sfml THOR_TARGET)
	target_link_libraries(${THOR_TARGET} ${SFML_LIBRARIES})
endmacro()


# Macro for easy linkage of Thor (for examples)
macro(thor_link_thor THOR_TARGET)
	target_link_libraries(${THOR_TARGET} thor)
endmacro()


# C++ source code
add_subdirectory(src)

if(THOR_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()


# Install include directory and license file
install(DIRECTORY include
		DESTINATION .
		REGEX "\\.svn|_svn" EXCLUDE)

install(FILES license.txt
		DESTINATION ${THOR_DOC_INSTALL_DIR})

install(DIRECTORY extlibs/aurora/include
		DESTINATION .
		REGEX "\\.svn|_svn" EXCLUDE)