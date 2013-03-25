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

/// @brief Small wrapper class for joystick number and button number
///
struct Joystick
{
	/// @brief Constructor
	/// @details Note that you can also construct a joystick id and button property
	///  with the following more expressive syntax:
	/// @code
	/// thor::Joystick j = thor::Joy(id).Button(button);
	/// @endcode
	Joystick(unsigned int id, unsigned int button)
	: id(id)
	, button(button)
	{
	}

	unsigned int id;		///< The joystick number
	unsigned int button;	///< The joystick button number
};

/// @}

// Proxy class that allows the Joy(id).Button(button) syntax
struct Joy
{
	explicit Joy(unsigned int id)
	: id(id)
	{
	}

	Joystick button(unsigned int button)
	{
		return Joystick(id, button);
	}

	unsigned int id;
};


} // namespace thor

#endif // THOR_JOYSTICK_HPP
