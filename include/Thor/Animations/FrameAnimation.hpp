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
/// @brief Class thor::FrameAnimation

#ifndef THOR_FRAMEANIMATION_HPP
#define THOR_FRAMEANIMATION_HPP

#include <Thor/Config.hpp>
#include <Aurora/Tools/ForEach.hpp>

#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include <cassert>


namespace thor
{
namespace detail
{

	// Class that stores a single frame of FrameAnimation
	struct Frame
	{
		Frame(float duration, const sf::IntRect& subrect)
		: duration(duration)
		, subrect(subrect)
		, origin()
		, applyOrigin(false)
		{
		}

		Frame(float duration, const sf::IntRect& subrect, sf::Vector2f origin)
		: duration(duration)
		, subrect(subrect)
		, origin(origin)
		, applyOrigin(true)
		{
		}

		mutable float			duration;
		sf::IntRect				subrect;
		sf::Vector2f			origin;
		bool					applyOrigin;
	};

} // namespace detail

// ---------------------------------------------------------------------------------------------------------------------------


/// @addtogroup Animations
/// @{

/// @brief Changes a sprite's subrect over time.
/// @details This class stores multiple frames that represent the sub-rectangle of a texture. The resulting animation consists
///  of a sequence of frames that are drawn one after another.
class THOR_API FrameAnimation
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		///
									FrameAnimation();

		/// @brief Adds a frame to the animation, changes only the sub-rect.
		/// @param relativeDuration Duration of the frame relative to the other durations.
		/// @param subrect %Rectangle of the sf::Texture that is used for the new frame.
		void						addFrame(float relativeDuration, const sf::IntRect& subrect);

		/// @brief Adds a frame to the animation, changes sub-rect and sprite origin.
		/// @param relativeDuration Duration of the frame relative to the other durations.
		/// @param subrect %Rectangle of the sf::Texture that is used for the new frame.
		/// @param origin Position of the coordinate system origin. Is useful when frames have rectangles of different sizes.
		void						addFrame(float relativeDuration, const sf::IntRect& subrect, sf::Vector2f origin);

		/// @brief Animates the object.
		/// @param animated Object to animate.
		/// @param progress Value in [0,1] determining the progress of the animation.
		/// @tparam Animated Class with member functions <i>void setTextureRect(sf::IntRect)</i> and
		///  <i>void setOrigin(sf::Vector2f)</i>, for example sf::Sprite.
		template <class Animated>
		void						operator() (Animated& animated, float progress) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		void						ensureNormalized() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::vector<detail::Frame>	mFrames;
		mutable bool				mNormalized;
};

/// @}

// ---------------------------------------------------------------------------------------------------------------------------


template <class Animated>
void FrameAnimation::operator() (Animated& target, float progress) const
{
	assert(!mFrames.empty());
	assert(progress >= 0.f && progress <= 1.f);

	ensureNormalized();
	AURORA_FOREACH(const detail::Frame& frame, mFrames)
	{
		progress -= frame.duration;

		// Must be <= and not <, to handle case (progress == frame.duration == 1) correctly
		if (progress <= 0.f)
		{
			target.setTextureRect(frame.subrect);
			if (frame.applyOrigin)
				target.setOrigin(frame.origin);

			break;
		}
	}
}

} // namespace thor

#endif // THOR_FRAMEANIMATION_HPP
