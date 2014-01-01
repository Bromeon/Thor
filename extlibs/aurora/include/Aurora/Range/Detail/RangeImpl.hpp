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

#ifndef AURORA_RANGEIMPL_HPP
#define AURORA_RANGEIMPL_HPP

#include <Aurora/Range/IteratorCategories.hpp>
#include <Aurora/SmartPtr/CopiedPtr.hpp>

#include <type_traits>
#include <iterator>


namespace aurora
{

template <typename T, typename C>
class Range;

namespace detail
{

	// Back-end: type-erased base class
	template <typename T, typename C>
	struct AbstractRange;

	template <typename T>
	struct AbstractRange<T, Traversal::Forward>
	{
		virtual ~AbstractRange() {}

		virtual T& front() = 0;
		virtual void popFront() = 0;
		virtual bool empty() const = 0;
	};

	template <typename T>
	struct AbstractRange<T, Traversal::Bidirectional>
		: AbstractRange<T, Traversal::Forward>
	{
		virtual T& back() = 0;
		virtual void popBack() = 0;
	};

	template <typename T>
	struct AbstractRange<T, Traversal::RandomAccess>
		: AbstractRange<T, Traversal::Bidirectional>
	{
	};
	
	// ---------------------------------------------------------------------------------------------------------------------------
	
	
	// Back-end: Derived class for iterator pair
	template <typename T, typename C, typename SpecializedC, typename Itr>
	struct IteratorRange;

	template <typename T, typename C, typename Itr>
	struct IteratorRange<T, C, Traversal::Forward, Itr>
		: AbstractRange<T, C>
	{
		explicit IteratorRange(Itr begin, Itr end)
		: begin(begin), end(end)
		{
		}

		virtual T& front()
		{
			return *begin;
		}

		virtual void popFront()
		{
			++begin;
		}

		virtual bool empty() const
		{
			return begin == end;
		}

		Itr begin, end;
	};

	template <typename T, typename C, typename Itr>
	struct IteratorRange<T, C, Traversal::Bidirectional, Itr>
		: IteratorRange<T, C, Traversal::Forward, Itr>
	{
		// Note: this-> required to make name dependent

		explicit IteratorRange(Itr begin, Itr end)
		: IteratorRange<T, C, Traversal::Forward, Itr>(begin, end)
		{
		}

		virtual T& back()
		{
			return *std::prev(this->end);
		}

		virtual void popBack()
		{
			--this->end;
		}
	};

	template <typename T, typename C, typename Itr>
	struct IteratorRange<T, C, Traversal::RandomAccess, Itr>
		: IteratorRange<T, C, Traversal::Bidirectional, Itr>
	{
		explicit IteratorRange(Itr begin, Itr end)
		: IteratorRange<T, C, Traversal::Bidirectional, Itr>(begin, end)
		{
		}
	};

	// ---------------------------------------------------------------------------------------------------------------------------
	
	
	// Front-end: Mixin for aurora::Range<T, C>
	template <typename T, typename C, typename SpecializedC>
	struct RangeBase;

	// Class to gain access via friend
	struct RangeAccess
	{
		template <typename T, typename C, typename C2>
		static AbstractRange<typename std::remove_const<T>::type, C>& getImpl(RangeBase<T, C, C2>* object)
		{
			return *static_cast<Range<T, C>*>(object)->mRange;
		}

		template <typename T, typename C, typename C2>
		static const AbstractRange<typename std::remove_const<T>::type, C>& getImpl(const RangeBase<T, C, C2>* object)
		{
			return *static_cast<const Range<T, C>*>(object)->mRange;
		}

		template <typename T, typename C>
		static CopiedPtr<AbstractRange<typename std::remove_const<T>::type, C>>& getPtr(Range<T, C>& object)
		{
			return object.mRange;
		}
	};

	template <typename T, typename C>
	struct RangeBase<T, C, Traversal::Forward>
	{
		T& front()
		{
			return RangeAccess::getImpl(this).front();
		}

		void popFront()
		{
			RangeAccess::getImpl(this).popFront();
		}

		bool empty() const
		{
			return RangeAccess::getImpl(this).empty();
		}
	};

	template <typename T, typename C>
	struct RangeBase<T, C, Traversal::Bidirectional>
		: RangeBase<T, C, Traversal::Forward>
	{
		T& back()
		{
			return RangeAccess::getImpl(this).back();
		}

		void popBack()
		{
			return RangeAccess::getImpl(this).popBack();
		}
	};
	
	template <typename T, typename C>
	struct RangeBase<T, C, Traversal::RandomAccess>
		: RangeBase<T, C, Traversal::Bidirectional>
	{
	};

} // namespace detail
} // namespace aurora

#endif // AURORA_RANGEIMPL_HPP
