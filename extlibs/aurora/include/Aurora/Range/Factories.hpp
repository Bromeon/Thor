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
/// @brief Range factory functions

#ifndef AURORA_RANGE_FACTORIES_HPP
#define AURORA_RANGE_FACTORIES_HPP

#include <Aurora/Range/Detail/AbstractRange.hpp>
#include <Aurora/Range/Detail/ConstructAccess.hpp>
#include <Aurora/Range/Detail/Metafunctions.hpp>


namespace aurora
{

template <typename T, typename C>
class Range;

/// @addtogroup Range
/// @{

#ifdef AURORA_HAS_VARIADIC_TEMPLATES

/// @brief Chain multiple ranges
/// @param ranges Variadic argument list of ranges. A range can be anything with begin()/end() method pairs,
///  such as STL containers or objects of the aurora::Range class. Important: the ranges or containers must
///  remain valid as long as the chained range is used, hence the lvalue reference.
/// @return aurora::Range instance that represents the concatenation of the passed ranges. It has the weakest of all
///  traversal categories. For example, a bidirectional range chained with a forward range yields a forward range.
/// @details Usage:
/// @code
/// std::vector<int> v = ...;          // Random access
/// std::list<int> l = ...;            // Bidirectional
///
/// auto range = aurora::chain(v, l);  // Bidirectional
/// @endcode
template <typename... Ranges>
typename detail::RangeFromChain<Ranges...>::type chain(Ranges&... ranges)
{
	typedef typename detail::ContainersValue<Ranges...>::type    T;
	typedef typename detail::WeakestCtrCategory<Ranges...>::type C;
	typedef          detail::ContainerToIteratorRange<T>         Transformer;

	auto subranges = tupleTransform<Transformer>(std::forward_as_tuple(ranges...));
	auto ptr = detail::makeTupleRangePtr<T>(std::move(subranges));
	return detail::constructPrivate<Range<T, C>>(std::move(ptr));
}

#endif // AURORA_HAS_VARIADIC_TEMPLATES

/// @brief Construct range from iterator interval [begin, end[
/// @param begin Iterator pointing to first element
/// @param end Iterator pointing behind last element
/// @return Range referring to all elements from @a begin to @a end. Note that the range becomes invalid when any of the iterators
///  in this interval becomes invalid (for example, when the underlying container is changed). The value type and traversal category
///  of the returned range correspond to those of the iterator.
/// @details Usage:
/// @code
/// std::vector<int> v = ...;
/// auto range = aurora::makeRange(v.begin(), v.begin() + 4);
/// @endcode
template <typename Iterator>
typename detail::RangeFromIterator<Iterator>::type makeRange(Iterator begin, Iterator end)
{
	typedef typename detail::RangeFromIterator<Iterator>::type ResultRange;

	typedef typename ResultRange::ValueType T;
	typedef typename ResultRange::TraversalCategory C;
	typedef typename detail::Element<T>::type MutableT;
	static_assert(std::is_convertible<C, Traversal::Forward>::value, "Container must support forward iterators");

	return detail::constructPrivate<ResultRange>(detail::makeIteratorRangePtr<MutableT>(begin, end));
}

/// @brief Construct range from container
/// @param c Container of a class that provides begin() and end() methods
/// @return Range referring to all elements in the container. Note that the range becomes invalid when iterators to the
///  container become invalid (e.g. as a result of inserting/removing elements). The value type and traversal category of
///  the returned range correspond to those of the container.
/// @details Usage:
/// @code
/// std::vector<int> v = ...;
/// auto range = aurora::makeRange(v);
/// @endcode
template <typename Container>
typename detail::RangeFromContainer<Container>::type makeRange(Container& c)
{
	return makeRange(c.begin(), c.end());
}

/// @}

} // namespace aurora

#endif // AURORA_RANGE_FACTORIES_HPP
