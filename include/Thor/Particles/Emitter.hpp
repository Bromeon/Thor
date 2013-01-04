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
/// @brief Concrete particle emitter classes

#ifndef THOR_EMITTER_HPP
#define THOR_EMITTER_HPP

#include <Thor/Particles/ParticleInterfaces.hpp>
#include <Thor/Math/Distribution.hpp>
#include <Thor/Config.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>


namespace thor
{

/// @addtogroup Particles
/// @{

/// @brief Class that emits particles with customizable initial conditions.
/// @details This emitter is universal with respect to the initial conditions of each emitted particle. It works with callbacks
///  that return initial values for the particle attributes (position, rotation, color, ...). So you can pass constants, random
///  distributions, or any functions that compute the value in an arbitrary way.
class THOR_API UniversalEmitter : public Emitter
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Shared pointer type referring to UniversalEmitter objects
		///
		typedef std::shared_ptr<UniversalEmitter> Ptr;

		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public static member functions
	public:
		/// @brief Creates a shared pointer to a UniversalEmitter.
		/// @copydetails UniversalEmitter::UniversalEmitter()
		static Ptr					create();

		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		///
									UniversalEmitter();

		virtual void				emit(Adder& system, sf::Time dt);

		/// @brief Sets the particle emission rate.
		/// @param particlesPerSecond How many particles are emitted in 1 second. The type is not integral to allow
		///  more flexibility (e.g. 0.5 yields one particle every 2 seconds).
		void						setEmissionRate(float particlesPerSecond);

		/// @brief Sets the lifetime (time between emission and death) of the particle.
		/// @param particleLifetime Constant or function returning the particle lifetime.
		void						setParticleLifetime(thor::Distribution<sf::Time> particleLifetime);

		/// @brief Sets the initial particle position.
		/// @param particlePosition Constant or function returning the initial particle position.
		void						setParticlePosition(thor::Distribution<sf::Vector2f> particlePosition);

		/// @brief Sets the initial particle velocity.
		/// @param particleVelocity Constant or function returning the initial particle velocity.
		void						setParticleVelocity(thor::Distribution<sf::Vector2f> particleVelocity);

		/// @brief Sets the initial particle rotation.
		/// @param particleRotation Constant or function returning the initial particle rotation.
		void						setParticleRotation(thor::Distribution<float> particleRotation);

		/// @brief Sets the initial particle rotation speed.
		/// @param particleRotationSpeed Constant or function returning the initial particle rotation speed.
		void						setParticleRotationSpeed(thor::Distribution<float> particleRotationSpeed);

		/// @brief Sets the initial particle scale.
		/// @param particleScale Constant or function returning the initial particle scale.
		void						setParticleScale(thor::Distribution<sf::Vector2f> particleScale);

		/// @brief Sets the initial particle color.
		/// @param particleColor Constant or function returning the initial particle color.
		void						setParticleColor(thor::Distribution<sf::Color> particleColor);

	
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		// Returns the number of particles to emit during this frame.
		unsigned int				computeParticleCount(sf::Time dt);

		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		float								mEmissionRate;
		float								mEmissionDifference;

		thor::Distribution<sf::Time>		mParticleLifetime;
		thor::Distribution<sf::Vector2f>	mParticlePosition;
		thor::Distribution<sf::Vector2f>	mParticleVelocity;
		thor::Distribution<float>			mParticleRotation;
		thor::Distribution<float>			mParticleRotationSpeed;
		thor::Distribution<sf::Vector2f>	mParticleScale;
		thor::Distribution<sf::Color>		mParticleColor;
};

/// @}

} // namespace thor

#endif // THOR_EMITTER_HPP
