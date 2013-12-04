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
/// @brief Class thor::Joystick

#ifndef THOR_JOYSTICK_HPP
#define THOR_JOYSTICK_HPP


namespace thor
{

/// @addtogroup Input
/// @{

/// @brief Contains information about a joystick number and button number
///
struct JoystickButton
{
	/// @brief Constructor
	/// @details Note that you can also construct a joystick id and button property
	///  with the following more expressive syntax:
	/// @code
	/// thor::JoystickButton j = thor::joy(id).button(b);
	/// @endcode
	JoystickButton(unsigned int id, unsigned int button)
	: id(id)
	, button(button)
	{
	}

	unsigned int		id;			///< The joystick number
	unsigned int		button;		///< The joystick button number
};

/// @}


namespace detail
{

	// Proxy class that allows the Joy(id).button(button) syntax
	struct JoyBuilder
	{
		explicit JoyBuilder(unsigned int id)
		: id(id)
		{
		}

		JoystickButton button(unsigned int button)
		{
			return JoystickButton(id, button);
		}

		unsigned int id;
	};

} // namespace detail

inline detail::JoyBuilder joy(unsigned int joystickId)
{
	return detail::JoyBuilder(joystickId);
}

} // namespace thor

#endif // THOR_JOYSTICK_HPP
