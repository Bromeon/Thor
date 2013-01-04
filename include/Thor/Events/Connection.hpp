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

/// @file
/// @brief Classes thor::Connection, thor::ScopedConnection

#ifndef THOR_CONNECTION_HPP
#define THOR_CONNECTION_HPP

#include <Thor/Config.hpp>

#include <SFML/System/NonCopyable.hpp>

#include <memory>


namespace thor
{
namespace detail
{

	class AbstractConnectionImpl;

	template <typename Parameter>
	class Listener;

} // namespace detail


/// @addtogroup Events
/// @{

/// @brief Class that manages the connection between an event and a listener.
/// @details You need a connection, if you want to remove listeners bound to events. The class %Connection uses
///  shared-ownership semantics, that means copying its instances results in multiple references to the same listener.
///  It automatically invalidates when the referenced listener is disconnected from the event system.
class THOR_API Connection
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor: Creates an instance which is initially invalid.
		/// @details Assign another object to make this instance a valid %Connection.
									Connection();
							
		/// @brief Checks whether the instance currently references a listener.
		/// @return true if this instance is currently connected.
		bool						isConnected() const;
		
		/// @brief Invalidates this instance.
		/// @details This does not affect the referenced listener (if any) nor other %Connection instances.
		///  Invalidating an already invalid instance has no effects.
		///  @n After the invalidation, you can reuse this instance by assigning another one. But normally, it pays
		///  out to have as few invalid connections as possible, as this saves you from case differentiations.
		void						invalidate();
		
		/// @brief Disconnects the referenced listener from its triggering event.
		/// @details This action invalidates this and all other Connections to the referenced listener.
		///  If no connection exists at the moment, this function has no effect.
		void						disconnect();

	
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::weak_ptr<detail::AbstractConnectionImpl> mWeakRef;
		
	
	// ---------------------------------------------------------------------------------------------------------------------------
	// Friends
	template <typename Parameter>
	friend class detail::Listener;
};


/// @brief Class that manages the connection between an event and a listener and disconnects it at destruction.
/// @details In contrast to thor::Connection, the thor::ScopedConnection class disconnects its referenced listener
///  in the destructor. You can use ScopedConnection for temporary listeners that aren't needed after the end of scope.
///  @n ScopedConnection is noncopyable.
class THOR_API ScopedConnection : private sf::NonCopyable
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor: Initializes the instance with a given %Connection.
		/// @details Scoped connections must be initialized in the constructor. You cannot assign a new value later.
		/// @param connection Connection which is copied. It may be invalid.
		explicit					ScopedConnection(const Connection& connection);
				
		/// @brief Destructor: Disconnects the event-listener connection.
		/// @details In case the instance isn't connected, nothing happens.
									~ScopedConnection();
				
		/// @brief Checks whether the instance currently references a listener.
		/// @return true if this instance is currently connected.
		bool						isConnected() const;

	
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		Connection					mConnection;
};

/// @}

} // namespace thor

#endif // THOR_CONNECTION_HPP
