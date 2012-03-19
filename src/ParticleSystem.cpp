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
#include <Aurora/Tools/Exceptions.hpp>

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <algorithm>
#include <cmath>


namespace thor
{
namespace
{

	// Function to setup the OpenGL view according to the passed parameter
	void setupOpenGL2DView(const sf::View& view)
	{
		// Precondition: The following statements have been executed:
		// glMatrixMode(GL_PROJECTION);
		// glLoadIdentity();

		const sf::Vector2f size = view.getSize();
		const sf::Vector2f center = view.getCenter();
		const sf::Vector2f position = center - size / 2.f;

		// setup translation (according to left-upper corner) and scale
		glOrtho(0., size.x, size.y, 0., -1., 1.);
		glTranslatef(-position.x, -position.y, 0.f);
	
		// Setup rotation 
		glTranslatef(center.x, center.y, 0.f);
		glRotatef(view.getRotation(), 0.f, 0.f, -1.f);
		glTranslatef(-center.x, -center.y, 0.f);
	}

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

} // namespace detail

// ---------------------------------------------------------------------------------------------------------------------------


ParticleSystem::ParticleSystem(ResourcePtr<const sf::Texture> texture)
: mParticles()
, mAffectors()
, mEmitters()
, mTexture()
, mTexCoordsBegin(0.f, 0.f)
, mTexCoordsEnd(1.f, 1.f)
, mHalfSize(texture->getWidth() / 2.f, texture->getHeight() / 2.f)
, mGlow(false)
{
	mTexture.swap(texture);
}

ParticleSystem::ParticleSystem(ResourcePtr<const sf::Texture> texture, const sf::IntRect& textureRect)
: mParticles()
, mAffectors()
, mEmitters()
, mTexture()
, mTexCoordsBegin()
, mTexCoordsEnd()
, mHalfSize(texture->getWidth() / 2.f, texture->getHeight() / 2.f)
, mGlow(false)
{
	mTexture.swap(texture);

	mTexCoordsBegin.x = static_cast<float>(textureRect.left)                     / mTexture->getWidth();
	mTexCoordsBegin.y = static_cast<float>(textureRect.top)                      / mTexture->getHeight();
	mTexCoordsEnd.x	  = static_cast<float>(textureRect.left + textureRect.width) / mTexture->getWidth();
	mTexCoordsEnd.y   = static_cast<float>(textureRect.top + textureRect.height) / mTexture->getHeight();
}

void ParticleSystem::swap(ParticleSystem& other)
{
	// Use ADL
	using std::swap;

	swap(mParticles, other.mParticles);
	swap(mAffectors, other.mAffectors);
	swap(mEmitters,  other.mEmitters);
	swap(mTexture,	 other.mTexture); 
	swap(mGlow,      other.mGlow);
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

void ParticleSystem::addParticle(const Particle& particle)
{
	mParticles.push_back(particle);
}

void ParticleSystem::update(sf::Time dt)
{
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

void ParticleSystem::draw(sf::RenderWindow& target) const
{
	pushOpenGLStates(target);

	AURORA_CITR_FOREACH(ParticleContainer, mParticles, itr)
		drawParticle(*itr);

	popOpenGLStates();
}

void ParticleSystem::clearParticles()
{
	mParticles.clear();
}

void ParticleSystem::setGlowing(bool glow)
{
	mGlow = glow;
}

bool ParticleSystem::isGlowing() const
{
	return mGlow;
}

void ParticleSystem::pushOpenGLStates(sf::RenderWindow& target) const
{
	// Activate SFML window for OpenGL rendering
	target.setActive();
	
	// Store SFML attributes
	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT | GL_TRANSFORM_BIT | GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	// Set blend function depending on glow effect
	if (mGlow)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	else
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind texture - instead of glBindTexture(GL_TEXTURE_2D, id)
	mTexture->bind();

	// Push and initialize projection matrix (2D view)
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	setupOpenGL2DView(target.getView());

	// Push and initialize texture matrix
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	// Push modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void ParticleSystem::popOpenGLStates() const
{
	// Restore modelview matrix
	glMatrixMode(GL_MODELVIEW); 
	glPopMatrix();

	// Restore texture matrix
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	// Restore projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Restore attributes
	glPopAttrib();
}

void ParticleSystem::updateParticle(Particle& particle, sf::Time dt)
{
	particle.passedLifetime += dt;

	particle.position += dt.asSeconds() * particle.velocity;
	particle.rotation += dt.asSeconds() * particle.rotationSpeed;
}

void ParticleSystem::drawParticle(const Particle& particle) const
{
	// Note: At the moment, the GL_MODELVIEW matrix is being edited

	// Reset transforms
	glLoadIdentity();

	// Apply translation, rotation, scale
	glTranslatef(particle.position.x, particle.position.y, 0.f);
	glRotatef(particle.rotation, 0.f, 0.f, 1.f);
	glScalef(particle.scale.x, particle.scale.y, 1.f);

	// Set color
	glColor4ub(particle.color.r, particle.color.g, particle.color.b, particle.color.a);

	// Draw vertices
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(mTexCoordsEnd.x,   mTexCoordsEnd.y);			glVertex2f(+mHalfSize.x, +mHalfSize.y);
		glTexCoord2f(mTexCoordsBegin.x, mTexCoordsEnd.y);			glVertex2f(-mHalfSize.x, +mHalfSize.y);
		glTexCoord2f(mTexCoordsEnd.x,   mTexCoordsBegin.y);			glVertex2f(+mHalfSize.x, -mHalfSize.y);
		glTexCoord2f(mTexCoordsBegin.x, mTexCoordsBegin.y);			glVertex2f(-mHalfSize.x, -mHalfSize.y);
	glEnd();
}

} // namespace thor
