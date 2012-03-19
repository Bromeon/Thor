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

#include <Thor/Particles/ParticleInterfaces.hpp>
#include <Thor/Particles/Particle.hpp>
#include <Thor/Geometry/Point.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include <Thor/Math/Random.hpp>

#include <cassert>


namespace thor
{
	
Affector::~Affector()
{
}

// ---------------------------------------------------------------------------------------------------------------------------


Emitter::Emitter(float particlesPerSecond, sf::Time particleLifetime)
: mEmissionZone(new Point(sf::Vector2f()))
, mEmissionRate(particlesPerSecond)
, mEmissionDifference(0.f)
, mParticleLifetime(particleLifetime)
, mParticleScale(1.f, 1.f)
, mParticleColor(sf::Color::White)
{
	assert(particlesPerSecond > 0.f);
	assert(particleLifetime > sf::Time::Zero);
}

Emitter::~Emitter()
{
}

void Emitter::setEmissionZone(ZonePtr zone)
{
	mEmissionZone.swap(zone);
}

Zone& Emitter::getEmissionZone()
{
	return *mEmissionZone;
}

const Zone& Emitter::getEmissionZone() const
{
	return *mEmissionZone;
}

void Emitter::setEmissionRate(float particlesPerSecond)
{
	assert(particlesPerSecond > 0.f);
	mEmissionRate = particlesPerSecond;
}

float Emitter::getEmissionRate() const
{
	return mEmissionRate;
}

void Emitter::setParticleScale(sf::Vector2f scale)
{
	mParticleScale = scale;
}

sf::Vector2f Emitter::getParticleScale() const
{
	return mParticleScale;
}

void Emitter::setParticleColor(const sf::Color& color)
{
	mParticleColor = color;
}

const sf::Color& Emitter::getParticleColor() const
{
	return mParticleColor;
}

void Emitter::setParticleLifetime(sf::Time lifetime)
{
	assert(lifetime > sf::Time::Zero);
	mParticleLifetime = lifetime;
}

sf::Time Emitter::getParticleLifetime() const
{
	return mParticleLifetime;
}

unsigned int Emitter::computeNbParticles(sf::Time dt)
{
	// We want to fulfill the desired particle rate as exact as possible. Since the amount of emitted particles per frame is
	// integral, we have to emit sometimes more and sometimes less. mParticleDifference takes care of the deviation each frame.
	float particleAmount = mEmissionRate * dt.asSeconds() + mEmissionDifference;
	unsigned int nbParticles = static_cast<unsigned int>(particleAmount);

	// Compute difference for next frame, return current amount
	mEmissionDifference = particleAmount - nbParticles;
	return nbParticles;
}

Particle Emitter::createParticlePrototype() const
{
	Particle particle(mParticleLifetime);
	particle.position = mEmissionZone->getRandomPoint();
	particle.rotation = mEmissionZone->getRotation();
	particle.scale    = mParticleScale;
	particle.color    = mParticleColor;

	return particle;
}

} // namespace thor
