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

#ifndef AURORA_UNARYFUNCTION_HPP
#define AURORA_UNARYFUNCTION_HPP


namespace aurora
{
namespace detail
{

	// Type-erased base class for caller interface
	// B: Base class of dispatched hierarchy
	// R: Function return type
	template <class B, typename R>
	struct UnaryFunctionBase
	{
		// Calls the function corresponding to ref's dynamic type
		virtual R call(B arg) = 0;

		// Virtual destructor
		virtual ~UnaryFunctionBase() {}
	};


	// Concrete caller class for global functions and function objects (like std::function)
	// B: Base class of dispatched hierarchy
	// D: Derived class of dispatched hierarchy
	// Fn: Function type
	template <class B, class D, typename R, typename Fn = R(*)(D)>
	struct UnaryGlobalFunction : UnaryFunctionBase<B, R>
	{
		// Creates a caller 
		explicit UnaryGlobalFunction(const Fn& fn)
		: fn(fn)
		{
		}

		// Calls the function corresponding to arg's dynamic type
		virtual R call(B arg)
		{
			// Drawback of static_cast: It doesn't work with virtual inheritance and deadly diamonds in general
			return fn( static_cast<D>(arg) );
		}

		// Actual member
		Fn fn;
	};


	// Concrete caller class for member functions
	// B: Base class of dispatched hierarchy
	// D: Derived class of dispatched hierarchy
	// R: Return type
	// C: Class type of the member function
	template <class B, class D, typename R, class C>
	struct UnaryMemberFunction : UnaryFunctionBase<B, R>
	{
		// Pointer to member function
		typedef R (C::*MemFn)(D);

		// Creates a caller 
		UnaryMemberFunction(MemFn fn, C& object)
		: fn(fn)
		, object(object)
		{
		}

		// Calls the function corresponding to ref's dynamic type
		virtual R call(B arg)
		{
			return (object.*fn)( static_cast<D>(arg) );
		}

		// Actual members
		MemFn fn;
		C& object;
	};

} // namespace detail
} // namespace aurora

#endif // AURORA_UNARYFUNCTION_HPP
