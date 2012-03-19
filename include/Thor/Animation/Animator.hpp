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

/// @file
/// @brief Class thor::Animator

#ifndef THOR_ANIMATOR_HPP
#define THOR_ANIMATOR_HPP

#include <Thor/Animation/Animation.hpp>
#include <Thor/Config.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <map>
#include <string>


namespace thor
{

/// @addtogroup Animation
/// @{

/// @brief Class that stores the progress of a sprite's animation 
/// @details The Animator class takes care of multiple Animation instances which are registered by a string. The registered
///  animations can be played at any time. Animator updates their progress and applies it to sf::Sprite instances.
class THOR_API Animator
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		/// 
									Animator();

		/// @brief Registers an animation with a given name.
		/// @param name String to assign to the animation (may not be registered yet).
		/// @param animation Animation to add to the animator.
		/// @param duration Duration of the animation.
		void						addAnimation(const std::string& name, Animation::Ptr animation, sf::Time duration);

		/// @brief Plays the animation with the given name.
		/// @param name Name of the animation to play (std::string). An animation with this name must have been added before.
		/// @param loop True if the animation is played repeatedly.
		void						playAnimation(const std::string& name, bool loop = false);

		/// @brief Interrupts the animation that is currently active.
		/// @details If a static frame has been specified, it is shown. Otherwise, the last visible frame of the stopped
		///  animation is shown.
		void						stopAnimation();

		/// @brief Updates the animator's progress. You should call this method each frame.
		/// @param dt Frame time.
		void						update(sf::Time dt);

		/// @brief Applies the stored animations to a sf::Sprite.
		/// @param target Sprite whose subrect is changed according to the current animation. If no animation is active,
		///  the sprite is left unchanged or set to the static frame, if previously specified.
		void						animate(sf::Sprite& target) const;

		/// @brief Sets an animation that is active when all others are stopped.
		/// @param animation Default animation to set. Will be played in a loop if no other animation is currently active.
		///  @a animation can be empty to reset the default animation. In this case, the sprite is not affected when
		///  no animation is playing.
		/// @param duration Duration of the animation.
		void						setDefaultAnimation(Animation::Ptr animation, sf::Time duration);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		typedef std::pair<Animation::Ptr, sf::Time>		ScaledAnimation;
		typedef std::map<std::string, ScaledAnimation>	AnimationMap;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		void						playAnimation(ScaledAnimation& animation, bool loop);

			
	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		AnimationMap				mAnimationMap;
		const ScaledAnimation*		mPlayingAnimation;
		ScaledAnimation				mDefaultAnimation;
		float						mProgress;
		bool						mLoop;
};

/// @}

} // namespace thor

#endif // THOR_ANIMATOR_HPP
