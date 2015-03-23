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
/// @brief Class thor::ParticleSystem

#ifndef THOR_PARTICLESYSTEM_HPP
#define THOR_PARTICLESYSTEM_HPP

#include <Thor/Particles/Particle.hpp>
#include <Thor/Particles/EmissionInterface.hpp>
#include <Thor/Input/Connection.hpp>
#include <Thor/Config.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <memory>
#include <array>


namespace sf
{

	class RenderWindow;
	class Image;
	class Texture;

} // namespace sf


namespace thor
{
namespace detail
{
	class AbstractConnectionImpl;
}

/// @addtogroup Particles
/// @{

/// @brief Class for particle systems.
/// @details A particle system stores, updates and draws particles. It also stores emitter and affector functions that control
///  how particles are generated and modified over time. To represent particles graphically, a particle system requires a texture,
///  and optionally one or multiple texture rectangles.
/// @n@n This class is noncopyable.
class THOR_API ParticleSystem : public sf::Drawable, private sf::NonCopyable, private EmissionInterface
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		// Type to store affector or emitter + time until removal + id for removal
		template <typename Signature>
		struct Function
		{
			Function(std::function<Signature> function, sf::Time timeUntilRemoval)
			: function(std::move(function))
			, timeUntilRemoval(timeUntilRemoval)
			, id(nextId())
			, tracker()
			{
			}

			static unsigned int nextId()
			{
				static unsigned int next = 0;
				return next++;
			}

			std::function<Signature>						function;
			sf::Time										timeUntilRemoval;
			unsigned int									id;
			std::shared_ptr<detail::AbstractConnectionImpl> tracker;
		};

		// Vertex quads, used to cache texture rectangles
		typedef std::array<sf::Vertex, 4>					Quad;

		// Function typedefs
		typedef Function<void(Particle&, sf::Time)>			Affector;
		typedef Function<void(EmissionInterface&, sf::Time)>	Emitter;

		// Container typedefs
		typedef std::vector<Particle>						ParticleContainer;
		typedef std::vector<Affector>						AffectorContainer;
		typedef std::vector<Emitter>						EmitterContainer;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		/// @details Requires a call to setTexture() and possibly addTextureRect() before the particle system can be used.
									ParticleSystem();

		/// @brief Move constructor
		/// 
									ParticleSystem(ParticleSystem&& source);

		/// @brief Move assignment operator
		/// 
		ParticleSystem&				operator= (ParticleSystem&& source);

		/// @brief Sets the used texture.
		/// @details Only one texture can be used at a time. If you need multiple particle representations, specify different texture
		///  rectangles using the method addTextureRect(). If no texture rect is added, the whole texture will be used.
		/// @param texture Reference to the sf::Texture used as particle texture. The texture must remain valid as long as the
		///  particle system uses it.
		void						setTexture(const sf::Texture& texture);

		/// @brief Defines a new texture rect to represent a particle.
		/// @details Can be used to create different visual representations of a particle, for example different shapes of debris.
		/// @param textureRect Area of the texture that is used to draw the particle.
		/// @return Index of the texture rect just added. This number shall be assigned to the Particle::textureIndex variable,
		///  in order to render a particle using @a textureRect.
		/// @see setTexture()
		unsigned int				addTextureRect(const sf::IntRect& textureRect);

		/// @brief Adds a particle affector to the system.
		/// @details Be aware that multiple affectors can interfere with each other. The affectors are applied in the order they were
		///  added to the system, therefore affectors at the end may overwrite particle states set by earlier affectors. To completely
		///  avoid the issue, only add orthogonal affectors (e.g. one for color, one for acceleration...).
		/// @param affector Affector function object which is copied into the particle system.
		/// @return Object that can be used to disconnect (remove) the affector from the system.
		Connection					addAffector(std::function<void(Particle&, sf::Time)> affector);

		/// @brief Adds a particle affector for a certain amount of time.
		/// @details Be aware that multiple affectors can interfere with each other. The affectors are applied in the order they were
		///  added to the system, therefore affectors at the end may overwrite particle states set by earlier affectors. To completely
		///  avoid the issue, only add orthogonal affectors (e.g. one for color, one for acceleration...).
		/// @param affector Affector function object which is copied into the particle system.
		/// @param timeUntilRemoval Time after which the affector is automatically removed from the system.
		/// @return Object that can be used to disconnect (remove) the affector from the system.
		Connection					addAffector(std::function<void(Particle&, sf::Time)> affector, sf::Time timeUntilRemoval);

		/// @brief Removes all affector instances from the system.
		/// @details All particles lose the influence of any external affectors. Movement and lifetime is still computed.
		void						clearAffectors();

		/// @brief Adds a particle emitter to the system.
		/// @param emitter Emitter function object which is copied into the particle system.
		/// @return Object that can be used to disconnect (remove) the emitter from the system.
		Connection					addEmitter(std::function<void(EmissionInterface&, sf::Time)> emitter);

		/// @brief Adds a particle emitter for a certain amount of time.
		/// @param emitter Emitter function object which is copied into the particle system.
		/// @param timeUntilRemoval Time after which the emitter is automatically removed from the system.
		/// @return Object that can be used to disconnect (remove) the emitter from the system.
		Connection					addEmitter(std::function<void(EmissionInterface&, sf::Time)> emitter, sf::Time timeUntilRemoval);

		/// @brief Removes all emitter instances from the system.
		/// @details Particles that are currently in the system are still processed, but no new ones
		///  are emitted until you add another emitter.
		void						clearEmitters();

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

		/// @brief Emits a particle into the system.
		/// @param particle Particle to emit.
		virtual void				emitParticle(const Particle& particle);

		// Updates a single particle.
		void						updateParticle(Particle& particle, sf::Time dt);

		// Recomputes the vertex array.
		void						computeVertices() const;

		// Recomputes the cached rectangles (position and texCoords quads)
		void						computeQuads() const;
		void						computeQuad(Quad& quad, const sf::IntRect& textureRect) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		ParticleContainer			mParticles;
		AffectorContainer			mAffectors;
		EmitterContainer			mEmitters;

		const sf::Texture*			mTexture;
		std::vector<sf::IntRect>	mTextureRects;

		mutable sf::VertexArray		mVertices;
		mutable bool				mNeedsVertexUpdate;
		mutable std::vector<Quad>	mQuads;
		mutable bool				mNeedsQuadUpdate;
};

/// @}

} // namespace thor

#endif // THOR_PARTICLESYSTEM_HPP
