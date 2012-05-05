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
/// @brief Class template aurora::SingleDispatcher

#ifndef AURORA_SINGLEDISPATCHER_HPP
#define AURORA_SINGLEDISPATCHER_HPP

#include <Aurora/Dispatch/Rtti.hpp>
#include <Aurora/Dispatch/Detail/UnaryFunction.hpp>
#include <Aurora/Dispatch/Detail/AssociativeHelpers.hpp>
#include <Aurora/SmartPtr/CopiedPtr.hpp>
#include <Aurora/Tools/NonCopyable.hpp>
#include <Aurora/Tools/Exceptions.hpp>
#include <Aurora/Tools/ForEach.hpp>
#include <Aurora/Tools/Metaprogramming.hpp>
#include <Aurora/Config.hpp>

#include <vector>
#include <algorithm>
#include <cassert>
#include <typeindex>


namespace aurora
{

/// @addtogroup Dispatch
/// @{

/// @brief Class that is able to perform dynamic dispatch on multiple functions with one parameter.
/// @details Sometimes you encounter the situation where you need to implement polymorphic behavior, but you cannot
///  or don't want to add a virtual function to an existing class hierarchy. Here comes dynamic dispatch into play:
///  You define free functions, which can be treated by the dispatcher like virtual functions.
/// @tparam B Reference or pointer to polymorphic class, which is the base class of every dispatched function's
///  parameter type. When @a B is a pointer, the arguments of the dispatched functions shall be pointers too (the
///  same applies to references).
///  If @a B is a pointer or reference to const, the dispatched functions cannot modify their arguments. In this case,
///  the dispatched functions shall have arguments of type pointer or reference to const, too.
/// @tparam R Return type of the dispatched functions.
template <class B, typename R = void>
class SingleDispatcher : private NonCopyable
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Static assertions

	// Make sure that B is either T* or T&, where T is a polymorphic base class (containing virtual functions).
	static_assert(
		std::is_pointer<B>::value && std::is_polymorphic< typename std::remove_pointer<B>::type >::value
	 || std::is_lvalue_reference<B>::value && std::is_polymorphic< typename std::remove_reference<B>::type >::value,
		"Template argument B must be a pointer or reference to a polymorphic base class.");

	 
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		/// @param supportDerivedToBase Specifies whether derived-to-base conversions are supported. If no function for a derived
		///  class is found, the dispatcher will look for functions taking base class parameters and upcast the arguments, if
		///  possible. You need to register the class hierarchy using the macros in Rtti.hpp. On average, calls with
		///  derived-to-base conversions are as fast as direct matches, but this feature requires a small memory overhead.
		explicit					SingleDispatcher(bool supportDerivedToBase = false);

		/// @brief Registers a global function.
		/// @tparam D Type of the derived class. Must be explicitly specified.
		/// @param globalFunction Pointer to function to register.
		/// @details Note that # is a placeholder for either & or *. The type D# has the same attributes as B
		/// (pointer, reference, const-qualification): For instance, <b>B=const Base&</b> implies <b>D#=const Derived1&</b>.
		/// @n Example (class hierarchy and dispatcher declaration missing):
		/// @code 
		/// // Overloaded global or namespace-level functions
		/// void func(Derived1&);
		/// void func(Derived2&);
		///
		/// // Register functions
		/// dispatcher.add<Derived1>(&func);
		/// dispatcher.add<Derived2>(&func);
		/// @endcode
		/// @pre A function taking an argument of dynamic type D is not registered yet.
		template <class D>
		void						add(R (*globalFunction)( AURORA_REPLICATE(B,D) ));

		/// @brief Registers a member function.
		/// @tparam D Type of the derived class. Must be explicitly specified.
		/// @tparam C Class that holds the member function.
		/// @param memberFunction Pointer to member function to register.
		/// @param object Reference to the object on which the member function is invoked.
		/// @details Note that # is a placeholder for either & or *. The type D# has the same attributes as B
		/// (pointer, reference, const-qualification): For instance, <b>B=const Base&</b> implies <b>D#=const Derived1&</b>.
		/// @n Example (class hierarchy and dispatcher declaration missing):
		/// @code 
		/// // Class with member functions
		/// struct MyClass
		/// {
		///     void memFunc(Derived1&);
		///     void memFunc(Derived2&);
		/// } obj;
		///
		/// // Register overloaded functions
		/// dispatcher.add<Derived1>(&MyClass::memFunc, obj);
		/// dispatcher.add<Derived2>(&MyClass::memFunc, obj);
		/// @endcode
		/// @pre A function taking an argument of dynamic type D is not registered yet.
		template <class D, class C>
		void						add(R (C::*memberFunction)( AURORA_REPLICATE(B,D) ), C& object);

		/// @brief Registers a function object.
		/// @tparam D Type of the derived class. Must be explicitly specified.
		/// @tparam Fn Type of the function object. Can be deduced from the argument.
		/// @param functionObject Functor to register. 
		/// @details Incomplete example using a function object (you can also have separate functors for each function):
		/// @code
		/// // Class for function objects
		/// struct Functor
		/// {
		/// 	void operator() (Derived1&);
		/// 	void operator() (Derived2&);
		/// };
		///
		/// // Register functor
		/// dispatcher.add<Derived1>(Functor());
		/// dispatcher.add<Derived2>(Functor());
		/// @endcode
		/// @pre A function taking an argument of dynamic type D is not registered yet.
		template <class D, typename Fn>
		void						add(const Fn& functionObject);

		/// @brief Dispatches the dynamic type of @a arg and invokes the corresponding function.
		/// @details Note that the argument's dynamic type must match @b exactly with the registered type, unless you enabled
		///  derived-to-base conversions in the constructor and specified the class hierarchy. In the latter case, the function
		///  with the best match is chosen for overload resolution.
		/// @param arg Function argument as a reference or pointer to the base class.
		/// @return The return value of the dispatched function, if any.
		/// @throw FunctionCallException when no corresponding function is found.
		R							call(B arg) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		typedef std::type_index									Key;
		typedef CopiedPtr< detail::UnaryFunctionBase<B, R> >	Value;
		typedef detail::KeyValuePair<Key, Value>				Pair;
		typedef std::vector<Pair>								FnMap;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		// Registers the type-id key with its associated function value
		void							registerFunction(FnMap& fnMap, std::type_index key, Value value) const;

		// Finds the key in the map. Returns end() if not found.
		typename FnMap::const_iterator	findFunction(std::type_index key, bool useCache = false) const;

		// Make sure the cached map is updated
		void							ensureCacheUpdate() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		FnMap						mMap;
		mutable FnMap				mCachedMap;
		mutable bool				mNeedsCacheUpdate;
		bool						mDerivedToBase;
};

/// @class SingleDispatcher
/// @code
/// // Example class hierarchy
/// class Base { public: virtual ~Base() {} };
/// class Derived1 : public Base {};
/// class Derived2 : public Base {};
///
/// // Free functions for the derived types
/// void func(Derived1* d);
/// void func(Derived2* d);
///
/// // Create dispatcher and register functions
/// aurora::SingleDispatcher<Base*> dispatcher;
/// dispatcher.add<Derived1>(&func);
/// dispatcher.add<Derived2>(&func);
///
/// // Invoke functions on base class pointer
/// Base* ptr = new Derived1;
/// dispatcher.Call(ptr); // Invokes void func(Derived1* d);
/// delete ptr;
/// @endcode

/// @}

} // namespace aurora

#include <Aurora/Dispatch/Detail/SingleDispatcher.inl>
#endif // AURORA_SINGLEDISPATCHER_HPP
