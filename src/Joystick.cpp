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

#include <Thor/Input/Joystick.hpp>


namespace thor
{

JoystickButton::JoystickButton(unsigned int joystickId, unsigned int button)
: joystickId(joystickId)
, button(button)
{
}

JoystickAxis::JoystickAxis(unsigned int joystickId, sf::Joystick::Axis axis, float threshold, bool above)
: joystickId(joystickId)
, axis(axis)
, threshold(threshold)
, above(above)
{
}

detail::JoyBuilder joy(unsigned int joystickId)
{
	return detail::JoyBuilder(joystickId);
}

// ---------------------------------------------------------------------------------------------------------------------------


namespace detail
{

	JoystickAxis JoyBuilder::Axis::above(float threshold)
	{
		return JoystickAxis(joystickId, axis, threshold, true);
	}

	JoystickAxis JoyBuilder::Axis::below(float threshold)
	{
		return JoystickAxis(joystickId, axis, threshold, false);
	}

	JoyBuilder::JoyBuilder(unsigned int joystickId)
	: joystickId(joystickId)
	{
	}

	JoystickButton JoyBuilder::button(unsigned int button)
	{
		return JoystickButton(joystickId, button);
	}

	JoyBuilder::Axis JoyBuilder::axis(sf::Joystick::Axis axis)
	{
		Axis a;
		a.joystickId = joystickId;
		a.axis = axis;
		return a;
	}

} // namespace detail
} // namespace thor