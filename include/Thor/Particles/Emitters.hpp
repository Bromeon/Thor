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
/// @brief Concrete particle emitter classes

#ifndef THOR_EMITTER_HPP
#define THOR_EMITTER_HPP

#include <Thor/Math/Distribution.hpp>
#include <Thor/Config.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>


namespace thor
{

class EmissionInterface;


/// @addtogroup Particles
/// @{

/// @brief Creates a functor that references an emitter.
/// @param referenced Emitter functor to reference.
/// @details Use this function if you do not want to copy the emitter, but reference it, when you pass it to thor::ParticleSystem.
///  This allows you to modify the original object after it has been added, and effects are still visible. However, you are responsible
///  to ensure the lifetime of the referenced object. Example:
/// @code
/// // Create emitter and particle system
/// thor::UniversalEmitter emitter;
/// thor::ParticleSystem system(...);
///
/// // Add emitter to particle system
/// system.addEmitter(thor::refEmitter(emitter));
///
/// // Change emitter properties later
/// emitter.setEmissionRate(20);
/// @endcode
template <typename Emitter>
std::function<void(EmissionInterface&, sf::Time)> refEmitter(Emitter& referenced)
{
	return [&referenced] (EmissionInterface& system, sf::Time dt)
	{
		return referenced(system, dt);
	};
}


/// @brief Class that emits particles with customizable initial conditions.
/// @details This emitter is universal with respect to the initial conditions of each emitted particle. It works with callbacks
///  that return initial values for the particle attributes (position, rotation, color, ...). So you can pass constants, random
///  distributions, or any functions that compute the value in an arbitrary way. Have a look at the thor::Distributions namespace
///  for useful predefined functions.
class THOR_API UniversalEmitter
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		///
									UniversalEmitter();

		/// @brief Emits particles into a particle system.
		/// @param system Indirection to the particle system that stores the particles.
		/// @param dt Time interval during which particles are emitted.
		void						operator() (EmissionInterface& system, sf::Time dt);

		/// @brief Sets the particle emission rate.
		/// @param particlesPerSecond How many particles are emitted in 1 second. The type is not integral to allow
		///  more flexibility (e.g. 0.5 yields one particle every 2 seconds).
		void						setEmissionRate(float particlesPerSecond);

		/// @brief Sets the lifetime (time between emission and death) of the particle.
		/// @param particleLifetime Constant or function returning the particle lifetime.
		void						setParticleLifetime(Distribution<sf::Time> particleLifetime);

		/// @brief Sets the initial particle position.
		/// @param particlePosition Constant or function returning the initial particle position.
		void						setParticlePosition(Distribution<sf::Vector2f> particlePosition);

		/// @brief Sets the initial particle velocity.
		/// @param particleVelocity Constant or function returning the initial particle velocity.
		void						setParticleVelocity(Distribution<sf::Vector2f> particleVelocity);

		/// @brief Sets the initial particle rotation.
		/// @param particleRotation Constant or function returning the initial particle rotation.
		void						setParticleRotation(Distribution<float> particleRotation);

		/// @brief Sets the initial particle rotation speed.
		/// @param particleRotationSpeed Constant or function returning the initial particle rotation speed.
		void						setParticleRotationSpeed(Distribution<float> particleRotationSpeed);

		/// @brief Sets the initial particle scale.
		/// @param particleScale Constant or function returning the initial particle scale.
		void						setParticleScale(Distribution<sf::Vector2f> particleScale);

		/// @brief Sets the initial particle color.
		/// @param particleColor Constant or function returning the initial particle color.
		void						setParticleColor(Distribution<sf::Color> particleColor);

		/// @brief Sets the initial particle texture index.
		/// @param particleTextureIndex Constant or function returning the initial index of the particle's texture rectangle.
		void						setParticleTextureIndex(Distribution<unsigned int> particleTextureIndex);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		// Returns the number of particles to emit during this frame.
		std::size_t					computeParticleCount(sf::Time dt);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		float						mEmissionRate;
		float						mEmissionDifference;

		Distribution<sf::Time>		mParticleLifetime;
		Distribution<sf::Vector2f>	mParticlePosition;
		Distribution<sf::Vector2f>	mParticleVelocity;
		Distribution<float>			mParticleRotation;
		Distribution<float>			mParticleRotationSpeed;
		Distribution<sf::Vector2f>	mParticleScale;
		Distribution<sf::Color>		mParticleColor;
		Distribution<unsigned int>	mParticleTextureIndex;
};

/// @}

} // namespace thor

#endif // THOR_EMITTER_HPP
