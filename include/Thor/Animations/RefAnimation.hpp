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
/// @brief Class template thor::RefAnimation

#ifndef THOR_REFANIMATION_HPP
#define THOR_REFANIMATION_HPP

namespace thor
{

/// @addtogroup Animations
/// @{

/// @brief Class to reference another animation.
/// @details This class can be used to create an animation that has no functionality on its own, but references another
///  existing animation. By using it, you can avoid copies and change the original animation object, even after it has been
///  added to a thor::Animator. On the other side, you must make sure the original animation stays alive while being referenced.
/// @n@n Usually, you create objects of this class by calling @ref refAnimation() "thor::refAnimation()".
template <typename Animation>
class RefAnimation
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param referenced Animation to reference. Must remain valid during usage.
		explicit					RefAnimation(Animation& referenced);

		/// @brief Animates the object.
		/// @details Forwards the call to the referenced animation.
		template <typename Animated>
		void						operator() (Animated& animated, float progress);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		Animation*					mReferenced;
};

/// @relates RefAnimation
/// @brief Creates an animation that references another one.
/// @param referenced Animation to reference. Must remain valid during usage.
/// @details Creates an animation object that has no functionality on its own, but references another existing animation.
///  By using it, you can avoid copies and change the original animation object, even after it has been added to a
///  thor::Animator. On the other side, you must make sure the original animation stays alive while being referenced.
/// @n@n Example:
/// @code
///  // Create animation
///  thor::FrameAnimation anim;
///
///  // Insert reference into animator
///  thor::Animator<...> animator;
///  animator.addAnimation(..., thor::refAnimation(anim), ...);
///
///  // Later changes will affect animation inside animator
///  anim.addFrame(...);
/// @endcode
template <typename Animation>
RefAnimation<Animation> refAnimation(Animation& referenced);

/// @}

} // namespace thor

#include <Thor/Animations/Detail/RefAnimation.inl>
#endif // THOR_REFANIMATION_HPP
