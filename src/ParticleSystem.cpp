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

#include <Thor/Particles/ParticleSystem.hpp>
#include <Thor/Input/Detail/ConnectionImpl.hpp>

#include <Aurora/Tools/ForEach.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cassert>


namespace thor
{
namespace
{

	// Erases emitter/affector at itr from ctr, if its time has expired. itr will point to the next element.
	template <class Container>
	void incrementCheckExpiry(Container& ctr, typename Container::iterator& itr, sf::Time dt)
	{
		// itr->second is the remaining time of the emitter/affector.
		// Time::Zero means infinite time (no removal).
		if (itr->timeUntilRemoval != sf::Time::Zero && (itr->timeUntilRemoval -= dt) <= sf::Time::Zero)
			itr = ctr.erase(itr);
		else
			++itr;
	}

	sf::IntRect getFullRect(const sf::Texture& texture)
	{
		return sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y);
	}

} // namespace

// ---------------------------------------------------------------------------------------------------------------------------


ParticleSystem::ParticleSystem()
: mParticles()
, mAffectors()
, mEmitters()
, mTexture(nullptr)
, mTextureRects()
, mVertices(sf::Quads)
, mNeedsVertexUpdate(true)
, mQuads()
, mNeedsQuadUpdate(true)
{
}

ParticleSystem::ParticleSystem(ParticleSystem&& source)
: mParticles(std::move(source.mParticles))
, mAffectors(std::move(source.mAffectors))
, mEmitters(std::move(source.mEmitters))
, mTexture(std::move(source.mTexture))
, mTextureRects(std::move(source.mTextureRects))
, mVertices(std::move(source.mVertices))
, mNeedsVertexUpdate(std::move(source.mNeedsVertexUpdate))
, mQuads(std::move(source.mQuads))
, mNeedsQuadUpdate(std::move(source.mNeedsQuadUpdate))
{
}

ParticleSystem& ParticleSystem::operator= (ParticleSystem&& source)
{
	mParticles = std::move(source.mParticles);
	mAffectors = std::move(source.mAffectors);
	mEmitters = std::move(source.mEmitters);
	mTexture = std::move(source.mTexture);
	mTextureRects = std::move(source.mTextureRects);
	mVertices = std::move(source.mVertices);
	mNeedsVertexUpdate = std::move(source.mNeedsVertexUpdate);
	mQuads = std::move(source.mQuads);
	mNeedsQuadUpdate = std::move(source.mNeedsQuadUpdate);

	return *this;
}

void ParticleSystem::setTexture(const sf::Texture& texture)
{
	mTexture = &texture;
	mNeedsQuadUpdate = true;
}

unsigned int ParticleSystem::addTextureRect(const sf::IntRect& textureRect)
{
	mTextureRects.push_back(textureRect);
	mNeedsQuadUpdate = true;

	return static_cast<unsigned int>(mTextureRects.size() - 1);
}

Connection ParticleSystem::addAffector(std::function<void(Particle&, sf::Time)> affector)
{
	return addAffector(std::move(affector), sf::Time::Zero);
}

Connection ParticleSystem::addAffector(std::function<void(Particle&, sf::Time)> affector, sf::Time timeUntilRemoval)
{
	mAffectors.push_back( Affector(std::move(affector), timeUntilRemoval) );
	mAffectors.back().tracker = detail::makeIdConnectionImpl(mAffectors, mAffectors.back().id);

	return Connection(mAffectors.back().tracker);
}

void ParticleSystem::clearAffectors()
{
	mAffectors.clear();
}

Connection ParticleSystem::addEmitter(std::function<void(EmissionInterface&, sf::Time)> emitter)
{
	return addEmitter(emitter, sf::Time::Zero);
}

Connection ParticleSystem::addEmitter(std::function<void(EmissionInterface&, sf::Time)> emitter, sf::Time timeUntilRemoval)
{
	mEmitters.push_back( Emitter(std::move(emitter), timeUntilRemoval) );
	mEmitters.back().tracker = detail::makeIdConnectionImpl(mEmitters, mEmitters.back().id);

	return Connection(mEmitters.back().tracker);
}

void ParticleSystem::clearEmitters()
{
	mEmitters.clear();
}

void ParticleSystem::update(sf::Time dt)
{
	// Invalidate stored vertices
	mNeedsVertexUpdate = true;

	// Emit new particles and remove expiring emitters
	for (EmitterContainer::iterator itr = mEmitters.begin(); itr != mEmitters.end(); )
	{
		itr->function(*this, dt);
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
			AURORA_FOREACH(auto& affectorPair, mAffectors)
				affectorPair.function(*reader, dt);

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
	// Check cached rectangles
	if (mNeedsQuadUpdate)
	{
		computeQuads();
		mNeedsQuadUpdate = false;
	}

	// Check cached vertices
	if (mNeedsVertexUpdate)
	{
		computeVertices();
		mNeedsVertexUpdate = false;
	}

	// Draw the vertex array with our texture
	states.texture = mTexture;
	target.draw(mVertices, states);
}

void ParticleSystem::emitParticle(const Particle& particle)
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

	// Fill vertex array
	AURORA_FOREACH(const Particle& p, mParticles)
	{
		sf::Transform transform;
		transform.translate(p.position);
		transform.rotate(p.rotation);
		transform.scale(p.scale);

		// Ensure valid index -- if this fails, you have not called addTextureRect() enough times, or p.textureIndex is simply wrong
		assert(p.textureIndex == 0 || p.textureIndex < mTextureRects.size());

		const auto& quad = mQuads[p.textureIndex];
		for (std::size_t i = 0; i < 4; ++i)
		{
			sf::Vertex vertex;
			vertex.position = transform.transformPoint(quad[i].position);
			vertex.texCoords = quad[i].texCoords;
			vertex.color = p.color;

			mVertices.append(vertex);
		}
	}
}

void ParticleSystem::computeQuads() const
{
	// Ensure setTexture() has been called
	assert(mTexture);

	// No texture rects: Use full texture, cache single rectangle
	if (mTextureRects.empty())
	{
		mQuads.resize(1);
		computeQuad(mQuads[0], getFullRect(*mTexture));
	}

	// Specified texture rects: Cache every one
	else
	{
		mQuads.resize(mTextureRects.size());
		for (std::size_t i = 0; i < mTextureRects.size(); ++i)
			computeQuad(mQuads[i], mTextureRects[i]);
	}
}

void ParticleSystem::computeQuad(Quad& quad, const sf::IntRect& textureRect) const
{
	sf::FloatRect rect(textureRect);

	quad[0].texCoords = sf::Vector2f(rect.left,              rect.top);
	quad[1].texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
	quad[2].texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
	quad[3].texCoords = sf::Vector2f(rect.left,              rect.top + rect.height);

	quad[0].position = sf::Vector2f(-rect.width, -rect.height) / 2.f;
	quad[1].position = sf::Vector2f( rect.width, -rect.height) / 2.f;
	quad[2].position = sf::Vector2f( rect.width,  rect.height) / 2.f;
	quad[3].position = sf::Vector2f(-rect.width,  rect.height) / 2.f;
}

} // namespace thor
