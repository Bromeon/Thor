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

#ifndef AURORA_ASSOCIATIVEHELPERS_HPP
#define AURORA_ASSOCIATIVEHELPERS_HPP

#include <algorithm>


namespace aurora
{
namespace detail
{

	// Structure similar to std::pair, but only comparing the first element
	template <typename Key, typename Value>
	struct KeyValuePair
	{
		Key		key;
		Value	value;

		KeyValuePair(const Key& key, const Value& value)
		: key(key)
		, value(value)
		{
		}
	};

	// Less-than operator for sorting
	template <typename Key, typename Value>
	bool operator< (const KeyValuePair<Key, Value>& lhs, const KeyValuePair<Key, Value>& rhs)
	{
		return lhs.key < rhs.key;
	}
	
	// ---------------------------------------------------------------------------------------------------------------------------
	
	
	// Are values equivalent with respect to sorting criterion? (Not necessarily equal)
	template <typename T>
	bool equivalent(const T& lhs, const T& rhs)
	{
		return !(lhs < rhs) && !(rhs < lhs);
	}

	// Binary search with useful return value, in contrast to std::binary_search()
	// Returns either iterator to first found element, or end-iterator of sequence
	template <typename ForwardIterator, typename T>
	ForwardIterator binarySearch(ForwardIterator first, ForwardIterator last, const T& value)
	{
		// Note: std::lower_bound() has O(log n) on random access iterators 
		ForwardIterator result = std::lower_bound(first, last, value);

		// std::lower_bound() returns iterator to first element >= value, which can be inequal to value
		if (result == last || !equivalent(*result, value))
			return last;
		else
			return result;
	}

} // namespace detail
} // namespace aurora

#endif // AURORA_ASSOCIATIVEHELPERS_HPP
