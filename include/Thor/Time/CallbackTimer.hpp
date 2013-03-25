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
/// @brief Class thor::CallbackTimer

#ifndef THOR_CALLBACKTIMER_HPP
#define THOR_CALLBACKTIMER_HPP

#include <Thor/Time/Timer.hpp>
#include <Thor/Input/Detail/EventListener.hpp>
#include <Thor/Config.hpp>

#include <SFML/System/Time.hpp>


namespace thor
{

/// @addtogroup Time
/// @{

/// @brief Advanced timer with the ability to trigger function calls.
/// @details Clock class that counts time down. As an extension of Timer, this class is able
///  to register functions that are called at expiration time.
class THOR_API CallbackTimer : public Timer
{	
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Callback type for events being processed by CallbackTimer.
		///
		typedef std::function< void(CallbackTimer&) >		Listener;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor: Creates a callback timer that is initially expired.
		/// @details You have to call reset() or restart() before you can use the countdown functionality.
									CallbackTimer();
		
		virtual void				reset(sf::Time timeLimit);

		virtual void				restart(sf::Time timeLimit);

		/// @brief Trigger listeners, if expired
		/// @details This is the most important function of this class. You should call update() 
		/// every frame (or, the better way, let this an automated routine do) to assure that
		/// all associated listeners are invoked when the timer expires.
		void						update();
		
		/// @brief Registers a function which will be called when the time reaches zero.
		/// @details Make sure to call update() each frame to invoke potential listeners.
		/// @param listener The function you want to associate with the timer expiration.
		/// @return %Connection that can be used to disconnect the listener.
		Connection					connect(const Listener& listener);
			
		/// @brief Removes all currently associated timer listeners.
		/// @details This also invalidates all connections to those listeners.
		void						clearConnections();
	
	
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		typedef detail::ListenerSequence< CallbackTimer& >		ListenerCtr;
		

	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		ListenerCtr					mListeners;
		bool						mJustExpired;
};

/// @}

} // namespace thor

#endif // THOR_CALLBACKTIMER_HPP
