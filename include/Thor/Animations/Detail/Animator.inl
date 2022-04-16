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

namespace thor
{

template <class Animated, typename Id>
Animator<Animated, Id>::Animator(const AnimationMap<Animated, Id>& animations)
: mAnimations(&animations)
, mQueuedIds()
, mPlayingAnimations()
, mProgressedTime(sf::Time::Zero)
{
}

template <class Animated, typename Id>
detail::PlayingQueue<Id> Animator<Animated, Id>::play()
{
	stop();
	return queue();
}

template <class Animated, typename Id>
detail::PlayingQueue<Id> Animator<Animated, Id>::queue()
{
	return detail::PlayingQueue<Id>(mQueuedIds);
}

template <class Animated, typename Id>
void Animator<Animated, Id>::stop()
{
	aurora::clearQueue(mQueuedIds);
	mPlayingAnimations.clear();
	mProgressedTime = sf::Time::Zero;
}

template <class Animated, typename Id>
void Animator<Animated, Id>::update(sf::Time dt)
{
	// Clean up previously playing animations. Leave at most the last one, if it's still playing.
	sf::Time duration = sf::Time::Zero;
	if (!mPlayingAnimations.empty())
	{
		sf::Time lastDuration = mPlayingAnimations.back()->duration;

		// Last animation in list is still playing: remove ones before
		if (mProgressedTime < lastDuration)
		{
			mPlayingAnimations.erase(mPlayingAnimations.begin(), std::prev(mPlayingAnimations.end()));
			duration = lastDuration;
		}

		// None playing: remove all
		else
		{
			mPlayingAnimations.clear();
		}
	}

	// Advance progressed time and check if animation ends this frame
	mProgressedTime += dt;
	while (mProgressedTime >= duration)
	{
		// No further animation IDs in queue: stop after this one
		if (mQueuedIds.empty())
		{
			mProgressedTime = duration;
			break;
		}

		// More animations: pop IDs -> load animations -> push animations
		else
		{
			detail::PlaybackScheme<Id>& next = mQueuedIds.front();

			// Check if the entry actually refers to an animation ID (or something else like a callback)
			if (const Id* id = next.checkId())
			{
				const TimedAnimation& nextAnim = mAnimations->getAnimation(*id);
				mPlayingAnimations.push_back(&nextAnim);

				// New progressed time refers to last animation
				mProgressedTime -= duration;
				duration = nextAnim.duration;
			}

			// Check if the ID is further repeated, pop if not
			if (next.consume())
				mQueuedIds.pop();
		}
	}
}

template <class Animated, typename Id>
void Animator<Animated, Id>::animate(Animated& animated) const
{
	if (mPlayingAnimations.empty())
		return;

	// Apply all animations except last one with progress 1.f
	for (std::size_t i = 0; i < mPlayingAnimations.size() - 1; ++i)
		mPlayingAnimations[i]->function(animated, 1.f);

	// Apply last animation with current progress (is 1 if animation ends this frame and no IDs are left in the queue)
	const TimedAnimation* last = mPlayingAnimations.back();
	last->function(animated, mProgressedTime / last->duration);
}

} // namespace thor
