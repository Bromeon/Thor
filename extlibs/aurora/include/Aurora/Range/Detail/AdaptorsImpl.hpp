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

#ifndef AURORA_ADAPTORSIMPL_HPP
#define AURORA_ADAPTORSIMPL_HPP

#include <Aurora/Range/Range.hpp>

#include <type_traits>
#include <iterator>


namespace aurora
{
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

	// Metafunction to find container traversal category
	template <typename Container>
	struct ContainerCategory
	{
		typedef typename std::iterator_traits<
			typename Container::iterator
		>::iterator_category type;
	};

	// Container value type
	template <typename Container>
	struct ContainerValue
	{
		typedef typename std::remove_reference<
			typename Container::reference
		>::type type;
	};

	// Concrete backend type for chained ranges
	template <typename T, typename C, typename SpecializedC>
	struct ChainedRange;
		
	template <typename T, typename C>
	struct ChainedRange<T, C, Traversal::Forward>
		: AbstractRange<T, C>
	{
		typedef typename detail::AbstractRange<T, C> AbstractRange;

		ChainedRange(CopiedPtr<AbstractRange>&& lhs, CopiedPtr<AbstractRange>&& rhs)
		: first(std::move(lhs))
		, second(std::move(rhs))
		{
		}

		virtual T& front()
		{
			return getFirstValid().front();
		}

		virtual void popFront()
		{
			getFirstValid().popFront();
		}

		virtual bool empty() const
		{
			return first->empty() && second->empty();
		}

		AbstractRange& getFirstValid() const
		{
			return first->empty() ? *second : *first;
		}

		AbstractRange& getLastValid() const
		{
			return second->empty() ? *first : *second;
		}

		CopiedPtr<AbstractRange> first, second;
	};

	template <typename T, typename C>
	struct ChainedRange<T, C, Traversal::Bidirectional>
		: ChainedRange<T, C, Traversal::Forward>
	{
		typedef typename detail::AbstractRange<T, C> AbstractRange;

		ChainedRange(CopiedPtr<AbstractRange>&& lhs, CopiedPtr<AbstractRange>&& rhs)
		: ChainedRange<T, C, Traversal::Forward>(std::move(lhs), std::move(rhs))
		{
		}

		virtual T& back()
		{
			return this->getLastValid().back();
		}

		virtual void popBack()
		{
			this->getLastValid().popBack();
		}
	};

	template <typename T, typename C>
	struct ChainedRange<T, C, Traversal::RandomAccess>
		: ChainedRange<T, C, Traversal::Bidirectional>
	{
		typedef typename detail::AbstractRange<T, C> AbstractRange;

		ChainedRange(CopiedPtr<AbstractRange>&& lhs, CopiedPtr<AbstractRange>&& rhs)
		: ChainedRange<T, C, Traversal::Bidirectional>(std::move(lhs), std::move(rhs))
		{
		}
	};

} // namespace detail
} // namespace aurora

#endif // AURORA_ADAPTORSIMPL_HPP
