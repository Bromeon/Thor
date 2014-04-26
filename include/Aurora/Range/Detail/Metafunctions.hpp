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

#ifndef AURORA_METAFUNCTIONS_HPP
#define AURORA_METAFUNCTIONS_HPP

#include <Aurora/Config.hpp>

#include <type_traits>
#include <iterator>


namespace aurora
{

template <typename T, typename C>
class Range;

namespace detail
{

	// Metafunction to find out weaker of two traversal categories
	template <typename C1, typename C2>
	struct WeakerCategory
	{
		typedef typename std::conditional<
			std::is_base_of<C1, C2>::value,
			C1,
			C2
		>::type type;
	};

	// Pure element type (strips CV qualification from a type)
	template <typename T>
	struct Element
	{
		typedef typename std::remove_const<T>::type type;
	};

	// Metafunction to extract iterator traversal category
	// Wraps std::iterator_traits for abstraction from standard iterators
	template <typename Iterator>
	struct IteratorCategory
	{
		typedef typename std::iterator_traits<Iterator>::iterator_category type;
	};

	// Metafunction to extract iterator value type
	template <typename Iterator>
	struct IteratorValue
	{
		typedef typename std::remove_reference<
			typename std::iterator_traits<Iterator>::reference
		>::type type;
	};

	// Check whether iterator fulfills a traversal category
	template <typename Iterator, typename Category>
	struct HasCategory
	{
		static const bool value = std::is_convertible<
			typename IteratorCategory<Iterator>::type,
			Category
		>::value;
	};

	// Extract container traversal category (note: Container may also be reference to a container)
	template <typename Container>
	struct ContainerCategory
	{
		typedef typename IteratorCategory<
			decltype(std::declval<Container>().begin())
		>::type type;
	};

	// Extract container value type
	template <typename Container>
	struct ContainerValue
	{
		typedef typename std::remove_reference<
			decltype(*std::declval<Container>().begin())
		>::type type;
	};

	template <typename T, typename C>
	struct ContainerValue<Range<T, C>>
	{
		typedef typename Element<T>::type type;
	};


#ifdef AURORA_HAS_VARIADIC_TEMPLATES

	// Value type of first container, from variadic list of container types
	template <typename... Ctrs>
	struct ContainersValue;

	template <typename Ctr, typename... Ctrs>
	struct ContainersValue<Ctr, Ctrs...>
	{
		typedef typename ContainerValue<Ctr>::type type;
	};

	// Metafunction to find weakest traversal category
	template <typename... Cs>
	struct WeakestCategory;

	template <typename C, typename... Cs>
	struct WeakestCategory<C, Cs...>
	{
		typedef typename WeakerCategory<
			C,
			typename WeakestCategory<Cs...>::type
		>::type type;
	};

	template <typename C>
	struct WeakestCategory<C>
	{
		typedef C type;
	};

	// Find weakest traversal category, given a number of containers
	template <typename... Ctrs>
	struct WeakestCtrCategory;

	template <typename Ctr, typename... Ctrs>
	struct WeakestCtrCategory<Ctr, Ctrs...>
	{
		typedef typename WeakerCategory<
			typename ContainerCategory<Ctr>::type,
			typename WeakestCtrCategory<Ctrs...>::type
		>::type type;
	};

	template <typename Ctr>
	struct WeakestCtrCategory<Ctr>
	{
		typedef typename ContainerCategory<Ctr>::type type;
	};

	template <typename... Ranges>
	struct RangeFromChain
	{
		typedef Range<
			typename detail::ContainersValue<Ranges...>::type,
			typename detail::WeakestCtrCategory<Ranges...>::type
		> type;
	};

#endif // AURORA_HAS_VARIADIC_TEMPLATES

	// Range from iterator type
	template <typename Iterator>
	struct RangeFromIterator
	{
		typedef Range<
			typename detail::IteratorValue<Iterator>::type,
			typename detail::IteratorCategory<Iterator>::type
		> type;
	};

	// Range from container type
	template <typename Container>
	struct RangeFromContainer
	{
		typedef Range<
			typename detail::ContainerValue<Container>::type, 
			typename detail::ContainerCategory<Container>::type
		> type;
	};

} // namespace detail
} // namespace aurora

#endif // AURORA_METAFUNCTIONS_HPP
