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

#include <Thor/Particles/ParticleSystem.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>

#include <Aurora/Tools/ForEach.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <algorithm>
#include <array>
#include <cmath>


namespace thor
{
namespace
{

	// Functor as argument for std::find_if()
	template <typename First>
	struct FirstFinder
	{
		explicit FirstFinder(const First& ptr)
		: ptr(ptr)
		{
		}

		bool operator() (const std::pair<First, sf::Time>& element)
		{
			return element.first == ptr;
		}

		const First& ptr;
	};

	// Erases emitter/affector at itr from c, if its time has expired. itr will point to the next element.
	template <class Container>
	void incrementCheckExpiry(Container& ctr, typename Container::iterator& itr, sf::Time dt)
	{
		// itr->second is the remaining time of the emitter/affector.
		// Time::Zero means infinite time (no removal).
		if (itr->second != sf::Time::Zero && (itr->second -= dt) <= sf::Time::Zero)
			itr = ctr.erase(itr);
		else
			++itr;
	}

} // namespace

// ---------------------------------------------------------------------------------------------------------------------------


ParticleSystem::ParticleSystem(std::shared_ptr<const sf::Texture> texture)
: mParticles()
, mAffectors()
, mEmitters()
, mTexture()
, mTextureRect(0, 0, texture->getSize().x, texture->getSize().y)
, mVertices(sf::Quads)
, mNeedsVertexUpdate(true)
{
	mTexture.swap(texture);
}

ParticleSystem::ParticleSystem(std::shared_ptr<const sf::Texture> texture, const sf::IntRect& textureRect)
: mParticles()
, mAffectors()
, mEmitters()
, mTexture()
, mTextureRect(textureRect)
, mVertices(sf::Quads)
, mNeedsVertexUpdate(true)
{
	mTexture.swap(texture);
}

void ParticleSystem::swap(ParticleSystem& other)
{
	// Use ADL
	using std::swap;

	swap(mParticles,			other.mParticles);
	swap(mAffectors,			other.mAffectors);
	swap(mEmitters,				other.mEmitters);
	swap(mTexture,				other.mTexture); 
	swap(mTextureRect,			other.mTextureRect);
	swap(mVertices,				other.mVertices);
	swap(mNeedsVertexUpdate,	other.mNeedsVertexUpdate);
}

void ParticleSystem::addAffector(Affector::Ptr affector)
{
	addAffector(affector, sf::Time::Zero);
}

void ParticleSystem::addAffector(Affector::Ptr affector, sf::Time timeUntilRemoval)
{
	assert(affector);
	assert(!containsAffector(affector));

	mAffectors.push_back( std::make_pair(affector, timeUntilRemoval) );
}

void ParticleSystem::removeAffector(Affector::Ptr affector)
{
	AffectorContainer::iterator itr = std::find_if(mAffectors.begin(), mAffectors.end(), FirstFinder<Affector::Ptr>(affector));
	assert(itr != mAffectors.end());

	mAffectors.erase(itr);
}

void ParticleSystem::clearAffectors()
{
	mAffectors.clear();
}

bool ParticleSystem::containsAffector(Affector::Ptr affector) const
{
	return std::find_if(mAffectors.begin(), mAffectors.end(), FirstFinder<Affector::Ptr>(affector)) != mAffectors.end();
}

void ParticleSystem::addEmitter(Emitter::Ptr emitter)
{
	addEmitter(emitter, sf::Time::Zero);
}

void ParticleSystem::addEmitter(Emitter::Ptr emitter, sf::Time timeUntilRemoval)
{
	assert(emitter);
	assert(!containsEmitter(emitter));

	mEmitters.push_back( std::make_pair(emitter, timeUntilRemoval) );
}

void ParticleSystem::removeEmitter(Emitter::Ptr emitter)
{
	EmitterContainer::iterator itr = std::find_if(mEmitters.begin(), mEmitters.end(), FirstFinder<Emitter::Ptr>(emitter));
	assert(itr != mEmitters.end());

	mEmitters.erase(itr);
}

void ParticleSystem::clearEmitters()
{
	mEmitters.clear();
}

bool ParticleSystem::containsEmitter(Emitter::Ptr emitter) const
{
	return std::find_if(mEmitters.begin(), mEmitters.end(), FirstFinder<Emitter::Ptr>(emitter)) != mEmitters.end();
}

void ParticleSystem::update(sf::Time dt)
{
	// Invalidate stored vertices
	mNeedsVertexUpdate = true;

	// Emit new particles and remove expiring emitters
	for (EmitterContainer::iterator itr = mEmitters.begin(); itr != mEmitters.end(); )
	{
		itr->first->emit(*this, dt);
		incrementCheckExpiry(mEmitters, itr, dt);
	}

	// Affect existing particles
	ParticleContainer::iterator writer = mParticles.begin();
	for (ParticleContainer::iterator reader = mParticles.begin(); reader != mParticles.end(); ++reader)
	{
		// Apply movement and decrease lifetime
		updateParticle(*reader, dt);
		
		// If current particle is not dead
		if (reader->passedLifetime < reader->totalLifetime)
		{
			// Only apply affectors to living particles
			AURORA_ITR_FOREACH(AffectorContainer, mAffectors, itr2)
				itr2->first->affect(*reader, dt);

			// Go ahead
			*writer++ = *reader;
		}
	}

	// Remove particles dying this frame
	mParticles.erase(writer, mParticles.end());

	// Remove affectors expiring this frame
	for (AffectorContainer::iterator itr = mAffectors.begin(); itr != mAffectors.end(); )
	{
		incrementCheckExpiry(mAffectors, itr, dt);
	}
}

void ParticleSystem::clearParticles()
{
	mParticles.clear();
}

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mNeedsVertexUpdate)
	{
		computeVertices();
		mNeedsVertexUpdate = false;
	}

	// Draw the vertex array with our texture
	states.texture = mTexture.get();
	target.draw(mVertices, states);
}

