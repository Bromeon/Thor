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
/// @brief Class thor::EmissionInterface

#ifndef THOR_EMISSIONINTERFACE_HPP
#define THOR_EMISSIONINTERFACE_HPP

#include <Thor/Config.hpp>


namespace thor
{

class Particle;


/// @addtogroup Particles
/// @{

/// @brief Class that connects emitters with their corresponding particle system.
/// @details This class acts as an interface from emitters to particle systems. A single method to emit particles is provided.
class THOR_API EmissionInterface
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Virtual destructor
		///
		virtual						~EmissionInterface() {}

		/// @brief Emits a particle into the particle system.
		/// @param particle Particle to emit.
		virtual void				emitParticle(const Particle& particle) = 0;
};

/// @}

} // namespace thor

#endif // THOR_EMISSIONINTERFACE_HPP
