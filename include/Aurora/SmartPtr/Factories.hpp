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
/// @brief Factory functions makeUnique(), makeCopied()

#ifndef AURORA_FACTORIES_HPP
#define AURORA_FACTORIES_HPP

#include <Aurora/Tools/Preprocessor.hpp>

#include <memory>	// std::unique_ptr
#include <utility>	// std::forward()


namespace aurora
{

// For documentation
#ifdef AURORA_DOXYGEN_SECTION

/// @addtogroup SmartPtr
/// @{

/// @brief Creates an object inside the unique pointer.
/// @param args Variable argument list, the single arguments are forwarded to T's constructor. Due to limited compiler support for
///  variadic templates, the number of arguments may not exceed 5.
/// @details Allows handy syntax analogous to std::make_shared, avoids explicit new operator. Example:
/// @code
/// auto str = aurora::makeUnique<std::string>(3, 'a'); // creates "aaa"
/// auto i = aurora::makeUnique<int>(37);               // creates 37
/// @endcode
template <typename T, typename... Args>
std::unique_ptr<T> makeUnique(Args... args);

/// @relates CopiedPtr
/// @brief Creates an object inside the copied pointer.
/// @param args Variable argument list, the single arguments are forwarded to T's constructor. Due to limited compiler support for
///  variadic templates, the number of arguments may not exceed 5.
/// @details Allows handy syntax analogous to std::make_shared, avoids explicit new operator. Example:
/// @code
/// auto str = aurora::makeCopied<std::string>(3, 'a'); // creates "aaa"
/// auto i = aurora::makeCopied<int>(37);               // creates 37
/// @endcode
template <typename T, typename... Args>
CopiedPtr<T> makeCopied(Args... args);

/// @}

// ---------------------------------------------------------------------------------------------------------------------------


#else  // AURORA_DOXYGEN_SECTION

template <typename T>
class CopiedPtr;

#define AURORA_DETAIL_TYPENAME(n) typename A ## n
#define AURORA_DETAIL_PARAMETER(n) A ## n && arg ## n
#define AURORA_DETAIL_FORWARD_ARG(n) std::forward<A ## n>(arg ## n)

#define AURORA_DETAIL_SMARTPTR_FACTORY(SmartPtr, factoryFunction, n)								\
	template <typename T AURORA_PP_COMMA_UNLESS_ZERO(n) AURORA_PP_LIST(n, AURORA_DETAIL_TYPENAME)>	\
	SmartPtr<T> factoryFunction(AURORA_PP_LIST(n, AURORA_DETAIL_PARAMETER))							\
	{																								\
		return SmartPtr<T>(new T( AURORA_PP_LIST(n, AURORA_DETAIL_FORWARD_ARG) ));					\
	}


// Actual function template overloads
// Note: I can't apply AURORA_PP_LIST here because nested macros with the same name are not expanded
AURORA_DETAIL_SMARTPTR_FACTORY(std::unique_ptr, makeUnique, 0)
AURORA_DETAIL_SMARTPTR_FACTORY(std::unique_ptr, makeUnique, 1)
AURORA_DETAIL_SMARTPTR_FACTORY(std::unique_ptr, makeUnique, 2)
AURORA_DETAIL_SMARTPTR_FACTORY(std::unique_ptr, makeUnique, 3)
AURORA_DETAIL_SMARTPTR_FACTORY(std::unique_ptr, makeUnique, 4)
AURORA_DETAIL_SMARTPTR_FACTORY(std::unique_ptr, makeUnique, 5)

AURORA_DETAIL_SMARTPTR_FACTORY(CopiedPtr, makeCopied, 0)
AURORA_DETAIL_SMARTPTR_FACTORY(CopiedPtr, makeCopied, 1)
AURORA_DETAIL_SMARTPTR_FACTORY(CopiedPtr, makeCopied, 2)
AURORA_DETAIL_SMARTPTR_FACTORY(CopiedPtr, makeCopied, 3)
AURORA_DETAIL_SMARTPTR_FACTORY(CopiedPtr, makeCopied, 4)
AURORA_DETAIL_SMARTPTR_FACTORY(CopiedPtr, makeCopied, 5)

#endif // AURORA_DOXYGEN_SECTION

} // namespace aurora

#endif // AURORA_FACTORIES_HPP
