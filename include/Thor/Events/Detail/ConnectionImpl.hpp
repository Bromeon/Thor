/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2013 Jan Haller
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


	// Concrete class implementing the actual disconnection
	template <class ListenerContainer>
	class ConnectionImpl : public AbstractConnectionImpl
	{
		private:	
			typedef typename ListenerContainer::Iterator ListenerIterator;

		public:
			// Constructor
			ConnectionImpl(ListenerContainer& container, ListenerIterator iterator)
			: mContainer(&container)
			, mIterator(iterator)
			{
			}
		
			virtual void disconnect()
			{
				mContainer->remove(mIterator);
			}
	
		private:	
			ListenerContainer*	mContainer;
			ListenerIterator	mIterator;
	};

} // namespace detail
} // namespace thor

#endif // THOR_CONNECTIONIMPL_HPP
