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
/// @brief Concrete particle emitter classes

#ifndef THOR_EMITTER_HPP
#define THOR_EMITTER_HPP

#include <Thor/Particles/ParticleInterfaces.hpp>
#include <Thor/Particles/Particle.hpp>
#include <Thor/Vectors/PolarVector.hpp>
#include <Thor/Geometry/Zone.hpp>
#include <Thor/Config.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>


namespace thor
{

/// @addtogroup Particles
/// @{

/// @brief Class that emits particles in a given direction.
/// @details This direction is the initial particle velocity.
class THOR_API DirectionalEmitter : public Emitter
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Shared pointer type referring to DirectionalEmitter objects
		///
		typedef std::tr1::shared_ptr<DirectionalEmitter> Ptr;

		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public static member functions
	public:
		/// @brief Creates an emitter that emits particles mainly in one direction.
		/// @copydetails DirectionalEmitter::DirectionalEmitter()
		static Ptr					create(float particlesPerSecond, sf::Time particleLifetime);

		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param particlesPerSecond How many particles are emitted in 1 second. The type is not integral to allow
		///  more flexibility (e.g. 0.5 yields one particle every 2 seconds).
		/// @param particleLifetime How long the particles live until they are removed.
									DirectionalEmitter(float particlesPerSecond, sf::Time particleLifetime);

		virtual void				emit(Adder& system, sf::Time dt);
		
		/// @brief Sets the initial particle velocity.
		/// @details Note that the emission angle influences the actual direction of particles.
		void						setParticleVelocity(sf::Vector2f velocity);

		/// @brief Returns the initial particle velocity.
		/// 
		sf::Vector2f				getParticleVelocity() const;

		/// @brief Sets the particle emission angle.
		/// @param emissionAngle The difference angle of the cone towards which particles are emitted, in degrees.
		///  For example, 0 means a straight line and 180 the half space.
		void						setEmissionAngle(float emissionAngle);

		/// @brief Returns the particle emission angle.
		/// @return The difference angle of the cone towards which particles are emitted, in degrees.
		///  For example, 0 means a straight line and 180 the half space.
		float						getEmissionAngle() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		float						mEmissionAngle;
		sf::Vector2f				mParticleVelocity;
};


/// @brief Emits particles towards a specified target zone.
/// @details 
class THOR_API TargetEmitter : public Emitter
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Shared pointer type referring to TargetEmitter objects
		///
		typedef std::tr1::shared_ptr<TargetEmitter> Ptr;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public static member functions
	public:
		/// @brief Creates an emitter that emits particles towards a target zone.
		/// @copydetails TargetEmitter::TargetEmitter()
		static Ptr					create(float particlesPerSecond, sf::Time particleLifetime);

		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param particlesPerSecond How many particles are emitted in 1 second. The type is not integral to allow
		///  more flexibility (e.g. 0.5 yields one particle every 2 seconds).
		/// @param particleLifetime How long the particles live until they are removed.
									TargetEmitter(float particlesPerSecond, sf::Time particleLifetime);

		virtual void				emit(Adder& system, sf::Time dt);

		/// @brief Sets the zone towards which particles are emitted.
		/// @param zone MovedPtr<Zone, VirtualClone> to concrete zone object. Every time a particle is emitted, a random point in
		///  the target zone is chosen, and the particle is accelerated towards that point.
		void						setTargetZone(Emitter::ZonePtr zone);

		/// @brief Returns the zone towards which the particles are emitted.
		/// @return Reference to modifiable zone, allowing changes of it.
		Zone&						getTargetZone();

		/// @brief Returns the zone towards which the particles are emitted (const overload).
		/// @return Reference to constant zone.
		const Zone&					getTargetZone() const;

		/// @brief Sets the speed of the emitted particles.
		/// @param speed Particle speed. If this value is negative, the particles are emitted @a away from the target zone (that
		///  is, the target zone works repulsively).
		void						setParticleSpeed(float speed);

		/// @brief Returns the speed of the emitted particles.
		/// @return Particle speed. If this value is negative, the particles are emitted @a away from the target zone (that
		///  is, the target zone works repulsively).
		float						getParticleSpeed() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		aur::CopiedPtr<Zone>		mTargetZone;
		float						mParticleSpeed;
};

/// @}

} // namespace thor

#endif // THOR_EMITTER_HPP
