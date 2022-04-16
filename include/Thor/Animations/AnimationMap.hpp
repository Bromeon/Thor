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
/// @brief Class template thor::AnimationMap

#ifndef THOR_ANIMATIONMAP_HPP
#define THOR_ANIMATIONMAP_HPP

#include <Thor/Config.hpp>

#include <SFML/System/Time.hpp>

#include <map>
#include <functional>
#include <cassert>
#include <cmath>


namespace thor
{
namespace detail
{

	template <class Animated>
	struct TimedAnimation
	{
		// @brief Functor to animate the objects.
		// @details Signature: <b>void (Animated& animated, float progress)</b>
		//  @arg @a animated is the object being animated.
		//  @arg @a progress is a number in [0,1] determining the animation state.
		typedef std::function<void(Animated&, float)>	AnimationFunction;

		TimedAnimation(AnimationFunction animation, sf::Time duration)
		: function(std::move(animation))
		, duration(duration)
		{
		}

		AnimationFunction			function;
		sf::Time					duration;
	};

} // namespace detail


/// @addtogroup Animations
/// @{

template <class Animated, typename Id>
class Animator;

/// @brief Class that stores multiple animations.
/// @details The only purpose of this class is to define and store different animations. Each animation is identified by an ID
///  (string, enum, ...).
/// @tparam Id Identifier to distinguish animations. This type acts as key in the map and must support @c operator<.
/// @tparam Animated Class that is affected by animations.
/// @warning Instances of this class must live as long as any Animator instance references them.
template <class Animated, typename Id>
class AnimationMap
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		typedef detail::TimedAnimation<Animated>							TimedAnimation;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		///
									AnimationMap();

		/// @brief Registers an animation with a given identifier.
		/// @details It is explicitly allowed to add animations while this AnimationMap is in use by Animator instances. Their
		///  playback will not be affected by doing so. Furthermore, the same animation can be added multiple times (with
		///  different durations), as long as a distinct ID is used.
		/// @param id Value that identifies the animation (must not be registered yet).
		/// @param animation Animation to add to the animator. The animation is copied; if you want to insert a reference
		///  instead, use the function refAnimation(). The signature is <tt>void(Animated& animated, float progress)</tt>, where:
		///  @arg @c animated is the object being animated.
		///  @arg @c progress is a number in [0,1] determining the animation state.
		/// @param duration Duration of the animation (> 0).
		void						addAnimation(Id id, std::function<void(Animated&, float)> animation, sf::Time duration);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions (accessible through friend)
	private:
		const TimedAnimation&		getAnimation(const Id& id) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::map<Id, TimedAnimation>	mAnimationMap;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Friends
	friend class Animator<Animated, Id>;
};

/// @}

} // namespace thor

#include <Thor/Animations/Detail/AnimationMap.inl>
#endif // THOR_ANIMATIONMAP_HPP
