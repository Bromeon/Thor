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

namespace thor
{

template <class Animated, typename Id>
Animator<Animated, Id>::Animator()
: mAnimationMap()
, mPlayingAnimation(mAnimationMap.end())
, mProgress(0.f)
, mLoop(false)
{
}

template <class Animated, typename Id>
Animator<Animated, Id>::Animator(const Animator& origin)
// No initialization
{
	adopt(origin);
}

template <class Animated, typename Id>
Animator<Animated, Id>& Animator<Animated, Id>::operator= (const Animator& origin)
{
	adopt(origin);
	return *this;
}

template <class Animated, typename Id>
Animator<Animated, Id>::Animator(Animator&& source)
// No initialization
{
	adopt(source);
}

template <class Animated, typename Id>
Animator<Animated, Id>& Animator<Animated, Id>::operator= (Animator&& source)
{
	adopt(source);
	return *this;
}

template <class Animated, typename Id>
Animator<Animated, Id>::~Animator()
{
}

template <class Animated, typename Id>
void Animator<Animated, Id>::addAnimation(const Id& id, const AnimationFunction& animation, sf::Time duration)
{
	assert(mAnimationMap.find(id) == mAnimationMap.end());
	mAnimationMap.insert( std::make_pair(id, ScaledAnimation(animation, duration)) );
}

template <class Animated, typename Id>
void Animator<Animated, Id>::playAnimation(const Id& id, bool loop)
{
	AnimationMapIterator itr = mAnimationMap.find(id);
	assert(itr != mAnimationMap.end());

	playAnimation(itr, loop);
}

template <class Animated, typename Id>
void Animator<Animated, Id>::stopAnimation()
{
	mPlayingAnimation = mAnimationMap.end();
}

template <class Animated, typename Id>
bool Animator<Animated, Id>::isPlayingAnimation() const
{
	return mPlayingAnimation != mAnimationMap.end();
}

template <class Animated, typename Id>
Id Animator<Animated, Id>::getPlayingAnimation() const
{
	assert(isPlayingAnimation());

	return mPlayingAnimation->first;
}

template <class Animated, typename Id>
void Animator<Animated, Id>::update(sf::Time dt)
{
	// No animation playing: Do nothing
	if (!isPlayingAnimation())
		return;

	// Update progress, scale dt with 1 / current animation duration
	mProgress += dt.asSeconds() / mPlayingAnimation->second.second.asSeconds();

	// If animation is expired, stop or restart animation at loops
	if (mProgress > 1.f)
	{
		if (mLoop)
			mProgress -= std::floor(mProgress); // Store only fractional part
		else
			stopAnimation();
	}
}

template <class Animated, typename Id>
void Animator<Animated, Id>::animate(Animated& animated) const
{
	// If animation is playing, apply it
	if (isPlayingAnimation())
		mPlayingAnimation->second.first(animated, mProgress);
}

template <class Animated, typename Id>
void Animator<Animated, Id>::playAnimation(AnimationMapIterator animation, bool loop)
{
	mPlayingAnimation = animation;
	mProgress = 0.f;
	mLoop = loop;
}

template <class Animated, typename Id>
template <typename T>
void Animator<Animated, Id>::adopt(T& source)
{
	// Move or copy source into this, depending on parameter type. T& is either Animator& or const Animator&

	// Get playing ID from source before map is possibly moved
	bool playing = source.isPlayingAnimation();
	Id playingId = playing ? source.getPlayingAnimation() : Id();

	// Move/copy other variables (std::move only has effect when source is mutable)
	mAnimationMap = std::move(source.mAnimationMap);
	mProgress = source.mProgress;
	mLoop = source.mLoop;

	// Reset playing ID in this instance
	if (playing)
		mPlayingAnimation = mAnimationMap.find(playingId);
	else
		mPlayingAnimation = mAnimationMap.end();
}

} // namespace thor
