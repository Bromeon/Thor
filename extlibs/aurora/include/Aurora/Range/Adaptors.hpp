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
/// @brief Range adaptors

#ifndef AURORA_ADAPTORS_HPP
#define AURORA_ADAPTORS_HPP

#include <Aurora/Range/Detail/AdaptorsImpl.hpp>


namespace aurora
{

/// @addtogroup Range
/// @{

/// @brief Chain two ranges
/// @param lhs First range (on left side).
/// @param rhs Second range (on right side).
/// @return A new range that represents the concatenation of the two passed ranges. It has the weaker of the
///  two categories. For example, a bidirectional range chained with a forward range yields a forward range.
template <typename T, typename C1, typename C2>
Range<T, typename detail::WeakerCategory<C1, C2>::type> chain(Range<T, C1> lhs, Range<T, C2> rhs)
{
	typedef typename detail::WeakerCategory<C1, C2>::type C;
	
	auto& lhsPtr = detail::RangeAccess::getPtr(lhs);
	auto& rhsPtr = detail::RangeAccess::getPtr(rhs);

	return Range<T, C>( 
		Type<detail::RangeAccess>(),
		new detail::ChainedRange<T, C, C>(std::move(lhsPtr), std::move(rhsPtr)) );
}

/// @brief Construct range from container
/// @param c Container class, must provide begin() and end() methods
/// @return Range referring to all elements in the container. Note that the range becomes invalid when iterators to the
///  container become invalid (as a result of inserting/removing elements). The value type and traversal category of the
///  returned range correspond to those of the container.
template <typename Container>
Range<typename detail::ContainerValue<Container>::type, typename detail::ContainerCategory<Container>::type> makeRange(Container& c)
{
	typedef typename detail::ContainerValue<Container>::type T;
	typedef typename detail::ContainerCategory<Container>::type C;
	static_assert(std::is_convertible<C, Traversal::Forward>::value, "Container must support forward iterators");

	return Range<T, C>(c.begin(), c.end());
}

/// @}

} // namespace aurora

#endif // AURORA_ADAPTORS_HPP
