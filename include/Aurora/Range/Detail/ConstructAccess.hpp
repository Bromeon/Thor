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

#ifndef AURORA_CONSTRUCTACCESS_HPP
#define AURORA_CONSTRUCTACCESS_HPP

#include <utility>


namespace aurora
{
namespace detail
{

	// TODO: Use variadic templates after some time (all most recent compilers support them)

	// Has access to internals of a class through friend
	// Can be used to invoke private constructors
	struct ConstructAccess
	{
		template <typename Class, typename A1>
		static Class construct(A1&& arg1)
		{
			return Class(std::forward<A1>(arg1));
		}

		template <typename Class, typename A1, typename A2>
		static Class construct(A1&& arg1, A2&& arg2)
		{
			return Class(std::forward<A1>(arg1), std::forward<A2>(arg2));
		}
	};

	// Helper function for more beautiful syntax
	template <typename Class, typename A1>
	Class constructPrivate(A1&& arg1)
	{
		return ConstructAccess::construct<Class>(std::forward<A1>(arg1));
	}

	template <typename Class, typename A1, typename A2>
	Class constructPrivate(A1&& arg1, A2&& arg2)
	{
		return ConstructAccess::construct<Class>(std::forward<A1>(arg1), std::forward<A2>(arg2));
	}

} // namespace detail
} // namespace aurora

#endif // AURORA_CONSTRUCTACCESS_HPP
