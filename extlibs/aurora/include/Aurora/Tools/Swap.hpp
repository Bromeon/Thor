/////////////////////////////////////////////////////////////////////////////////
//
// Aurora C++ Library
// Copyright (c) 2012-2015 Jan Haller
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
/// @brief Macro to implement global swap() overload

#ifndef AURORA_SWAP_HPP
#define AURORA_SWAP_HPP


/// @addtogroup Tools
/// @{

/// @brief Macro to implement a global overload of swap(lhs, rhs) to allow argument-dependent lookup.
/// @details Accesses the memberfunction void Class::Swap(Class&).
#define AURORA_GLOBAL_SWAP(Class)			\
inline void swap(Class& lhs, Class& rhs)	\
{											\
	lhs.swap(rhs);							\
}

/// @}

#endif // AURORA_SWAP_HPP
