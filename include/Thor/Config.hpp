/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2015 Jan Haller
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////

/// @file
/// @brief Configuration header of the library

#ifndef THOR_CONFIG_HPP
#define THOR_CONFIG_HPP

#include <Aurora/Config.hpp>


// Define DLL import/export macros (only Windows, and only dynamic configuration)
#if (defined(_WIN32) || defined(__WIN32__)) && !defined(SFML_STATIC)

	// Export dynamic link library (from DLL side)
	#ifdef THOR_EXPORTS
		#define THOR_API __declspec(dllexport)

	// Import dynamic link library (from client side)
	#else
		#define THOR_API __declspec(dllimport)

	#endif // THOR_EXPORTS

	// Disable annoying MSVC++ warning
	#ifdef _MSC_VER
		#pragma warning(disable: 4251)
	#endif // _MSC_VER


// Other platforms don't have DLLs
#else
	#define THOR_API

#endif


// Version of the library
#define THOR_VERSION_MAJOR 2
#define THOR_VERSION_MINOR 0


#endif // THOR_CONFIG_HPP
