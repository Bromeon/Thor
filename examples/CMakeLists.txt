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

# Directory Thor/examples


# Macro to build and install examples
macro(thor_example THOR_EXAMPLE_NAME)
	project(${THOR_EXAMPLE_NAME})

	add_executable(${THOR_EXAMPLE_NAME} "${THOR_EXAMPLE_NAME}.cpp")
	if(NOT THOR_SHARED_LIBS)
		add_definitions(-DSFML_STATIC)
	endif()

	thor_link_thor(${THOR_EXAMPLE_NAME})
	thor_link_sfml_opengl(${THOR_EXAMPLE_NAME})

	install(TARGETS ${THOR_EXAMPLE_NAME}
			RUNTIME DESTINATION ${THOR_EXAMPLE_INSTALL_DIR}/examples)

	install (DIRECTORY .
			 DESTINATION ${THOR_EXAMPLE_INSTALL_DIR}/examples
			 REGEX "\\.svn|_svn" EXCLUDE
			 PATTERN "CMakeLists.txt" EXCLUDE)
endmacro()


thor_example(Action)
thor_example(Animation)
thor_example(Multimedia)
thor_example(Particles)
thor_example(Resources)
thor_example(Time)
thor_example(Triangulation)
thor_example(UserEvents)
