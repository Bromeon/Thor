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
/// @brief Class template aurora::SingleDispatcher

#ifndef AURORA_SINGLEDISPATCHER_HPP
#define AURORA_SINGLEDISPATCHER_HPP

#include <Aurora/Dispatch/Rtti.hpp>
#include <Aurora/Tools/NonCopyable.hpp>
#include <Aurora/Tools/Exceptions.hpp>
#include <Aurora/Meta/Templates.hpp>
#include <Aurora/Config.hpp>

#include <unordered_map>
#include <functional>
#include <algorithm>
#include <cassert>


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
/// @tparam Traits Traits class to customize the usage of the dispatcher. To define your own traits, you can (but don't have to)
///  inherit the class aurora::DispatchTraits<K>, where K is your key. In general, the @a Traits class must contain the following members:
/// @code 
/// struct Traits
/// {
///	    // The type that is used to differentiate objects. For RTTI class hierarchies, std::type_index is a good choice 
///	    // -- but you're free to choose anything, such as an enum or a string. The requirements are that Key can be used 
///	    // as a key in std::unordered_map, i.e. it must support a std::hash<Key> specialization and operator==.
///	    typedef K Key;
///	    
///	    // A function that returns the corresponding key (such as std::type_index) from a type identifier
///	    // (such as aurora::Type<T>). Often, key and type identifier are the same.
///	    static Key keyFromId(Id id);
///	    
///	    // Given a function argument base, this static function extracts the key from it. B corresponds to the template parameter
///	    // specified at SingleDispatcher, that is, it is a reference or pointer.
///	    static Key keyFromBase(B base);
///	    
///	    // trampoline1() takes a function that is passed to SingleDispatcher::bind() and modifies it in order to fit the common
///	    // R(B) signature. For example, this is the place to insert downcasts.
///	    // The first template parameter Id is required, as it will be explicitly specified when trampoline1() is called.
///	     template <typename Id, typename Fn>
///	     static std::function<R(B)> trampoline1(Fn f);
///	    
///	    // Optional function that returns a string representation of @a key for debugging.
///	    static const char* name(Key k);
/// };
/// @endcode
///
/// Usage example:
/// @code
/// // Example class hierarchy
/// class Base { public: virtual ~Base() {} };
/// class Derived1 : public Base {};
/// class Derived2 : public Base {};
///
/// // Free functions for the derived types
/// void func1(Derived1* d);
/// void func2(Derived2* d);
///
/// // Create dispatcher and register functions
/// aurora::SingleDispatcher<Base*> dispatcher;
/// dispatcher.bind(aurora::Type<Derived1>(), &func1);
/// dispatcher.bind(aurora::Type<Derived2>(), &func2);
///
/// // Invoke functions on base class pointer
/// Base* ptr = new Derived1;
/// dispatcher.call(ptr); // Invokes void func1(Derived1* d);
/// delete ptr;
/// @endcode
template <class B, typename R = void, class Traits = RttiDispatchTraits<B, R>>
class SingleDispatcher : private NonCopyable
{	 
	// ---------------------------------------------------------------------------------------------------------------------------
	// Static assertions

	// Make sure that B is either T* or T&
	static_assert(std::is_pointer<B>::value || std::is_lvalue_reference<B>::value,
		"Template argument B must be a pointer or reference.");

	 
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
									SingleDispatcher();

		/// @brief Registers a function bound to a specific key.
		/// @tparam Id %Type that identifies the class. By default, this is aurora::Type<D>, where D is the derived class.
		///  Can be deduced from the argument.
		/// @tparam Fn %Type of the function. Can be deduced from the argument.
		/// @param identifier Value that identifies the object. The key, which is mapped to the function, is computed from the
		///  identifier through Traits::keyFromId(identifier).
		/// @param functionObject Function to register and associate with the given identifier.
		template <typename Id, typename Fn>
		void						bind(Id identifier, Fn function);

		/// @brief Dispatches the key of @a arg and invokes the corresponding function.
		/// @details Traits::keyFromBase(arg) is invoked to determine the key of the passed argument. The function bound to that
		///  key is then looked up in the map and invoked.
		/// @param arg Function argument as a reference or pointer.
		/// @return The return value of the dispatched function, if any.
		/// @throw FunctionCallException when no corresponding function is found.
		R							call(B arg) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		typedef typename Traits::Key					Key;
		typedef std::function<R(B)>						BaseFunction;
		typedef std::unordered_map<Key, BaseFunction>	FnMap;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		FnMap						mMap;
};

/// @}

} // namespace aurora

#include <Aurora/Dispatch/Detail/SingleDispatcher.inl>
#endif // AURORA_SINGLEDISPATCHER_HPP
