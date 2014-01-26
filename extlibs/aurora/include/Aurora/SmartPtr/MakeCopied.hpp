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
/// @brief Factory function aurora::makeCopied()

#ifndef AURORA_MAKECOPIED_HPP
#define AURORA_MAKECOPIED_HPP

#include <utility>	// std::forward()


namespace aurora
{

// Forward declarations
template <typename T>
class CopiedPtr;

namespace detail
{
	struct EmplaceTag;
}

// For documentation and modern compilers
#if defined(AURORA_DOXYGEN_SECTION) || defined(AURORA_HAS_VARIADIC_TEMPLATES)

/// @addtogroup SmartPtr
/// @{

/// @relates CopiedPtr
/// @brief Emplaces an object directly inside the copied pointer.
/// @param args Variable argument list, the single arguments are forwarded to T's constructor. If your compiler does not
/// support variadic templates, the number of arguments must be smaller than @ref AURORA_PP_LIMIT.
/// @details This function has several advantages over the regular CopiedPtr(U*) constructor:
/// * The new operator is encapsulated, making code exception-safe (in cases where multiple temporary smart pointers are constructed).
/// * This function is considerably more efficient and requires less memory, because pointee and cloner/deleter can be stored together.
/// * You avoid mentioning the pointee type twice.
/// 
/// Example:
/// @code
/// auto ptr = aurora::makeCopied<MyClass>(arg1, arg2); // instead of
/// aurora::CopiedPtr<MyClass> ptr(new MyClass(arg1, arg2));
/// @endcode
template <typename T, typename... Args>
CopiedPtr<T> makeCopied(Args&&... args)
{
	return CopiedPtr<T>(detail::EmplaceTag(), std::forward<Args>(args)...);
}

/// @}

// ---------------------------------------------------------------------------------------------------------------------------


// Unoptimized fallback for compilers that don't support variadic templates, emulated by preprocessor metaprogramming
#else  // defined(AURORA_DOXYGEN_SECTION) || defined(AURORA_HAS_VARIADIC_TEMPLATES)

#include <Aurora/SmartPtr/Detail/Factories.hpp>

// Define metafunction to generate overloads for aurora::CopiedPtr
#define AURORA_DETAIL_COPIEDPTR_FACTORY(n) AURORA_DETAIL_SMARTPTR_FACTORY(CopiedPtr, makeCopied, n)

// Generate code
AURORA_PP_ENUMERATE(AURORA_PP_LIMIT, AURORA_DETAIL_COPIEDPTR_FACTORY)

#endif // defined(AURORA_DOXYGEN_SECTION) || defined(AURORA_HAS_VARIADIC_TEMPLATES)

} // namespace aurora

#endif // AURORA_MAKECOPIED_HPP
