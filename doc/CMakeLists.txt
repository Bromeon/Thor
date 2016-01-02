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

# Directory Thor/doc


find_package(Doxygen REQUIRED)

set(THOR_INPUT_DOXYFILE "${PROJECT_SOURCE_DIR}/doc/Doxyfile.txt")
set(THOR_OUTPUT_DOXYFILE "${PROJECT_BINARY_DIR}/doc/Doxyfile.txt")

# Copy doxyfile, replacing only @variables@
configure_file(${THOR_INPUT_DOXYFILE} ${THOR_OUTPUT_DOXYFILE} @ONLY)

# Copy other files for documentation
configure_file(${PROJECT_SOURCE_DIR}/doc/Doxygen.css ${PROJECT_BINARY_DIR}/doc/html/Doxygen.css COPYONLY)

# Add target for documentation, invokes Doxygen
add_custom_target(doc ALL
	COMMAND ${DOXYGEN_EXECUTABLE} "${PROJECT_BINARY_DIR}/doc/Doxyfile.txt"
	WORKING_DIRECTORY "${PROJECT_BINARY_DIR}")

# Install documentation to doc directory
install(DIRECTORY "${PROJECT_BINARY_DIR}/doc/html"
		DESTINATION "${THOR_DOC_INSTALL_DIR}/doc"
		COMPONENT doc)
