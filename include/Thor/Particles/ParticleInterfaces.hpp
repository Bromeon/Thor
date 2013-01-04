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
/// @brief Interfaces thor::Emitter, thor::Affector

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

/// @brief Abstract base class for particle affectors.
/// @details Affectors are classes that influence emitted particles over time.
/// @n Inherit from this class and override affect() to implement custom affectors.
class THOR_API Affector
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Shared pointer type referring to derivates of Affector
		///
		typedef std::shared_ptr<Affector>	Ptr;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Virtual destructor	
		///
		virtual						~Affector();

		/// @brief Affects particles.
		/// @param particle The particle currently being affected.
		/// @param dt Time interval during which particles are affected.
		virtual void				affect(Particle& particle, sf::Time dt) = 0; 
};


/// @brief Abstract base class for particle emitters.
/// @details Emitters are classes which create particles (using particular initial conditions) and insert them into a particle system.
/// @n Inherit from this class and override emit() to implement custom emitters.
class THOR_API Emitter
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Shared pointer type referring to derivates of Emitter
		///
		typedef std::shared_ptr<Emitter>	Ptr;

		/// @brief Class that connects emitters with their corresponding particle system.
		/// @details Provides a virtual method that adds particles to the system.
		struct THOR_API Adder
		{
			/// @brief Virtual destructor
			///
			virtual						~Adder() {}

			/// @brief Adds a particle to the system.
			/// @param particle Particle to add.
			virtual void				addParticle(const Particle& particle) = 0;
		};

	
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Virtual destructor
		///
		virtual						~Emitter();

		/// @brief Emits particles into a particle system.
		/// @details Override this method in your emitter class to implement your own functionality. If your emitter
		///  does only emit the particles in a different area, you should have a look at RandomOffset().
		/// @param system Indirection to the particle system that stores the particles.
		/// @param dt Time interval during which particles are emitted.
		virtual void				emit(Adder& system, sf::Time dt) = 0;
};

/// @}

} // namespace thor

#endif // THOR_PARTICLEINTERFACES_HPP
