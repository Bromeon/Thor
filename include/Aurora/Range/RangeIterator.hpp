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
/// @brief Class template aurora::RangeIterator

#ifndef AURORA_RANGEITERATOR_HPP
#define AURORA_RANGEITERATOR_HPP

#include <Aurora/Range/Detail/AbstractIterator.hpp>
#include <Aurora/Range/Detail/AbstractRange.hpp>
#include <Aurora/Range/Detail/ConstructAccess.hpp>
#include <Aurora/Range/Detail/RangeIteratorBase.hpp>

#include <iterator>
#include <type_traits>


namespace aurora
{

template <typename T, typename C>
class Range;

/// @addtogroup Range
/// @{

/// @brief Iterator on type-erased ranges.
/// @details Class to iterate on aurora::Range instances. Depending on the traversal category @a C, different sets of operations
///  are supported. For example, forward iterators provide only @a ++ operators, while bidirectional also provide @a --. An iterator
///  is only valid as long as the underlying aurora::Range instance exists, and ranges are only valid as long as their origin
///  (most often a container) exists.
/// @tparam T Element type. Can be const-qualified to prevent modifications of the elements through this iterator.
/// @tparam C Iterator traversal category. Must be a type in the aurora::Traversal namespace.
template <typename T, typename C>
class RangeIterator
	: public detail::RangeIteratorBase<T, C, C>
	, public std::iterator<C, T>
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Value type (template parameter T)
		/// 
		typedef T										ValueType;

		/// @brief Iterator traversal category
		/// 
		typedef C										TraversalCategory;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		typedef typename std::remove_const<T>::type					MutableType;
		typedef typename detail::AbstractIterator<MutableType>::Ptr AbstractIteratorPtr;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Copy constructor
		/// 
		RangeIterator(const RangeIterator& origin)
		: mAbstractRange(origin.mAbstractRange)
		, mIterator(origin.mIterator)
		{
		}

		/// @brief Move constructor
		/// 
		RangeIterator(RangeIterator&& source)
		: mAbstractRange(std::move(source.mAbstractRange))
		, mIterator(std::move(source.mIterator))
		{
		}

		/// @brief Construct from different iterator
		/// @details Can be used to add const qualifiers or to weaken the traversal category.
		template <typename T2, typename C2>
		RangeIterator(RangeIterator<T2, C2> origin)
		: mAbstractRange(std::move(origin.mAbstractRange))
		, mIterator(std::move(origin.mIterator))
		{
			static_assert( std::is_convertible<T2, T>::value, "Range element types incompatible." );
			static_assert( std::is_convertible<C2, C>::value, "Traversal categories incompatible." );
		}

		/// @brief Copy assignment operator
		/// 
		RangeIterator& operator= (const RangeIterator& origin)
		{
			mAbstractRange = origin.mAbstractRange;
			mIterator = origin.mIterator;
			return *this;
		}

		/// @brief Move assignment operator
		/// 
		RangeIterator& operator= (RangeIterator&& source)
		{
			mAbstractRange = std::move(source.mAbstractRange);
			mIterator = std::move(source.mIterator);
			return *this;
		}

		/// @brief Assign from different iterator
		/// @details Can be used to add const qualifiers or to weaken the traversal category.
		template <typename T2, typename C2>
		RangeIterator& operator= (RangeIterator<T2, C2> origin)
		{
			static_assert( std::is_convertible<T2, T>::value, "Iterator element types incompatible." );
			static_assert( std::is_convertible<C2, C>::value, "Traversal categories incompatible." );

			mAbstractRange = std::move(origin.mAbstractRange);
			mIterator = std::move(origin.mIterator);
			return *this;
		}

#ifdef AURORA_DOXYGEN_SECTION

		/// @brief Increment operator
		/// @details Available for all traversal categories.
		RangeIterator& operator++();

		/// @brief Decrement operator
		/// @details Requires Traversal::Bidirectional category.
		RangeIterator& operator--();

#endif // AURORA_DOXYGEN_SECTION


		/// @brief Equality comparison operator
		/// 
		bool operator== (const RangeIterator& rhs) const
		{
			return mAbstractRange == rhs.mAbstractRange
				&& (!mIterator && !rhs.mIterator
				  || mIterator && rhs.mIterator && mIterator->equal(*rhs.mIterator));
		}

		/// @brief Inequality comparison operator
		/// 
		bool operator!= (const RangeIterator& rhs) const
		{
			return !(*this == rhs);
		}

		/// @brief Dereferencing operator
		/// 
		T& operator* () const
		{
			assert(mIterator);
			return mIterator->deref();
		}

		/// @brief Member dereferencing operator
		/// 
		T* operator-> () const
		{
			return &operator*();
		}


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		// Construct from range implementation
		RangeIterator(detail::AbstractRange<MutableType>& enclosingRange, AbstractIteratorPtr iterator)
		: mAbstractRange(&enclosingRange)
		, mIterator(std::move(iterator))
		{
		}

	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		detail::AbstractRange<MutableType>*	mAbstractRange;
		AbstractIteratorPtr					mIterator;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Friends	
	template <typename T2, typename C2>
	friend class RangeIterator;
		
	friend class detail::RangeIteratorBase<T, C, Traversal::Forward>; // top base class

	friend struct detail::ConstructAccess;
};

/// @}

} // namespace aurora

#endif // AURORA_RANGEITERATOR_HPP
