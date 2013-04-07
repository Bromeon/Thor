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
/// @brief Class thor::ParticleSystem

#ifndef THOR_PARTICLESYSTEM_HPP
#define THOR_PARTICLESYSTEM_HPP

#include <Thor/Particles/ParticleInterfaces.hpp>
#include <Thor/Particles/Particle.hpp>
#include <Thor/Config.hpp>

#include <Aurora/Tools/Swap.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <vector>
#include <utility>


namespace sf
{

	class RenderWindow;
	class Image;
	class Texture;

} // namespace sf


namespace thor
{

/// @addtogroup Particles
/// @{

/// @brief Class for simple particle systems.
/// @details Like sprites, particles are represented as sub-rectangles of sf::Texture. During their
///  lifetime, the particles can be affected in translation, rotation, scale and coloring.
/// @n@n This class is noncopyable.
class THOR_API ParticleSystem : public sf::Drawable, private sf::NonCopyable, private Emitter::Adder
{		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		// Container typedefs
		typedef std::vector< Particle >										ParticleContainer;
		typedef std::vector< std::pair<Affector::Ptr, sf::Time> >			AffectorContainer;
		typedef std::vector< std::pair<Emitter::Ptr, sf::Time> >			EmitterContainer;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor: Create particle system from a whole sf::Texture
		/// @param texture Reference to the sf::Texture used as particle texture. The texture must remain valid as long as the
		///  particle system uses it.
		explicit					ParticleSystem(const sf::Texture& texture);

		/// @brief Constructor: Create particle system using parts of a sf::Texture
		/// @param texture Reference to the sf::Texture used as particle texture. The texture must remain valid as long as the
		///  particle system uses it.
		/// @param textureRect Area of the texture that is used to draw the particle.
									ParticleSystem(const sf::Texture& texture, const sf::IntRect& textureRect);

		/// @brief Swaps the contents of two instances in constant time.
		///
		void						swap(ParticleSystem& other);
				
		/// @brief Adds a particle affector to the system.
		/// @details Be aware that multiple affectors can interfere with each other. The affectors are applied in the order they were
		///  added to the system, therefore affectors at the end may overwrite particle states set by earlier affectors. To completely
		///  avoid the issue, only add orthogonal affectors (e.g. one for color, one for acceleration...).
		/// @param affector Shared pointer to a derivate of Affector (non-empty).
		/// @pre @a affector has not been added yet.
		void						addAffector(Affector::Ptr affector);

		/// @brief Adds a particle affector for a certain amount of time.
		/// @details Be aware that multiple affectors can interfere with each other. The affectors are applied in the order they were
		///  added to the system, therefore affectors at the end may overwrite particle states set by earlier affectors. To completely
		///  avoid the issue, only add orthogonal affectors (e.g. one for color, one for acceleration...).
		/// @param affector Shared pointer to a derivate of Affector (non-empty).
		/// @param timeUntilRemoval Time after which the affector is automatically removed from the system.
		/// @pre @a affector has not been added yet.
		void						addAffector(Affector::Ptr affector, sf::Time timeUntilRemoval);

		/// @brief Removes a particle affector from the system.
		/// @param affector Shared pointer to a derivate of Affector (non-empty).
		/// @pre @a affector is currently stored in the particle system.
		void						removeAffector(Affector::Ptr affector);

		/// @brief Removes all %Affector instances from the system.
		/// @details All particles lose the influence of any extern affectors. Movement and lifetime
		///  is still computed.
		void						clearAffectors();

		/// @brief Checks whether an affector is currently stored in the particle system.
		/// @return true if affector is used by this particle system, false otherwise.
		bool						containsAffector(Affector::Ptr affector) const;

		/// @brief Adds a particle emitter to the system.
		/// @param emitter Shared pointer to a derivate of Emitter (non-empty).
		/// @pre @a emitter has not been added yet.
		void						addEmitter(Emitter::Ptr emitter);

		/// @brief Adds a particle emitter for a certain amount of time.
		/// @param emitter Shared pointer to a derivate of Emitter (non-empty).
		/// @param timeUntilRemoval Time after which the emitter is automatically removed from the system.
		/// @pre @a emitter has not been added yet.
		void						addEmitter(Emitter::Ptr emitter, sf::Time timeUntilRemoval);

		/// @brief Removes a particle emitter from the system.
		/// @param emitter Shared pointer to a derivate of Emitter (non-empty).
		/// @pre @a emitter is currently stored in the particle system.
		void						removeEmitter(Emitter::Ptr emitter);

		/// @brief Removes all %Emitter instances from the system.
		/// @details Particles that are currently in the system are still processed, but no new ones
		///  are emitted until you add another emitter.
		void						clearEmitters();
		
		/// @brief Checks whether an emitter is currently stored in the particle system.
		/// @return true if emitter is used by this particle system, false otherwise.
		bool						containsEmitter(Emitter::Ptr emitter) const;

		/// @brief Updates all particles in the system.
		/// @details Invokes all emitters and applies all affectors. The lifetime of every particle is decreased,
		///  dead particles are removed.
		/// @param dt Frame duration.
		void						update(sf::Time dt);
		
		/// @brief Removes all particles that are currently in the system.
		///
		void						clearParticles();


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		/// @brief Draws all particles in the system.
		/// @param target The render target to which the particles are drawn.
		/// @param states Current render states (shader, blend mode, ...)
		virtual void				draw(sf::RenderTarget& target, sf::RenderStates states) const;
		
		/// @brief Adds a particle to the system.
		/// @param particle Particle to add.
		virtual void				addParticle(const Particle& particle);
			
		// Updates a single particle.
		void						updateParticle(Particle& particle, sf::Time dt);

		// Recomputes the vertex array.
		void						computeVertices() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		ParticleContainer					mParticles;
		AffectorContainer					mAffectors;
		EmitterContainer					mEmitters;

		const sf::Texture*					mTexture;
		sf::IntRect							mTextureRect;

		mutable sf::VertexArray				mVertices;
		mutable bool						mNeedsVertexUpdate;
};

/// @relates ParticleSystem
/// @brief Exchanges the contents of two ParticleSystem instances.
AURORA_GLOBAL_SWAP(ParticleSystem)

/// @}

} // namespace thor

#endif // THOR_PARTICLESYSTEM_HPP
