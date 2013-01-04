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

#include <Thor/Particles/Affector.hpp>
#include <Thor/Particles/Particle.hpp>

#include <cassert>
#include <cmath>


namespace thor
{
	
ForceAffector::Ptr ForceAffector::create(sf::Vector2f acceleration)
{
	return std::make_shared<ForceAffector>(acceleration);
}

ForceAffector::ForceAffector(sf::Vector2f acceleration)
: mAcceleration(acceleration)
{
}

void ForceAffector::affect(Particle& particle, sf::Time dt)
{
	particle.velocity += dt.asSeconds() * mAcceleration;
}

void ForceAffector::setAcceleration(sf::Vector2f acceleration)
{
	mAcceleration = acceleration;
}

sf::Vector2f ForceAffector::getAcceleration() const
{
	return mAcceleration;
}

// ---------------------------------------------------------------------------------------------------------------------------


TorqueAffector::Ptr TorqueAffector::create(float angularAcceleration)
{
	return std::make_shared<TorqueAffector>(angularAcceleration);
}

TorqueAffector::TorqueAffector(float angularAcceleration)
: mAngularAcceleration(angularAcceleration)
{
}

void TorqueAffector::affect(Particle& particle, sf::Time dt)
{
	particle.rotationSpeed += dt.asSeconds() * mAngularAcceleration;
}

void TorqueAffector::setAngularAcceleration(float angularAcceleration)
{
	mAngularAcceleration = angularAcceleration;
}

float TorqueAffector::getAngularAcceleration() const
{
	return mAngularAcceleration;
}

// ---------------------------------------------------------------------------------------------------------------------------


ScaleAffector::Ptr ScaleAffector::create(sf::Vector2f scaleFactor)
{
	return std::make_shared<ScaleAffector>(scaleFactor);
}

ScaleAffector::ScaleAffector(sf::Vector2f scaleFactor)
: mScaleFactor(scaleFactor)
{
}

void ScaleAffector::affect(Particle& particle, sf::Time dt)
{
	particle.scale += dt.asSeconds() * mScaleFactor;
}

void ScaleAffector::setScaleFactor(sf::Vector2f scaleFactor)
{
	mScaleFactor = scaleFactor;
}

sf::Vector2f ScaleAffector::getScaleFactor() const
{
	return mScaleFactor;
}

// ---------------------------------------------------------------------------------------------------------------------------


AnimationAffector::Ptr AnimationAffector::create(std::function<void(Particle&, float)> particleAnimation)
{
	return std::make_shared<AnimationAffector>(std::move(particleAnimation));
}

AnimationAffector::AnimationAffector(std::function<void(Particle&, float)> particleAnimation)
: mAnimation(std::move(particleAnimation))
{
}

void AnimationAffector::affect(Particle& particle, sf::Time)
{
	mAnimation(particle, getPassedRatio(particle));
}

} // namespace thor
