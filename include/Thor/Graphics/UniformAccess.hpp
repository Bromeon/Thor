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

/// @file
/// @brief Global functions to access SFML and Thor objects uniformly

#ifndef THOR_UNIFORMACCESS_HPP
#define THOR_UNIFORMACCESS_HPP

#include <Thor/Config.hpp>

#include <SFML/Graphics/Color.hpp>


namespace thor
{

class Particle;

/// @addtogroup Graphics
/// @{

/// @brief Sets the color of a graphical object.
///
template <typename T>
void						setColor(T& object, const sf::Color& color);

/// @brief Sets the color of a particle.
/// 
void THOR_API				setColor(Particle& particle, const sf::Color& color);

/// @brief Sets the alpha color value of a graphical object.
/// @details The object shall support the methods @a getColor() and @a setColor().
template <typename T>
void						setAlpha(T& object, sf::Uint8 alpha);

/// @brief Sets the alpha color value of a particle.
///
void THOR_API				setAlpha(Particle& particle, sf::Uint8 alpha);

/// @}

} // namespace thor

#include <Thor/Graphics/Detail/UniformAccess.inl>
#endif // THOR_UNIFORMACCESS_HPP
