/////////////////////////////////////////////////////////////////////////////////
//
// Aurora C++ Library
// Copyright (c) 2012-2014 Jan Haller
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
/// @brief Iterator category types

#ifndef AURORA_ITERATORCATEGORIES_HPP
#define AURORA_ITERATORCATEGORIES_HPP

#include <iterator>


namespace aurora
{

/// @addtogroup Range
/// @{

/// @brief Namespace for iterator traversal categories
/// 
namespace Traversal
{

	/// @brief Forward traversal category
	/// 
	typedef std::forward_iterator_tag		Forward;

	/// @brief Bidirectional traversal category
	/// 
	typedef std::bidirectional_iterator_tag	Bidirectional;

	/// @brief Random access category
	/// 
	typedef std::random_access_iterator_tag	RandomAccess;

} // namespace Traversal

/// @}

} // namespace aurora

#endif // AURORA_ITERATORCATEGORIES_HPP
