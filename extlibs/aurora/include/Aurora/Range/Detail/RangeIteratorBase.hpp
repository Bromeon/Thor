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
//	claim that you wrote the original software. If you use this software
//	in a product, an acknowledgment in the product documentation would be
//	appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be
//	misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef AURORA_RANGEITERATORBASE_HPP
#define AURORA_RANGEITERATORBASE_HPP

#include <Aurora/Range/Traversal.hpp>
#include <Aurora/Range/Detail/AbstractIterator.hpp>


namespace aurora
{

template <typename T, typename C>
class RangeIterator;

namespace detail
{

	template <typename T>
	struct AbstractRange;

	template <typename T, typename C, typename DerivedC>
	class RangeIteratorBase;

	template <typename T, typename C>
	class RangeIteratorBase<T, C, Traversal::Forward>
	{
		// ---------------------------------------------------------------------------------------------------------------------------
		// Protected types
		protected:
			typedef typename Element<T>::type							MutableType;
			typedef typename detail::AbstractIterator<MutableType>::Ptr	AbstractIteratorPtr;


		// ---------------------------------------------------------------------------------------------------------------------------
		// Public member functions
		public:
			RangeIterator<T, C>& operator++ ()
			{
				auto& itr = iterator();

				// ++beforeBegin(), reach end() and get iterator to next subrange (or nullptr if this is the last subrange)
				if (itr->increment())
					itr = abstractRange().next(std::move(itr));

				return derived();
			}
			

		// ---------------------------------------------------------------------------------------------------------------------------
		// Protected member functions
		protected:
			RangeIterator<T, C>& derived()
			{
				return static_cast<RangeIterator<T, C>&>(*this);
			}

			AbstractIteratorPtr& iterator()
			{
				return derived().mIterator;
			}

			AbstractRange<MutableType>& abstractRange()
			{
				return *derived().mAbstractRange;
			}
	};


	template <typename T, typename C>
	class RangeIteratorBase<T, C, Traversal::Bidirectional>
	: public RangeIteratorBase<T, C, Traversal::Forward>
	{		
		// ---------------------------------------------------------------------------------------------------------------------------
		// Imported names from base class
		private:
			using RangeIteratorBase<T, C, Traversal::Forward>::iterator;
			using RangeIteratorBase<T, C, Traversal::Forward>::abstractRange;
			using RangeIteratorBase<T, C, Traversal::Forward>::derived;


		// ---------------------------------------------------------------------------------------------------------------------------
		// Public member functions
		public:
			RangeIterator<T, C>& operator--()
			{
				// Find iterator to next subrange
				auto& itr = iterator();

				// --end(), get last valid iterator before end
				if (!itr)
					itr = abstractRange().beforeEnd();
				
				// --begin(), get iterator to previous subrange (or nullptr if this is the first subrange)
				else if (itr->decrement())
					itr = abstractRange().prev(std::move(itr));
	
				return derived();
			}
	};


	template <typename T, typename C>
	class RangeIteratorBase<T, C, Traversal::RandomAccess>
	: public RangeIteratorBase<T, C, Traversal::Bidirectional>
	{
	
	};

} // namespace detail
} // namespace aurora

#endif // AURORA_RANGEITERATORBASE_HPP
