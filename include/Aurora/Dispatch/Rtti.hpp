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

#ifndef AURORA_RTTI_HPP
#define AURORA_RTTI_HPP

/// @file
/// @brief Manual enhancements of Runtime Type Information

#include <Aurora/Dispatch/Detail/RttiImpl.hpp>
#include <Aurora/Meta/Templates.hpp>
#include <Aurora/Config.hpp>

#include <typeindex>
#include <functional>


namespace aurora
{

/// @addtogroup Dispatch
/// @{

/// @brief Register runtime type information for a base class
/// @details You need this for the dynamic dispatchers to cope with derived-to-base conversions.
/// Since C++ offers no possibility to recognize the base classes of a class at runtime, this must be done manually.
/// Note that multiple inheritance is not supported.
/// @n@n Example class hierarchy:
/// @code
/// class Vehicle { public: virtual ~Vehicle(); }
/// class Aircraft : public Vehicle {};
/// class Watercraft : public Vehicle {};
/// class Ship : public Watercraft {};
/// class Submarine : public Watercraft {};
/// @endcode
/// Register classes as follows (once inside a function):
/// @code
/// AURORA_RTTI_BASE(Vehicle)
///   AURORA_RTTI_DERIVED(Aircraft)
///   AURORA_RTTI_DERIVED(Watercraft);
/// AURORA_RTTI_BASE(Watercraft)
///   AURORA_RTTI_DERIVED(Ship)
///   AURORA_RTTI_DERIVED(Submarine);
/// @endcode
/// @hideinitializer
#define AURORA_RTTI_BASE(BaseClass)			::aurora::detail::RttiClass(typeid(BaseClass))

/// @brief Register runtime type information for a derived class
/// @see AURORA_RTTI_BASE
/// @hideinitializer
#define AURORA_RTTI_DERIVED(DerivedClass)	.derived(typeid(DerivedClass))


/// @brief Helper base class to implement custom traits for dispatchers
/// @details This class provides some default type definitions and static member functions. Inherit it
///  to customize and extend the traits -- for example, you have to define keyFromBase().
/// @tparam K The key type that identifies the objects to dispatch.
/// @see SingleDispatcher
template <typename K>
struct DispatchTraits
{
	/// @brief Key to differentiate objects
	/// 
	typedef K Key;

	/// @brief Maps a key to itself (assuming key and type identifier are the same)
	///
	static Key keyFromId(Key k)
	{
		return k;
	}

	/// @brief Maps a function to itself (no trampoline needed)
	/// 
	template <typename UnusedId, typename Fn>
	static Fn trampoline1(Fn f)
	{
		return f;
	}

	/// @brief Maps a function to itself (no trampoline needed)
	/// 
	template <typename UnusedId1, typename UnusedId2, typename Fn>
	static Fn trampoline2(Fn f)
	{
		return f;
	}

	/// @brief Returns a string representation of the key, for debugging
	/// 
	static const char* name(Key k)
	{
		return "unknown";
	}
};

/// @brief Identifies a class using RTTI.
/// @details Default key for SingleDispatcher and DoubleDispatcher. With it, classes are identified using the compiler's
///  RTTI capabilities (in particular, the @a typeid operator).
template <class B, typename R>
struct RttiDispatchTraits
{
	static_assert(std::is_polymorphic<typename std::remove_pointer<typename std::remove_reference<B>::type>::type>::value, 
		"B must be a pointer or reference to a polymorphic base class.");

	/// @brief Key type.
	/// 
	typedef std::type_index Key;

	/// @brief Function that takes an object to identify and returns the corresponding std::type_index object.
	/// 
	static Key keyFromBase(B m)
	{
		return detail::derefTypeid(m);
	}
	
	/// @brief Function that takes static type information and returns a type-erased std::type_index object.
	/// 
	template <typename T>
	static Key keyFromId(Type<T> id)
	{
		return typeid(T);
	}

	/// @brief Wraps a function such that the argument is downcast before being passed
	/// 
	template <typename Id, typename Fn>
	static std::function<R(B)> trampoline1(Fn f)
	{
		return [f] (B arg) mutable -> R
		{
			typedef AURORA_REPLICATE(B, typename Id::type) Derived;
			return f(static_cast<Derived>(arg));
		};
	}

	/// @brief Wraps a function such that both arguments are downcast before being passed
	/// 
	template <typename Id1, typename Id2, typename Fn>
	static std::function<R(B, B)> trampoline2(Fn f)
	{
		return [f] (B arg1, B arg2) mutable -> R
		{
			typedef AURORA_REPLICATE(B, typename Id1::type) Derived1;
			typedef AURORA_REPLICATE(B, typename Id2::type) Derived2;
			return f(static_cast<Derived1>(arg1), static_cast<Derived2>(arg2));
		};
	}

	/// @brief Returns a string representation of the key, for debugging
	/// 
	static const char* name(Key k)
	{
		return k.name();
	}
};

/// @}

} // namespace aurora

#endif // AURORA_RTTI_HPP
