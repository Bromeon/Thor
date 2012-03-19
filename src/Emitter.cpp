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

#include <Thor/Particles/Emitter.hpp>
#include <Thor/Math/Random.hpp>

#include <cmath>
#include <cassert>


namespace thor
{

DirectionalEmitter::Ptr DirectionalEmitter::create(float particlesPerSecond, sf::Time particleLifetime)
{
	return Ptr(new DirectionalEmitter(particlesPerSecond, particleLifetime));
}

DirectionalEmitter::DirectionalEmitter(float particlesPerSecond, sf::Time particleLifetime)
: Emitter(particlesPerSecond, particleLifetime)
, mEmissionAngle(0.f)
, mParticleVelocity()
{
}

void DirectionalEmitter::emit(Adder& system, sf::Time dt)
{
	const unsigned int nbParticles = computeNbParticles(dt);

	for (unsigned int i = 0; i < nbParticles; ++i)
	{
		// Create particle and specify parameters
		Particle particle = createParticlePrototype();
		particle.velocity = mParticleVelocity;

		// Influence of emission angle
		float var = randomDev(0.f, getEmissionAngle() / 2.f);
		rotate(particle.velocity, var);

		system.addParticle(particle);
	}
}

void DirectionalEmitter::setEmissionAngle(float emissionAngle)
{
	mEmissionAngle = emissionAngle;
}

float DirectionalEmitter::getEmissionAngle() const
{
	return mEmissionAngle;
}

void DirectionalEmitter::setParticleVelocity(sf::Vector2f velocity)
{
	mParticleVelocity = velocity;
}

sf::Vector2f DirectionalEmitter::getParticleVelocity() const
{
	return mParticleVelocity;
}

// ---------------------------------------------------------------------------------------------------------------------------


TargetEmitter::Ptr TargetEmitter::create(float particlesPerSecond, sf::Time particleLifetime)
{
	return Ptr(new TargetEmitter(particlesPerSecond, particleLifetime));
}

TargetEmitter::TargetEmitter(float particlesPerSecond, sf::Time particleLifetime)
: Emitter(particlesPerSecond, particleLifetime)
, mTargetZone(getEmissionZone().clone(), aur::VirtualClone<thor::Zone>(), aur::OperatorDelete<thor::Zone>())
, mParticleSpeed(0.f)
{
}

void TargetEmitter::emit(Adder& system, sf::Time dt)
{
	const unsigned int nbParticles = computeNbParticles(dt);

	for (unsigned int i = 0; i < nbParticles; ++i)
	{
		// Create particle and specify parameters
		Particle particle = createParticlePrototype();

		// Compute vector from emission to target zone and scale it (unless it is zero)
		sf::Vector2f direction = mTargetZone->getRandomPoint() - particle.position;
		if (direction != sf::Vector2f())
			particle.velocity = mParticleSpeed * unitVector(direction);

		system.addParticle(particle);
	}
}

void TargetEmitter::setTargetZone(ZonePtr zone)
{
	mTargetZone = zone;
}

Zone& TargetEmitter::getTargetZone()
{
	return *mTargetZone;
}

const Zone& TargetEmitter::getTargetZone() const
{
	return *mTargetZone;
}

void TargetEmitter::setParticleSpeed(float speed)
{
	mParticleSpeed = speed;
}

float TargetEmitter::getParticleSpeed() const
{
	return mParticleSpeed;
}

} // namespace thor
