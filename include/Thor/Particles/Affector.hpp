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
/// @brief Concrete particle affector classes

#ifndef THOR_AFFECTOR_HPP
#define THOR_AFFECTOR_HPP

#include <Thor/Particles/ParticleInterfaces.hpp>
#include <Thor/Graphics/Colors.hpp>
#include <Thor/Config.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>


namespace thor
{

/// @addtogroup Particles
/// @{

/// @brief Applies a translational acceleration to particles over time.
/// @details Affector class that applies an acceleration vector to each particle. A popular use case is gravity.
class THOR_API ForceAffector : public Affector
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Shared pointer type referring to ForceAffector objects
		///
		typedef std::shared_ptr<ForceAffector> Ptr;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public static member functions
	public:
		/// @brief Creates an affector that accelerates particles continuously.
		/// @copydetails ForceAffector::ForceAffector()
		static Ptr					create(sf::Vector2f acceleration);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param acceleration The acceleration vector. The particle's velocity changes by this vector each second.
		explicit					ForceAffector(sf::Vector2f acceleration);

		virtual void				affect(Particle& particle, sf::Time dt);

		/// @brief Sets the linear acceleration applied to the particles.
		///
		void						setAcceleration(sf::Vector2f acceleration);

		/// @brief Returns the linear acceleration applied to the particles.
		///
		sf::Vector2f				getAcceleration() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		sf::Vector2f				mAcceleration;
};


/// @brief Applies a rotational acceleration to particles over time.
/// @details Affector class that applies a scalar angular acceleration value to each particle.
class THOR_API TorqueAffector : public Affector
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Shared pointer type referring to TorqueAffector objects
		///
		typedef std::shared_ptr<TorqueAffector> Ptr;

		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public static member functions
	public:
		/// @brief Creates an affector that applies an angular acceleration to particles.
		/// @copydetails TorqueAffector::TorqueAffector()
		static Ptr 					create(float angularAcceleration);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param angularAcceleration The angular acceleration, in degrees. The particle's rotation speed changes by
		///  this value each second.
		explicit					TorqueAffector(float angularAcceleration);

		virtual void				affect(Particle& particle, sf::Time dt);

		/// @brief sets the angular acceleration applied to the particles (in degrees).
		///
		void						setAngularAcceleration(float angularAcceleration);

		/// @brief Returns the angular acceleration applied to the particles (in degrees).
		///
		float						getAngularAcceleration() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		float						mAngularAcceleration;
};


/// @brief Scales particles over time.
/// @details 
class THOR_API ScaleAffector : public Affector
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Shared pointer type referring to ScaleAffector objects
		///
		typedef std::shared_ptr<ScaleAffector> Ptr;
		
		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public static member functions
	public:
		/// @brief Creates an affector that continuously scales particles by a given factor.
		/// @copydetails ScaleAffector::ScaleAffector()
		static Ptr					create(sf::Vector2f scaleFactor);

		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param scaleFactor Factor by which particles are scaled every second.
		explicit					ScaleAffector(sf::Vector2f scaleFactor);

		virtual void				affect(Particle& particle, sf::Time dt);

		/// @brief Sets the factor by which particles are scaled every second.
		/// 
		void						setScaleFactor(sf::Vector2f scaleFactor);

		/// @brief Returns the factor by which particles are scaled every second.
		/// 
		sf::Vector2f				getScaleFactor() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		sf::Vector2f				mScaleFactor;
};


/// @brief Fades particles in over time.
/// @details Affector class that constantly increases the alpha color value of a particle.
class THOR_API FadeInAffector : public Affector
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Shared pointer type referring to FadeInAffector objects
		///
		typedef std::shared_ptr<FadeInAffector> Ptr;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public static member functions
	public:
		/// @brief Creates an affector that increases the alpha value from the beginning of the particle lifetime.
		/// @copydetails FadeInAffector::FadeInAffector()
		static Ptr 					create(float timeRatio = 1.f);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param timeRatio How long the fade-in time is in comparison to the total particle lifetime. For example,
		///  1.f means over the whole lifetime, and 0.25f means to fade in the particle only in the first quarter.
		explicit					FadeInAffector(float timeRatio = 1.f);

		virtual void				affect(Particle& particle, sf::Time dt);

		/// @brief sets the time ratio (how long the fade-out time is compared to the particle lifetime).
		/// 
		void						setTimeRatio(float timeRatio);
	
		/// @brief Returns the time ratio (how long the fade-out time is compared to the particle lifetime).
		/// 
		float						getTimeRatio() const;

	
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		float						mTimeRatio;
};


/// @brief Fades particles out over time.
/// @details Affector class that constantly decreases the alpha color value of a particle.
class THOR_API FadeOutAffector : public Affector
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Shared pointer type referring to FadeOutAffector objects
		///
		typedef std::shared_ptr<FadeOutAffector> Ptr;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public static member functions
	public:		
		/// @brief Creates an affector that decreases the alpha value towards the end of the particle lifetime.
		/// @copydetails FadeOutAffector::FadeOutAffector()
		static Ptr 					create(float timeRatio = 1.f);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param timeRatio How long the fade-out time is in comparison to the total particle lifetime. For example,
		///  1.f means over the whole lifetime, and 0.25f means to fade out the particle only in the last quarter.
		explicit					FadeOutAffector(float timeRatio = 1.f);

		virtual void				affect(Particle& particle, sf::Time dt);

		/// @brief sets the time ratio (how long the fade-out time is compared to the particle lifetime).
		/// 
		void						setTimeRatio(float timeRatio);
	
		/// @brief Returns the time ratio (how long the fade-out time is compared to the particle lifetime).
		/// 
		float						getTimeRatio() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		float						mTimeRatio;
};


/// @brief Applies a color gradient to particles.
/// @details Affector class that changes the color of a particle smoothly from one to another.
class THOR_API ColorAffector : public Affector
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Shared pointer type referring to ColorAffector objects
		///
		typedef std::shared_ptr<ColorAffector> Ptr;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public static member functions
	public:
		/// @brief Creates an affector that colorizes particles according to a given gradient.
		/// @copydetails ColorAffector::ColorAffector(const ColorGradient&)
		static Ptr 					create(const ColorGradient& gradient);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @details Creates a color gradient over the whole lifetime of the particles.
		/// @param gradient The color gradient affecting the particles. Can also be a single sf::Color.
		explicit					ColorAffector(const ColorGradient& gradient);

		virtual void				affect(Particle& particle, sf::Time dt);

		/// @brief Sets the color gradient used to colorize the particles.
		/// 
		void						setGradient(const ColorGradient& gradient);

		/// @brief Returns the color gradient used to colorize the particles.
		/// 
		const ColorGradient&		getGradient() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		ColorGradient				mGradient;
};

/// @}

} // namespace thor

#endif // THOR_AFFECTOR_HPP
