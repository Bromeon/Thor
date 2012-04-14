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

#ifndef AURORA_METAPROGRAMMING_HPP
#define AURORA_METAPROGRAMMING_HPP

#include <Aurora/Config.hpp>

#include <type_traits>


namespace aurora
{

/// @addtogroup Tools
/// @{
	
/// @brief Simple type wrapper, can be used for overload resolution.
///
template <typename T>
struct Type
{
};

/// @brief Simple integer wrapper, can be used for overload resolution.
///
template <int N>
struct Int
{
};


/// @brief SFINAE Enable If Macro for parameter lists
/// @details Usage:
/// @code
///  template <typename T>
///  void Function(T value
///      AURORA_ENABLE_IF(std::is_integral<T>::value));
/// @endcode
#define AURORA_ENABLE_IF(...)  , typename std::enable_if<__VA_ARGS__>::type* = nullptr


/// @brief Macro to ensure complete type
/// @details Usage:
/// @code
///  template <typename T>
///  void CheckedDelete(T* pointer)
///  {
///      AURORA_REQUIRE_COMPLETE_TYPE(T);
///      delete pointer;
///  }
/// @endcode
/// @hideinitializer
#define AURORA_REQUIRE_COMPLETE_TYPE(Type) typedef char auroraRequireCompleteType[(sizeof(Type))]

/// @}

// ---------------------------------------------------------------------------------------------------------------------------
	

namespace detail
{

	// Removes reference and pointer attribute from type
	template <typename T>
	struct RemoveIndirection
	{
		typedef typename std::remove_pointer<
			typename std::remove_reference<T>::type
		>::type type;
	};


	// Removes reference, pointer and const attribute from type
	template <typename T>
	struct RawType
	{
		typedef typename std::remove_const<
			typename RemoveIndirection<T>::type
		>::type type;
	};


	// Adjusts New such that it has the same const, pointer, reference attributes as Origin
	template <typename Origin, typename New>
	struct Replicate
	{
		typedef typename RawType<New>::type																			raw;

		typedef typename std::conditional<
			std::is_const<typename RemoveIndirection<Origin>::type>::value,
			const raw,
			raw>::type																								c_qualified;

		typedef typename std::conditional<std::is_pointer<Origin>::value, c_qualified*, c_qualified>::type			cp_qualified;
		typedef typename std::conditional<std::is_reference<Origin>::value, cp_qualified&, cp_qualified>::type		cpr_qualified;
		typedef typename std::conditional<std::is_const<Origin>::value, const cpr_qualified, cpr_qualified>::type	type;
	};

	// Human-readable form
	#define AURORA_REPLICATE(Origin, New) typename aurora::detail::Replicate<Origin, New>::type
	
} // namespace detail
} // namespace aurora

#endif // AURORA_METAPROGRAMMING_HPP
