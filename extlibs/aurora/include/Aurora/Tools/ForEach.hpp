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
/// @brief Macros to simplify container iteration

#ifndef AURORA_FOREACH_HPP
#define AURORA_FOREACH_HPP

/// @addtogroup Tools
/// @{

/// @brief Macro to iterate forward on a mutable sequence.
/// @hideinitializer
#define AURORA_ITR_FOREACH(ContainerType, containerVar, iteratorVar)			\
	for (ContainerType::iterator iteratorVar = containerVar.begin(),			\
	aurForeachEnd = containerVar.end();											\
	iteratorVar != aurForeachEnd; ++iteratorVar)

/// @brief Macro to iterate forward on a constant sequence.
/// @hideinitializer
#define AURORA_CITR_FOREACH(ContainerType, containerVar, iteratorVar)			\
	for (ContainerType::const_iterator iteratorVar = containerVar.begin(),		\
	aurForeachEnd = containerVar.end();											\
	iteratorVar != aurForeachEnd; ++iteratorVar)

/// @}

#endif // AURORA_FOREACH_HPP
