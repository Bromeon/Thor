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
/// @brief Concrete particle affector classes

#ifndef THOR_AFFECTOR_HPP
#define THOR_AFFECTOR_HPP

#include <Thor/Config.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <functional>


namespace thor
{

class Particle;

/// @addtogroup Particles
/// @{

/// @brief Creates a functor that references an affector.
/// @param referenced Affector functor to reference.
/// @details Use this function if you do not want to copy the affector, but reference it, when you pass it to thor::ParticleSystem.
///  This allows you to modify the original object after it has been added, and effects are still visible. However, you are responsible
///  to ensure the lifetime of the referenced object. Example:
/// @code
/// // Create affector and particle system
/// thor::ForceAffector affector(acceleration);
/// thor::ParticleSystem system(...);
///
/// // Add affector to particle system
/// system.addAffector(thor::refAffector(affector));
///
/// // Change affector properties later
/// affector = ForceAffector(newAcceleration);
/// @endcode
template <typename Affector>
std::function<void(Particle&, sf::Time)> refAffector(Affector& referenced)
{
	return [&referenced] (Particle& particle, sf::Time dt)
	{
		return referenced(particle, dt);
	};
}


/// @brief Applies a translational acceleration to particles over time.
/// @details Affector class that applies an acceleration vector to each particle. A popular use case is gravity.
class THOR_API ForceAffector
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param acceleration The acceleration vector. The particle's velocity changes by this vector each second.
		explicit					ForceAffector(sf::Vector2f acceleration);

		/// @brief Affects particles.
		/// @param particle The particle currently being affected.
		/// @param dt Time interval during which particles are affected.
		void						operator() (Particle& particle, sf::Time dt);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		sf::Vector2f				mAcceleration;
};


/// @brief Applies a rotational acceleration to particles over time.
/// @details Affector class that applies a scalar angular acceleration value to each particle.
class THOR_API TorqueAffector
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param angularAcceleration The angular acceleration, in degrees. The particle's rotation speed changes by
		///  this value each second.
		explicit					TorqueAffector(float angularAcceleration);

		/// @copydoc ForceAffector::operator()()
		///
		void						operator() (Particle& particle, sf::Time dt);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		float						mAngularAcceleration;
};


/// @brief Scales particles over time.
/// @details
class THOR_API ScaleAffector
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param scaleFactor Factor by which particles are scaled every second.
		explicit					ScaleAffector(sf::Vector2f scaleFactor);

		/// @copydoc ForceAffector::operator()()
		///
		void						operator() (Particle& particle, sf::Time dt);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		sf::Vector2f				mScaleFactor;
};


/// @brief %Affector that animates particles using a function.
/// @details This affector can be used to apply animations of Thor's Animations module to particles. Such animations are described
///  by a function with signature <b>void(Particle& animated, float progress)</b>.
class THOR_API AnimationAffector
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @details Applies an animation during the whole lifetime of the particles.
		/// @param particleAnimation An animation function that is applied to the particle. Its second parameter @a progress
		///  corresponds to getElapsedRatio(particle), the delta time of operator() is ignored.
		explicit					AnimationAffector(std::function<void(Particle&, float)> particleAnimation);

		/// @copydoc ForceAffector::operator()()
		///
		void						operator() (Particle& particle, sf::Time dt);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::function<void(Particle&, float)>	mAnimation;
};

/// @}

} // namespace thor

#endif // THOR_AFFECTOR_HPP
