/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2012 Jan Haller
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
/// @brief Class template thor::Distribution

#ifndef THOR_DISTRIBUTION_HPP
#define THOR_DISTRIBUTION_HPP

#include <Aurora/Tools/Metaprogramming.hpp>

#include <functional>


namespace thor
{
namespace detail
{

	// Functor that returns always the same value (don't use lambda expression because of Clang compiler bug)
	template <typename T>
	struct Constant
	{
		explicit Constant(T value)
		: value(value)
		{
		}

		T operator() () const
		{
			return value;
		}

		T value;
	};

} // namespace detail
	
// ---------------------------------------------------------------------------------------------------------------------------
	
	
/// @addtogroup Math
/// @{

/// @brief Class holding a rule to create values with predefined properties
/// @details Contains a callback that returns values on demand. These can be constant (always the same value), according to a
///  random distribution, or be read from a value elsewhere in your code. Generally, the callback can be any function, member
///  function or functor returning a value of type T and taking no arguments.
template <typename T>
class Distribution
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		typedef std::function<T()> FactoryFn;
	
		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Construct from constant
		/// @param constant Constant value convertible to T. The distribution's operator() will always return this value.
		template <typename U>
									Distribution(U constant
										AURORA_ENABLE_IF(std::is_convertible<U, T>::value))
		: mFactory(detail::Constant<T>(constant))
		{
		}

		/// @brief Construct from distribution function
		/// @param function Callable convertible to std::function<T()>. Every time the operator() of this distribution is invoked,
		///  it returns the return value of the specified function.
		template <typename Fn>
									Distribution(Fn function
										AURORA_ENABLE_IF(!std::is_convertible<Fn, T>::value))
		: mFactory(function)
		{
		}

		/// @brief Returns a value according to the distribution.
		/// 
		T							operator() () const
		{
			return mFactory();
		}
			
		/// @brief Exchanges the contents of *this with other.
		/// 
		void						swap(Distribution<T>& other)
		{
			mFactory.swap(other.mFactory);
		}

	
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		FactoryFn					mFactory;	
};

/// @relates Distribution
/// @brief Swaps two Distribution<T> instances.
template <typename T>
void swap(Distribution<T>& lhs, Distribution<T>& rhs)
{
	lhs.swap(rhs);
}

/// @}

} // namespace thor

#endif // THOR_DISTRIBUTION_HPP
