#################################################################################
##
## Thor C++ Library
## Copyright (c) 2011-2016 Jan Haller
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
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/extlibs/aurora/include)

# Predefined configuration options
set(THOR_SHARED_LIBS TRUE CACHE BOOL "Build shared libraries (use shared SFML librares)")
set(THOR_BUILD_EXAMPLES FALSE CACHE BOOL "Build example projects")
set(THOR_BUILD_DOC FALSE CACHE BOOL "Create HTML documentation (requires Doxygen)")

# Windows: Choose to link CRT libraries statically or dynamically
if(WIN32)
	set(THOR_STATIC_STD_LIBS FALSE CACHE BOOL "Use statically linked standard/runtime libraries? This option must match the one used for SFML.")

	# Determine whether we're dealing with a TDM compiler or not
	if(CMAKE_COMPILER_IS_GNUCXX)
		execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "--version" OUTPUT_VARIABLE GCC_COMPILER_VERSION)
		string(REGEX MATCHALL ".*(tdm[64]*-[1-9]).*" COMPILER_GCC_TDM "${GCC_COMPILER_VERSION}")
	endif()

	# Modify compiler flags globally
	if(THOR_STATIC_STD_LIBS)
		if(THOR_SHARED_LIBS)
			message("\n-> THOR_STATIC_STD_LIBS and THOR_SHARED_LIBS are not compatible.")
			message("-> They lead to multiple runtime environments which result in undefined behavior.\n")
		elseif(MSVC)
			foreach(flag CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
				if(${flag} MATCHES "/MD")
					string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
				endif()
			endforeach()
		elseif(CMAKE_COMPILER_IS_GNUCXX AND NOT COMPILER_GCC_TDM)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
		endif()
	elseif(COMPILER_GCC_TDM)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared-libgcc -shared-libstdc++")
	endif()
endif()


# Compiler-specific flags and definitions
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(THOR_CLANG_STDLIB "" CACHE STRING "Chose standard library (libstdc++ or libc++) or leave empty for default")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wno-switch -Wno-logical-op-parentheses")
	if(NOT THOR_CLANG_STDLIB STREQUAL "")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=${THOR_CLANG_STDLIB}")
	endif()

	add_definitions(-DTHOR_USE_STD_RANDOMENGINE) # Workaround for libc++ bug
endif()


# Variable for install directory
if(UNIX)
	set(THOR_EXAMPLE_INSTALL_DIR share/Thor)
	set(THOR_DOC_INSTALL_DIR share/doc/Thor)
else()
	set(THOR_EXAMPLE_INSTALL_DIR .)
	set(THOR_DOC_INSTALL_DIR .)
endif()


# Add directory containing FindSFML.cmake to module path
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH}")


# Find SFML
if(NOT THOR_SHARED_LIBS)
	set(SFML_STATIC_LIBRARIES TRUE)
endif()
find_package(SFML 2 COMPONENTS audio graphics window system)

if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
else()
	set(SFML_ROOT "" CACHE PATH "SFML top-level directory")
	message("\n-> SFML directory not found. Set SFML_ROOT to SFML's top-level path (containing \"include\" and \"lib\" directories).")
	message("-> Make sure the SFML libraries with the same configuration (Release/Debug, Static/Dynamic) exist.\n")
endif()


# Macro to link SFML
macro(thor_link_sfml THOR_TARGET)
	target_link_libraries(${THOR_TARGET} ${SFML_LIBRARIES})
endmacro()

macro(thor_link_sfml_dependencies THOR_TARGET)
	target_link_libraries(${THOR_TARGET} ${SFML_DEPENDENCIES})
endmacro()

# Macro to link Thor (for examples)
macro(thor_link_thor THOR_TARGET)
	target_link_libraries(${THOR_TARGET} thor)
endmacro()


# Enable IDE folders and set them for predefined CMake projects
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")


# Documentation
if(THOR_BUILD_DOC)
	add_subdirectory(doc)
endif()


# C++ source code
add_subdirectory(src)

if(THOR_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()


# Install include directory and license files
install(DIRECTORY include
		DESTINATION .)

install(FILES cmake/Modules/FindThor.cmake
		DESTINATION ./cmake/Modules/)

install(FILES License.txt
		RENAME LicenseThor.txt
		DESTINATION ${THOR_DOC_INSTALL_DIR})

install(FILES extlibs/aurora/License.txt
		RENAME LicenseAurora.txt
		DESTINATION ${THOR_DOC_INSTALL_DIR})

install(DIRECTORY extlibs/aurora/include
		DESTINATION .)
