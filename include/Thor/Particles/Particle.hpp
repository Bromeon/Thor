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
/// @brief Structure thor::Particle

#ifndef THOR_PARTICLE_HPP
#define THOR_PARTICLE_HPP

#include <Thor/Config.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>


namespace thor
{

/// @addtogroup Particles
/// @{

/// @brief %Particle class
/// @details This class represents single particles of a particle system. It stores properties like position, rotation,
///  scale, movement, color and life time.
/// @n You normally don't have to work directly with particles outside of an affector or emitter.
class THOR_API Particle
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param totalLifetime How long the particle totally exists.
		explicit					Particle(sf::Time totalLifetime);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public variables
	public:
		sf::Vector2f				position;			///< Current position.
		sf::Vector2f				velocity;			///< Velocity (change in position per second).
		float						rotation;			///< Current rotation angle.
		float						rotationSpeed;		///< Angular velocity (change in rotation per second).
		sf::Vector2f				scale;				///< Scale, where (1,1) represents the original size.
		sf::Color					color;				///< %Particle color.
		unsigned int				textureIndex;		///< Index of the used texture rect, returned by ParticleSystem::addTextureRect()


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		sf::Time					passedLifetime;		// Time passed since emitted.
		sf::Time					totalLifetime;		// Total time to live.


	// ---------------------------------------------------------------------------------------------------------------------------
	// Friends
	/// @cond FriendsAreAnImplementationDetail
	friend class ParticleSystem;
	friend sf::Time THOR_API getElapsedLifetime(const Particle& particle);
	friend sf::Time THOR_API getTotalLifetime(const Particle& particle);
	friend void THOR_API abandonParticle(Particle& particle);
	/// @endcond
};

/// @relates Particle
/// @brief Returns the time passed since the particle has been emitted.
sf::Time THOR_API			getElapsedLifetime(const Particle& particle);

/// @relates Particle
/// @brief Returns the total time the particle is alive.
sf::Time THOR_API			getTotalLifetime(const Particle& particle);

/// @relates Particle
/// @brief Returns the time left until the particle dies.
sf::Time THOR_API			getRemainingLifetime(const Particle& particle);

/// @relates Particle
/// @brief Returns <b>elapsed lifetime / total lifetime</b>.
float THOR_API				getElapsedRatio(const Particle& particle);

/// @relates Particle
/// @brief Returns <b>remaining lifetime / total lifetime</b>.
float THOR_API				getRemainingRatio(const Particle& particle);

/// @relates Particle
/// @brief Marks a particle for removal.
/// @details This function can be used when the lifetime of particles is not bound to time, but another condition.
///  For example, in a physics simulation, particles may disappear as soon as they hit an object. Calling this function
///  will set the particle's elapsed time to the total lifetime, and the next update of the particle system will remove it.
void THOR_API				abandonParticle(Particle& particle);

/// @}

} // namespace thor

#endif // THOR_PARTICLE_HPP
