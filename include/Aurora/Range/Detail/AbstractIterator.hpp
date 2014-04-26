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

#ifndef AURORA_ABSTRACTITERATOR_HPP
#define AURORA_ABSTRACTITERATOR_HPP

#include <Aurora/Range/Traversal.hpp>
#include <Aurora/Range/Detail/Metafunctions.hpp>
#include <Aurora/Meta/Templates.hpp>
#include <Aurora/SmartPtr/CopiedPtr.hpp>


namespace aurora
{

template <typename T, typename C>
class Range;

namespace detail
{

	// Type-erased base class for iterator implementatons
	// Design: Type-erase as much as possible -> only mutable value type is kept in base class, not traversal category
	// or CV qualification. This requires some possibly unused methods, but reduces code bloat and simplifies the
	// implementation significantly.
	template <typename T>
	struct AbstractIterator
	{
		//typedef std::unique_ptr<AbstractIterator> Ptr;
		typedef CopiedPtr<AbstractIterator> Ptr;

		virtual ~AbstractIterator() {}

		virtual T& deref() = 0;
		virtual bool increment() = 0;
		virtual bool decrement() { assert(false); return true; }

		virtual bool equal(AbstractIterator& other) const = 0;

		virtual void setSubrangeIndex(std::size_t index) = 0;
		virtual std::size_t getSubrangeIndex() const = 0;
	};
	
	// ---------------------------------------------------------------------------------------------------------------------------
	
	
	// Concrete iterator implementation
	template <typename T, typename Itr, typename C>
	struct SubIterator : AbstractIterator<T>
	{
		SubIterator(Itr begin, Itr end, bool /*startAtBegin*/)
		: end(end)
		, itr(begin)
		, subrangeIndex(0u)
		{
		}

		virtual T& deref()
		{
			// Effect of const_cast is only temporary, this allows iterators with different value type const-qualifications
			// to use the same type-erased backend, namely AbstractIterator<T>. Type safety is maintained in the API,
			// since aurora::RangeIterator<const T, C> doesn't allow mutable access to elements.
			return const_cast<T&>(*itr);
		}

		virtual bool increment()
		{
			++itr;
			return itr == end;
		}

		virtual bool equal(AbstractIterator<T>& other) const
		{
			// First check whether in same subrange
			if (subrangeIndex != other.getSubrangeIndex())
				return false;

			// If so, downcast other (we can be sure it has the same iterator type)
			assert(dynamic_cast<SubIterator*>(&other));
			auto& o = static_cast<SubIterator&>(other);

			return itr == o.itr;
		}

		virtual void setSubrangeIndex(std::size_t index)
		{
			subrangeIndex = index;
		}

		virtual std::size_t getSubrangeIndex() const
		{
			return subrangeIndex;
		}

		Itr end;
		Itr itr;
		std::size_t subrangeIndex;
	};

	template <typename T, typename Itr>
	struct SubIterator<T, Itr, Traversal::Bidirectional>
		: SubIterator<T, Itr, Traversal::Forward>
	{
		// Import names from base class
		using SubIterator<T, Itr, Traversal::Forward>::itr;
		using SubIterator<T, Itr, Traversal::Forward>::end;

		SubIterator(Itr begin, Itr end, bool startAtBegin)
		: SubIterator<T, Itr, Traversal::Forward>(begin, end, startAtBegin)
		, begin(begin)
		{
			// Set iterator to beforeEnd() if requested (requires bidirectional category)
			if (!startAtBegin)
				itr = --end;
		}

		virtual bool decrement()
		{
			if (itr == begin)
			{
				return true;
			}
			else
			{
				--itr;
				return false;
			}
		}

		Itr begin;
	};

	template <typename T, typename Itr>
	struct SubIterator<T, Itr, Traversal::RandomAccess>
		: SubIterator<T, Itr, Traversal::Bidirectional>
	{
		SubIterator(Itr begin, Itr end, bool startAtBegin)
		: SubIterator<T, Itr, Traversal::Bidirectional>(begin, end, startAtBegin)
		{
		}
	};
	
	// ---------------------------------------------------------------------------------------------------------------------------
	
	
	template <typename T, typename Itr>
	typename AbstractIterator<T>::Ptr makeSubIterator(Itr begin, Itr end, bool startAtBegin)
	{
		typedef typename IteratorCategory<Itr>::type C;
		typedef SubIterator<T, Itr, C> Type;

		return makeCopied<Type>(begin, end, startAtBegin);
	}

} // namespace detail
} // namespace aurora

#endif // AURORA_ABSTRACTITERATOR_HPP
