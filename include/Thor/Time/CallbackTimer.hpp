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
/// @details Clock class that counts time down. As an extension of Timer, this class is able to register functions that are
/// called at expiration time. When you use this class, make sure you invoke the update() method every frame. Possible usage:
/// @code
/// #include <Thor/Time/CallbackTimer.hpp>
/// #include <iostream>
///
/// void listener(thor::CallbackTimer& trigger)
/// {
///     std::cout << "expired" << std::endl;
///     trigger.restart(sf::seconds(1.f));
/// }
///
/// int main()
/// {
///     thor::CallbackTimer timer;
///     timer.connect(&listener);
///     timer.restart(sf::seconds(1.f));
///
///     for (;;)
///         timer.update();
/// }
/// @endcode
class THOR_API CallbackTimer : public Timer
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor: Creates a callback timer that is initially expired.
		/// @details You have to call reset() or restart() before you can use the countdown functionality.
									CallbackTimer();

		virtual void				reset(sf::Time timeLimit);

		virtual void				restart(sf::Time timeLimit);

		/// @brief Trigger listeners, if expired.
		/// @details This is the most important function of this class. You should call update() every frame (or, even better,
		/// let this an automated routine do) to assure that all associated listeners are invoked when the timer expires.
		/// If you restart the timer, call update() before restarting; otherwise the listeners won't fire.
		void						update();

		/// @brief Registers a unary function which will be called when the time reaches zero.
		/// @details Make sure to call update() each frame to invoke potential listeners. Use this function if your callback
		///  should get the timer as a parameter.
		/// @param unaryListener The function you want to associate with the timer expiration. The first parameter is a reference
		///  to the CallbackTimer instance that just expired.
		/// @return %Connection that can be used to disconnect the listener.
		/// @warning Inside a callback, you may not modify the connections of the timer that invokes it. Insertion and removal of
		///  callbacks need to be postponed after the update() call.
		Connection					connect(std::function<void(CallbackTimer&)> unaryListener);

		/// @brief Registers a nullary function which will be called when the time reaches zero.
		/// @details Make sure to call update() each frame to invoke potential listeners. Use this function if you don't care
		///  about the timer that fired the callback, and thus don't need a parameter for it in your callback.
		/// @param nullaryListener The function you want to associate with the timer expiration.
		/// @return %Connection that can be used to disconnect the listener.
		/// @warning Inside a callback, you may not modify the connections of the timer that invokes it. Insertion and removal of
		///  callbacks need to be postponed after the update() call.
		Connection					connect0(std::function<void()> nullaryListener);

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
