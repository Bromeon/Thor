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
/// @brief Functions to convert SFML keyboard/mouse enumerators from and to strings

#ifndef THOR_INPUTNAMES_HPP
#define THOR_INPUTNAMES_HPP

#include <Thor/Config.hpp>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Joystick.hpp>

#include <string>


namespace thor
{

/// @addtogroup Input
/// @{

/// @brief Returns a string representation of @a key.
/// @details The returned string is identical to the enum identifier. For example:
/// @code
/// thor::toString(sf::Keyboard::BackSlash) == "BackSlash"
/// @endcode
/// @throw StringConversionException in case of invalid key.
std::string THOR_API		toString(sf::Keyboard::Key key);

/// @brief Returns a string representation of @a button.
/// @details The returned string is identical to the enum identifier. For example:
/// @code
/// thor::toString(sf::Mouse::Right) == "Right"
/// @endcode
/// @throw StringConversionException in case of invalid mouse button.
std::string THOR_API		toString(sf::Mouse::Button button);

/// @brief Returns a string representation of @a axis.
/// @details The returned string is identical to the enum identifier. For example:
/// @code
/// thor::toString(sf::Joystick::U) == "U"
/// @endcode
/// @throw StringConversionException in case of invalid joystick axis.
std::string THOR_API		toString(sf::Joystick::Axis axis);

/// @brief Returns the key interpretation of @a string.
/// @details The passed string shall be identical to the enum identifier. For example:
/// @code
/// thor::toKeyboardKey("BackSlash") == sf::Keyboard::BackSlash
/// @endcode
/// @throw StringConversionException if the string doesn't match a key.
sf::Keyboard::Key THOR_API	toKeyboardKey(const std::string& string);

/// @brief Returns the mouse button interpretation of @a string.
/// @details The passed string shall be identical to the enum identifier. For example:
/// @code
/// thor::toMouseButton("Right") == sf::Mouse::Right
/// @endcode
/// @throw StringConversionException if the string doesn't match a mouse button.
sf::Mouse::Button THOR_API	toMouseButton(const std::string& string);

/// @brief Returns the joystick axis interpretation of @a string.
/// @details The passed string shall be identical to the enum identifier. For example:
/// @code
/// thor::toJoystickAxis("U") == sf::Joystick::U
/// @endcode
/// @throw StringConversionException if the string doesn't match a joystick axis.
sf::Joystick::Axis THOR_API	toJoystickAxis(const std::string& string);

/// @}

} // namespace thor

#endif // THOR_INPUTNAMES_HPP
