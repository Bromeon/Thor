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
/// @brief Class thor::ColorAnimation

#ifndef THOR_COLORANIMATION_HPP
#define THOR_COLORANIMATION_HPP

#include <Thor/Config.hpp>
#include <Thor/Graphics/ColorGradient.hpp>
#include <Thor/Graphics/UniformAccess.hpp>


namespace thor
{

/// @addtogroup Animations
/// @{

/// @brief Changes an object's color smoothly over time.
/// @details This class stores a ColorGradient which is applied to animated objects.
class THOR_API ColorAnimation
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Constructor
		/// @param gradient The color gradient affecting the objects. Can also be a single sf::Color.
		explicit					ColorAnimation(const ColorGradient& gradient);

		/// @brief Animates the object.
		/// @param animated Object to colorize according to the color gradient.
		/// @param progress Value in [0,1] determining the progress of the animation.
		/// @tparam Animated Type of animated object. The function thor::setColor() is invoked for it.
		template <class Animated>
		void						operator() (Animated& animated, float progress) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		ColorGradient				mGradient;
};

/// @}

// ---------------------------------------------------------------------------------------------------------------------------


template <class Animated>
void ColorAnimation::operator() (Animated& target, float progress) const
{
	setColor(target, mGradient.sampleColor(progress));
}

} // namespace thor

#endif // THOR_COLORANIMATION_HPP
