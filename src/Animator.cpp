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

#include <Thor/Animation/Animator.hpp>

#include <cassert>


namespace thor
{

Animator::Animator()
: mAnimationMap()
, mPlayingAnimation(NULL)
, mDefaultAnimation()
, mProgress(0.f)
, mLoop(false)
{
}

void Animator::addAnimation(const std::string& name, Animation::Ptr animation, sf::Time duration)
{
	assert(mAnimationMap.find(name) == mAnimationMap.end());
	mAnimationMap.insert( std::make_pair(name, ScaledAnimation(animation, duration)) );
}

void Animator::playAnimation(const std::string& name, bool loop)
{
	AnimationMap::iterator itr = mAnimationMap.find(name);
	assert(itr != mAnimationMap.end());

	playAnimation(itr->second, loop);
}

void Animator::stopAnimation()
{
	// Animations stopped: Play default animation if available
	if (mDefaultAnimation.first)
		playAnimation(mDefaultAnimation, true);
	else
		mPlayingAnimation = NULL;
}

void Animator::update(sf::Time dt)
{
	// No animation playing (no default animation available): Do nothing
	if (!mPlayingAnimation)
		return;

	// Update progress, scale dt with 1 / current animation duration
	mProgress += dt.asSeconds() / mPlayingAnimation->second.asSeconds();

	// If animation is expired, stop or restart animation at loops
	if (mProgress > 1.f)
	{
		if (mLoop)
			mProgress -= 1.f;
		else
			stopAnimation();
	}
}

void Animator::animate(sf::Sprite& target) const
{
	// If animation is playing, apply it (includes default animation, if others are stopped)
	if (mPlayingAnimation)
		mPlayingAnimation->first->apply(target, mProgress);
}

void Animator::playAnimation(ScaledAnimation& animation, bool loop)
{
	mPlayingAnimation = &animation;
	mProgress = 0.f;
	mLoop = loop;
}

void Animator::setDefaultAnimation(Animation::Ptr animation, sf::Time duration)
{
	// Invalidate old playing animation, if it refers to default animation (which might be destroyed)
	if (mPlayingAnimation == &mDefaultAnimation)
		mPlayingAnimation = NULL;

	// Assign animation and duration
	mDefaultAnimation.first = animation;
	mDefaultAnimation.second = duration;

	// If no other animation is active, play default animation
	if (!mPlayingAnimation)
		stopAnimation();
}

} // namespace thor
