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
/// @brief Class template aur::Distribution

#ifndef AURORA_DISTRIBUTION_HPP
#define AURORA_DISTRIBUTION_HPP

#include <Aurora/Tools/Detail/Metaprogramming.hpp>
#include <Aurora/Config.hpp>

#include AURORA_TR1_HEADER(functional)


namespace aur
{
	namespace detail
	{

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


/// @addtogroup Tools
/// @{

/// @brief Class holding a rule to create values with predefined properties
/// @details Contains a callback that returns values on demand. These can be constant (always the same value), according to a
///  random distribution, or be read from a value elsewhere in your code.
template <typename T>
class Distribution
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		typedef std::tr1::function<T()> FactoryFn;
	
		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Implicit constructor from a constant value
		/// @details
		template <typename U>
									Distribution(U initializer)
		: mFactory(typename detail::Conditional<
				std::tr1::is_convertible<U, T>::value,
				detail::Constant<T>,
				FactoryFn
			>::Type(initializer))
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

/// @relates 
/// @brief Swaps two Distribution<T> instances.
template <typename T>
void swap(Distribution<T>& lhs, Distribution<T>& rhs)
{
	lhs.swap(rhs);
}

/// @}

} // namespace aur

#endif // AURORA_DISTRIBUTION_HPP
