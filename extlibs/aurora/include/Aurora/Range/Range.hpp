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
/// @brief Class template aurora::Range

#ifndef AURORA_RANGE_HPP
#define AURORA_RANGE_HPP

#include <Aurora/Range/RangeIterator.hpp>
#include <Aurora/Range/Detail/AbstractRange.hpp>
#include <Aurora/Range/Detail/AbstractIterator.hpp>
#include <Aurora/Range/Detail/ConstructAccess.hpp>
#include <Aurora/Meta/Templates.hpp>


namespace aurora
{

/// @addtogroup Range
/// @{

/// @brief Type-erased range
/// @details Ranges are generalizations of iterators. They act as a view (mutable or constant) to containers, they do not own
///  any elements themselves. Basic ranges can be imagined as (begin,end) iterator pairs, however they allow much more. 
///  Like iterators, ranges work on traversal categories which determine how a sequence can be traversed.
/// @n@n This class implements a <i>type-erased</i> range. This means that the underlying iterator types are abstracted and
///  only the iterator's value type is relevant. This abstraction allows you to treat ranges based on different containers, but
///  with same value types, in a uniform way (for example <i>std::vector<int></i> and <i>std::list<int></i>). It is even possible to concatenate
///  such differently typed ranges.
/// @n@n aurora::Range has value semantics; i.e. it can be copied, moved, assigned, passed to and returned from functions. Everything happens as
///  you would expect: after copying, both the origin and the copy refer to the same sequence of elements. The actual access to elements referenced
///  by a range happens through range iterators that can be acquired with begin() and end(). The construction of ranges is performed using one of
///  the global functions, most often makeRange().
/// @n@n Const-correctness is correctly propagated through ranges and range iterators. When the value type of this range is const-qualified, it
///  will not be possible to modify elements through this range; in other words, this is then a read-only range. Keep also in mind that a constant
///  range (const %Range<T, C>) is not the same as a range to constant elements (%Range<const T, C>): while the former doesn't allow to re-assign
///  the range, i.e. let it point to another sequence, the latter doesn't allow modifications of the referenced elements. This concept is exactly
///  the same as you know it from const iterators, const references and const pointers.
/// @tparam T Element type. Can be const-qualified to prevent changes of the elements.
/// @tparam C Iterator category. Must be one of the categories in namespace aurora::Traversal.
template <typename T, typename C>
class Range
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Value type, can be const-qualified
		/// 
		typedef T										ValueType;

		/// @brief %Range iterator to mutable elements
		/// 
		typedef RangeIterator<T, C>						Iterator;

		/// @brief %Range iterator to constant elements
		/// 
		typedef RangeIterator<const T, C>				ConstIterator;

		/// @brief Iterator traversal category
		/// 
		typedef C										TraversalCategory;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		typedef typename std::remove_const<T>::type						MutableType;
		typedef typename detail::AbstractRange<MutableType>::Ptr		AbstractRangePtr;
		typedef typename detail::AbstractIterator<MutableType>::Ptr		AbstractIteratorPtr;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Copy constructor
		/// 
		Range(const Range& origin)
		: mRange(origin.mRange)
		{
		}

		/// @brief Move constructor
		/// 
		Range(Range&& source)
		: mRange(std::move(source.mRange))
		{
		}

		/// @brief Construct from different range
		/// @details Can be used to add const qualifiers or to weaken the traversal category.
		template <typename T2, typename C2>
		Range(Range<T2, C2> origin)
		: mRange(std::move(origin.mRange))
		{
			static_assert( std::is_convertible<T2, T>::value, "Range element types incompatible." );
			static_assert( std::is_convertible<C2, C>::value, "Traversal categories incompatible." );
		}

		/// @brief Copy assignment operator
		/// 
		Range& operator= (const Range& origin)
		{
			mRange = origin.mRange;
			return *this;
		}

		/// @brief Move assignment operator
		/// 
		Range& operator= (Range&& source)
		{
			mRange = std::move(source.mRange);
			return *this;
		}

		/// @brief Assign from different range
		/// @details Can be used to add const qualifiers or to weaken the traversal category.
		template <typename T2, typename C2>
		Range& operator= (Range<T2, C2> origin)
		{
			static_assert( std::is_convertible<T2, T>::value, "Range element types incompatible." );
			static_assert( std::is_convertible<C2, C>::value, "Traversal categories incompatible." );

			mRange = std::move(origin.mRange);
			return *this;
		}

		/// @brief Swaps the range with another range of the same type.
		/// 
		void swap(Range& other)
		{
			aurora::swap(mRange, other.mRange);
		}

		/// @brief Returns an iterator to the begin of the range
		/// 
		Iterator begin()
		{
			return detail::constructPrivate<Iterator>(*mRange, mRange->begin());
		}

		/// @brief Returns an iterator to the end of the range
		/// 
		Iterator end()
		{
			return detail::constructPrivate<Iterator>(*mRange, nullptr);
		}


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		// Construct from smart pointer to implementation
		explicit Range(AbstractRangePtr impl)
		: mRange(std::move(impl))
		{
		}


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		AbstractRangePtr			mRange;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Friends
	template <typename T2, typename C2>
	friend class Range;

	friend struct detail::ConstructAccess;
};

/// @relates Range
/// @brief Swaps two ranges.
template <typename T, typename C>
void swap(Range<T, C>& lhs, Range<T, C>& rhs)
{
	lhs.swap(rhs);
}

/// @}

} // namespace aurora

#endif // AURORA_RANGE_HPP
