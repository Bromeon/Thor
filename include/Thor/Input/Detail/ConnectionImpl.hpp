/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2022 Jan Haller
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

#ifndef THOR_CONNECTIONIMPL_HPP
#define THOR_CONNECTIONIMPL_HPP

#include <algorithm>
#include <memory>


namespace thor
{
namespace detail
{

	// Abstract class that allows to disconnect listeners using type erasure
	class AbstractConnectionImpl
	{
		public:
			// Disconnects a listener from an event
			virtual void disconnect() = 0;

			// Virtual destructor
			virtual ~AbstractConnectionImpl()
			{
			}
	};

	// Concrete class implementing the actual disconnection for std::list and iterator
	template <typename List>
	class IteratorConnectionImpl : public AbstractConnectionImpl
	{
		private:
			typedef typename List::Iterator Iterator;

		public:
			// Constructor
			IteratorConnectionImpl(List& container, Iterator iterator)
			: mContainer(&container)
			, mIterator(iterator)
			{
			}

			virtual void disconnect()
			{
				mContainer->remove(mIterator);
			}

		private:
			List*		mContainer;
			Iterator	mIterator;
	};

	// Concrete class implementing the actual disconnection for any container and ID
	template <typename Container>
	class IdConnectionImpl : public AbstractConnectionImpl
	{
		private:
			typedef typename Container::value_type ValueType;

		public:
			// Constructor
			IdConnectionImpl(Container& container, unsigned int id)
			: mContainer(&container)
			, mId(id)
			{
			}

			virtual void disconnect()
			{
				// TODO: Use binary search
				auto found = std::find_if(mContainer->begin(), mContainer->end(), [this] (ValueType& v) { return v.id == mId; });

				if (found != mContainer->end())
					mContainer->erase(found);
			}

		private:
			Container*			mContainer;
			unsigned int		mId;
	};

	template <typename List>
	std::shared_ptr<IteratorConnectionImpl<List>> makeIteratorConnectionImpl(List& container, typename List::Iterator iterator)
	{
		return std::make_shared<IteratorConnectionImpl<List>>(container, iterator);
	}

	template <typename Container>
	std::shared_ptr<IdConnectionImpl<Container>> makeIdConnectionImpl(Container& container, unsigned int id)
	{
		return std::make_shared<IdConnectionImpl<Container>>(container, id);
	}

} // namespace detail
} // namespace thor

#endif // THOR_CONNECTIONIMPL_HPP
