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

#ifndef AURORA_BINARYFUNCTION_HPP
#define AURORA_BINARYFUNCTION_HPP


namespace aurora
{
namespace detail
{

	// Type-erased base class for caller interface
	// B: Base class of dispatched hierarchy
	// R: Function return type
	template <class B, typename R>
	struct BinaryFunctionBase
	{
		// Calls the function corresponding to arg1's and arg2's dynamic type
		virtual R call(B arg1, B arg2) = 0;

		// Virtual destructor
		virtual ~BinaryFunctionBase() {}
	};


	// Concrete caller class for global functions and function objects (like std::function)
	// B: Base class of dispatched hierarchy
	// D1: Derived class of dispatched hierarchy (1st parameter)
	// D2: Derived class of dispatched hierarchy (2nd parameter)
	// Fn: Function type
	template <class B, class D1, class D2, typename R, typename Fn = R(*)(D1, D2)>
	struct BinaryGlobalFunction : BinaryFunctionBase<B, R>
	{
		// Creates a caller 
		explicit BinaryGlobalFunction(const Fn& fn)
		: fn(fn)
		{
		}

		// Calls the function corresponding to ref's dynamic type
		virtual R call(B arg1, B arg2)
		{
			// Drawback of static_cast: It doesn't work with virtual inheritance and deadly diamonds in general
			return fn( static_cast<D1>(arg1), static_cast<D2>(arg2) );
		}

		// Actual member
		Fn fn;
	};


	// Concrete caller class for member functions
	// B: Base class of dispatched hierarchy
	// D1: Derived class of dispatched hierarchy (1st parameter)
	// D2: Derived class of dispatched hierarchy (2nd parameter)
	// R: Return type
	// C: Class type of the member function
	template <class B, class D1, class D2, typename R, class C>
	struct BinaryMemberFunction : BinaryFunctionBase<B, R>
	{
		// Pointer to member function type
		typedef R (C::*MemFn)(D1, D2);

		// Creates a caller 
		BinaryMemberFunction(MemFn fn, C& object)
		: fn(fn)
		, object(object)
		{
		}

		// Calls the function corresponding to ref's dynamic type
		virtual R call(B arg1, B arg2)
		{
			return (object.*fn)( static_cast<D1>(arg1), static_cast<D2>(arg2) );
		}

		// Actual members
		MemFn fn;
		C& object;
	};

} // namespace detail
} // namespace aurora

#endif // AURORA_BINARYFUNCTION_HPP
