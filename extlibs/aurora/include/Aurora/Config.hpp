/////////////////////////////////////////////////////////////////////////////////
//
// Aurora C++ Library
// Copyright (c) 2012 Jan Haller
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

#ifndef AURORA_CONFIG_HPP
#define AURORA_CONFIG_HPP

// Compiler-dependent include path for TR1 headers
// If your standard library isn't covered by this macro, please inform me.
#ifdef _MSC_VER
	#define AURORA_TR1_HEADER(Header) <Header>
#else
	#define AURORA_TR1_HEADER(Header) <tr1/Header>
#endif // _MSC_VER


// C++11 support - define AURORA_CPP11 to 0 in order to disable C++11 features.
#if _MSC_VER >= 1600 || __GNUC__ >= 4 && __GNUC_MINOR__ >= 5
	#define AURORA_HAS_CPP11
#endif

// Version of the library
#define AURORA_VERSION_MAJOR 1
#define AURORA_VERSION_MINOR 0


// Mislead doxygen to keep documentation clean from internals
#define AURORA_FAKE_DOC(real, fake) real

#endif // AURORA_CONFIG_HPP
