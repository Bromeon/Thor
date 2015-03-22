/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2015 Jan Haller
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

#ifndef THOR_EVENTLISTENER_HPP
#define THOR_EVENTLISTENER_HPP

#include <Thor/Config.hpp>
#include <Aurora/Tools/ForEach.hpp>
#include <Thor/Input/Connection.hpp>
#include <Thor/Input/Detail/ConnectionImpl.hpp>

#include <functional>
#include <vector>
#include <map>
#include <algorithm>


namespace thor
{
namespace detail
{

	// Class to store a unique command listener.
	// When a Listener instance is destroyed, all weak pointers from Connections to it are invalidated,
	// thus preventing undefined behavior when disconnecting destroyed Listeners.
	template <typename Parameter>
	class Listener
	{
		private:
			// Callback function type
			typedef std::function<void(Parameter)> Function;

		public:
			// Constructor, initializes function with fn and creates a new unique id
			Listener(const Function& fn)
			: mFunction(fn)
			, mStrongRef() // will be initialized later by setEnvironment()
			{
			}

			// Call function
			void call(Parameter arg) const
			{
				mFunction(arg);
			}

			// Swap instances
			void swap(Listener& other)
			{
				std::swap(mFunction,	other.mFunction);
				std::swap(mStrongRef,	other.mStrongRef);
			}

			// Sets the container and iterator in which this Listener is hold (to be able to create
			// Connections from *this)
			template <class ListenerContainer>
			void setEnvironment(ListenerContainer& container, typename ListenerContainer::Iterator iterator)
			{
				mStrongRef = makeIteratorConnectionImpl(container, iterator);
			}

			// Creates a Connection that can disconnect this Listener
			Connection shareConnection() const
			{
				return Connection(mStrongRef);
			}

		private:
			Function								mFunction;
			std::shared_ptr<AbstractConnectionImpl>	mStrongRef;
	};


	// Sequential container for listener
	template <typename Parameter>
	class ListenerSequence
	{
		public:
			// The type of the function together with the id
			typedef Listener<Parameter>				ValueType;

		private:
			// The container type used to store the callback functions
			typedef std::vector<ValueType>			Container;

		public:
			// The iterator type (used to disconnect listeners)
			typedef typename Container::iterator	Iterator;

		public:
			// Inserts a new listener to the collection and returns the respective Connection.
			Connection add(const ValueType& listener)
			{
				// Actual insertion
				mListeners.push_back(listener);

				// Let the Listener know about its container and iterator
				Iterator added = mListeners.end() - 1;
				added->setEnvironment(*this, added);

				// Create connection from the added Listener
				return added->shareConnection();
			}

			// Removes a listener through the given iterator.
			void remove(Iterator iterator)
			{
				// Erase in O(1)
				iterator->swap(mListeners.back());
				mListeners.pop_back();
			}

			// Removes all listeners from the container
			void clear()
			{
				mListeners.clear();
			}

			// Invokes all stored functions with arg as argument.
			void call(Parameter arg) const
			{
				AURORA_FOREACH(const ValueType& listener, mListeners)
					listener.call(arg);
			}

		private:
			Container mListeners;
	};


	// Associative container (map) for listener
	template <typename Trigger, typename Parameter>
	class ListenerMap
	{
		public:
			// The type of the function together with the id
			typedef Listener<Parameter>							ValueType;

			// The event identifier associated with the listener
			typedef Trigger										KeyType;

		private:
			// The container type used to store the callback functions
			typedef std::multimap<KeyType, ValueType>			Container;

			// The const iterator type (used internally)
			typedef typename Container::const_iterator			ConstIterator;

		public:
			// The iterator type (used to disconnect listeners)
			typedef typename Container::iterator				Iterator;

		public:
			// Inserts a new listener to the collection and returns the respective Connection.
			Connection add(const KeyType& trigger, const ValueType& listener)
			{
				// Actual insertion
				Iterator added = mListeners.insert( std::make_pair(trigger, listener) );

				// Let the Listener know about its container and iterator
				added->second.setEnvironment(*this, added);

				// Create connection from the added Listener
				return added->second.shareConnection();
			}

			// Removes a listener through the given iterator.
			void remove(Iterator iterator)
			{
				mListeners.erase(iterator);
			}

			// Removes all listeners for a specific key
			void clear(KeyType key)
			{
				mListeners.erase(key);
			}

			// Removes all listeners from the container
			void clearAll()
			{
				mListeners.clear();
			}

			// Invokes all stored functions with arg as argument.
			void call(Trigger event, Parameter arg) const
			{
				std::pair<ConstIterator, ConstIterator> range = mListeners.equal_range(event);

				for (ConstIterator itr = range.first; itr != range.second; ++itr)
					itr->second.call(arg);
			}

		private:
			Container mListeners;
	};

} // namespace detail
} // namespace thor

#endif // THOR_EVENTLISTENER_HPP
