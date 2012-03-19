/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2012 Jan Haller
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

#include <Thor/Geometry/Zone.hpp>
#include <Thor/Config.hpp>

#include <Aurora/SmartPtr/CopiedPtr.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include AURORA_TR1_HEADER(memory)


namespace thor
{

class Particle;
class Zone;


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
		typedef std::tr1::shared_ptr<Affector>	Ptr;


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
		typedef std::tr1::shared_ptr<Emitter>	Ptr;


		/// @brief Copyable smart pointer to Zone derivates
		/// @details Is used to pass polymorphic zones around. Note that this pointer doesn't have shared ownership semantics, it
		///  rather duplicates ownership when copied.
		typedef aur::CopiedPtr<Zone>			ZonePtr;


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

		/// @brief Sets the zone inside which particles are created.
		/// @param zone MovedPtr<Zone, VirtualClone> to concrete zone object.
		void						setEmissionZone(Emitter::ZonePtr zone);

		/// @brief Returns the zone inside which particles are created.
		/// @return Reference to modifiable zone, allowing changes of it.
		Zone&						getEmissionZone();

		/// @brief Returns the zone inside which particles are created (const overload).
		/// @return Reference to constant zone.
		const Zone&					getEmissionZone() const;

		/// @brief Sets the particle emission rate.
		/// @param particlesPerSecond How many particles are emitted in 1 second. The type is not integral to allow
		///  more flexibility (e.g. 0.5 yields one particle every 2 seconds).
		void						setEmissionRate(float particlesPerSecond);

		/// @brief Returns the particle emission rate.
		/// @return How many particles are emitted in 1 second. The type is not integral to allow
		///  more flexibility (e.g. 0.5 yields one particle every 2 seconds).
		float						getEmissionRate() const;

		/// @brief Sets the initial particle scale.
		/// 
		void						setParticleScale(sf::Vector2f scale);

		/// @brief Returns the initial particle velocity.
		/// 
		sf::Vector2f				getParticleScale() const;

		/// @brief Sets the initial particle color.
		/// 
		void						setParticleColor(const sf::Color& color);

		/// @brief Returns the initial particle color.
		/// 
		const sf::Color&			getParticleColor() const;

		/// @brief sets the lifetime (time between emission and death) of the particle.
		///
		void						setParticleLifetime(sf::Time lifetime);

		/// @brief Returns the lifetime (time between emission and death) of the particle.
		/// 
		sf::Time					getParticleLifetime() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Protected member functions
	protected:
		/// @brief Constructor
		/// @param particlesPerSecond How many particles are emitted in 1 second. The type is not integral to allow
		///  more flexibility (e.g. 0.5 yields one particle every 2 seconds).
		/// @param particleLifetime How long the particles live until they are removed.
									Emitter(float particlesPerSecond, sf::Time particleLifetime);

		/// @brief Helper function for emission: Computes how many particles should be emitted in this frame.
		/// @details Saves the user from calculations, takes care of too short times to emit particles. This function
		///  shall be called exactly once each frame.
		/// @param dt Frame time.
		/// @return Number of particles to emit.
		unsigned int				computeNbParticles(sf::Time dt);

		/// @brief Creates a prototype of a particle.
		/// @details Applies the initial particle settings (position, rotation, scale, color, lifetime).
		Particle					createParticlePrototype() const;

	
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		aur::CopiedPtr<Zone>		mEmissionZone;
		float						mEmissionRate;
		float						mEmissionDifference;

		sf::Time					mParticleLifetime;
		sf::Vector2f				mParticleScale;
		sf::Color					mParticleColor;
};

/// @}

} // namespace thor

#endif // THOR_PARTICLEINTERFACES_HPP
