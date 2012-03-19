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
/// @brief Class thor::Animation

#ifndef THOR_ANIMATION_HPP
#define THOR_ANIMATION_HPP

#include <Thor/Config.hpp>

#include AURORA_TR1_HEADER(memory)


namespace sf
{

	class Sprite;

} // namespace sf


namespace thor
{

/// @addtogroup Animation
/// @{

/// @brief Abstract base class for animations
/// @details This class defines the interface for concrete animations that affect sf::Sprite. Note that thor::Animation and
///  its derivates do not store the current animation progress. They only specify how an animation runs, i.e. how it changes
///  depending on the progress. The actual progress is computed by thor::Animator. As a result of this separation, multiple
///  animators can refer to a single animation.
class THOR_API Animation
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Shared pointer type to Animation objects
		/// 
		typedef std::tr1::shared_ptr<Animation>		Ptr;
		
		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Applies the animation to a sprite
		/// @param target The sprite who is modified according to the animation.
		/// @param progress Total animation progress, in the interval [0,1].
		virtual void				apply(sf::Sprite& target, float progress) const = 0;
};

/// @}

} // namespace thor

#endif // THOR_ANIMATION_HPP
