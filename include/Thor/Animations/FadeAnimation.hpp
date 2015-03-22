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

/// @file
/// @brief Class thor::FadeAnimation

#ifndef THOR_FADEANIMATION_HPP
#define THOR_FADEANIMATION_HPP

#include <Thor/Config.hpp>
#include <Thor/Graphics/UniformAccess.hpp>


namespace thor
{

/// @addtogroup Animations
/// @{

/// @brief Lets an object fade in or out.
/// @details Changes the alpha value of animated objects at the beginning and/or end of the animation.
class THOR_API FadeAnimation
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param inRatio The part of time during which the object is faded @b in. Must be in the interval [0, 1].
		/// @param outRatio The part of time during which the object is faded @b out. Must be in the interval [0, 1-inRatio].
		/// @details Example: Let's say you want an object to fade in during the first quarter of its animation, and to fade out
		///  during the second half of its animation. Therefore, inRatio = 0.25 and outRatio = 0.5.
									FadeAnimation(float inRatio, float outRatio);

		/// @brief Animates the object.
		/// @param animated Object to fade in and/or out.
		/// @param progress Value in [0,1] determining the progress of the animation.
		/// @tparam Animated Type of animated object. The function thor::setAlpha() is invoked for it.
		template <class Animated>
		void						operator() (Animated& animated, float progress) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		float						mInRatio;
		float						mOutRatio;
};

/// @}

// ---------------------------------------------------------------------------------------------------------------------------


template <class Animated>
void FadeAnimation::operator() (Animated& target, float progress) const
{
	if (progress < mInRatio)
		setAlpha(target, static_cast<sf::Uint8>(256 * progress / mInRatio));
	else if (progress > 1.f - mOutRatio)
		setAlpha(target, static_cast<sf::Uint8>(256 * (1.f-progress) / mOutRatio));
}

} // namespace thor

#endif // THOR_FADEANIMATION_HPP
