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
/// @brief Class thor::EmissionAdder

#ifndef THOR_PARTICLEINTERFACES_HPP
#define THOR_PARTICLEINTERFACES_HPP

#include <Thor/Config.hpp>

#include <memory>

#include <SFML/System/Time.hpp>


namespace thor
{

class Particle;


/// @addtogroup Particles
/// @{

/// @brief Class that connects emitters with their corresponding particle system.
/// @details Provides a virtual method that adds particles to the system.
class THOR_API EmissionAdder
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Virtual destructor
		///
		virtual						~EmissionAdder() {}

		/// @brief Adds a particle to the system.
		/// @param particle Particle to add.
		virtual void				addParticle(const Particle& particle) = 0;
};

/// @}

} // namespace thor

#endif // THOR_PARTICLEINTERFACES_HPP
