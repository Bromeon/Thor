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

/// @file
/// @brief Class template thor::Animator

#ifndef THOR_ANIMATOR_HPP
#define THOR_ANIMATOR_HPP

#include <Thor/Config.hpp>
#include <Thor/Animations/AnimationMap.hpp>
#include <Thor/Animations/Playback.hpp>

#include <Aurora/Tools/Optional.hpp>
#include <Aurora/Tools/Algorithms.hpp>

#include <SFML/System/Time.hpp>

#include <queue>
#include <functional>
#include <cassert>
#include <cmath>


namespace thor
{

/// @addtogroup Animations
/// @{

/// @brief Class to animate objects.
/// @details The Animator class takes care of multiple animations which are registered with an ID. The animations can be played
///  at any time, queued for subsequent animations, and repeated or looped as desired. Animator updates the animations' progress
///  and applies them to animated objects. It thus relieves the user of keeping track how far an animation has progressed, when
///  it is finished, and so on.
/// @n@n Each Animator always references an AnimationMap instance, which serves as the storage of animations. The
///  Animator::update() function must be called in every frame in order to update the animation progress.
/// @n@n This class does not provide any methods to query the state and progress of the animations, by design. On one hand, there
///  is not always an unambiguous animation playing -- there may be none, or there may be multiple (when the delta time passed
///  to update() has exceeded one). On the other hand, the intent of animators is to abstract such information from the user, and
///  attempts to retrieve it nonetheless indicate a misunderstanding of Animator's capabilities (e.g. the need to know when an
///  animation has finished, in order to start another). Sequential actions can be queued using play() and queue(); it's even
///  possible to notify the user through callbacks.
template <class Animated, typename Id>
class Animator
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Functor to animate the objects.
		/// @details Signature: <b>void (Animated& animated, float progress)</b>
		///  @arg @a animated is the object being animated.
		///  @arg @a progress is a number in [0,1] determining the animation state.
		typedef std::function<void(Animated&, float)>	AnimationFunction;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Construct from animation map
		/// @param animations Animation map that stores the animation used by this animator. It must stay alive as long as this
		///	 animator uses it.
		explicit					Animator(const AnimationMap<Animated, Id>& animations);

		/// @brief Plays one or more animations, discarding the current queue.
		/// @details This function clears the current playing queue and creates a new one. The behavior of play() is equivalent
		///  to stop() followed by queue().
		/// @n@n Usage:
		/// @code
		/// thor::Animator<Anim, std::string> animator;
		/// animator.play() << "id1" << thor::Playback::repeat("id2", 3);
		/// @endcode
		/// @return Implementation-defined queue object that can be appended to with @c operator<<. Do not store this object.
		AURORA_IMPL_DEF(detail::PlayingQueue<Id>)	play();

		/// @brief Plays one or more animations, appending to current queue.
		/// @details This function appends to the end of the current playing queue. Make sure there is no looped animation
		///  except at the end.
		/// @n@n Usage:
		/// @code
		/// thor::Animator<Anim, std::string> animator;
		/// animator.queue() << "id1" << thor::Playback::repeat("id2", 3);
		/// @endcode
		/// @return Implementation-defined queue object that can be appended to with @c operator<<. Do not store this object.
		AURORA_IMPL_DEF(detail::PlayingQueue<Id>)	queue();

		/// @brief Stops all playing and queued animations.
		/// @details Animations currently playing are interrupted, and the queue of pending animations is cleared. After this
		///  call, this object effectively behaves like a newly constructed Animator instance.
		void						stop();

		/// @brief Updates the animator's progress. You should call this method each frame.
		/// @details Progresses the current animation by @c dt. If its duration is exceeded, the next animation in the queue is
		///  loaded. This method guarantees that <b>every</b> animation in the queue is updated at least once, and that the last
		///  %update occurs with a progress of 1. Therefore, you can design your animations in a way that leaves the animated
		///  object in a meaningful state at the end of the animation.
		/// @n@n If no animation is currently playing or queued, nothing happens.
		/// @param dt Frame time.
		void						update(sf::Time dt);

		/// @brief Applies the stored animations to an object.
		/// @details If no animation is active, the animated object is left unchanged. If multiple animations have been finished
		///  during the last update() call, each of them is applied in order. The last time an animation is applied, its progress
		///  is guaranteed to be 1.
		/// @param animated Object which is animated by the current animation.
		void						animate(Animated& animated) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		typedef detail::TimedAnimation<Animated>	TimedAnimation;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		const AnimationMap<Animated, Id>*		mAnimations;
		std::queue<detail::PlaybackScheme<Id>>	mQueuedIds;
		std::vector<const TimedAnimation*>		mPlayingAnimations;
		sf::Time								mProgressedTime;
};

/// @}

} // namespace thor

#include <Thor/Animations/Detail/Animator.inl>
#endif // THOR_ANIMATOR_HPP