void ParticleSystem::addParticle(const Particle& particle)
{
	mParticles.push_back(particle);
}

void ParticleSystem::updateParticle(Particle& particle, sf::Time dt)
{
	particle.passedLifetime += dt;

	particle.position += dt.asSeconds() * particle.velocity;
	particle.rotation += dt.asSeconds() * particle.rotationSpeed;
}

void ParticleSystem::computeVertices() const
{
	// Clear vertex array (keeps memory allocated)
	mVertices.clear();

	// Compute offsets to vertex positions in rendering coordinates
	const sf::Vector2f halfSize = sf::Vector2f(mTexture->getSize()) / 2.f;
	const std::array<sf::Vector2f, 4> positionOffsets =
	{
		-halfSize,
		perpendicularVector(halfSize),
		halfSize,
		-perpendicularVector(halfSize)
	};

	// Compute absolute positions of vertex texture coordinates
	const float left	= static_cast<float>(mTextureRect.left);
	const float right	= static_cast<float>(mTextureRect.left + mTextureRect.width);
	const float top		= static_cast<float>(mTextureRect.top);
	const float bottom	= static_cast<float>(mTextureRect.top + mTextureRect.height);
	const std::array<sf::Vector2f, 4> texCoords =
	{
		sf::Vector2f(left,	top),
		sf::Vector2f(right,	top),
		sf::Vector2f(right,	bottom),
		sf::Vector2f(left,	bottom)
	};

	// Fill vertex array
	AURORA_CITR_FOREACH(ParticleContainer, mParticles, itr)
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			sf::Transform transform;
			transform.translate(itr->position);
			transform.rotate(itr->rotation);
			transform.scale(itr->scale);

			mVertices.append( sf::Vertex(transform.transformPoint(positionOffsets[i]), itr->color, texCoords[i]) );
		}
	}
}

} // namespace thor
