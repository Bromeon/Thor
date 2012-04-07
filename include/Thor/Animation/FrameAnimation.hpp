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
/// @brief Class thor::FrameAnimation

#ifndef THOR_FRAMEANIMATION_HPP
#define THOR_FRAMEANIMATION_HPP

#include <Thor/Animation/Animation.hpp>
#include <Thor/Config.hpp>

#include <SFML/Graphics/Sprite.hpp>

#include <vector>


namespace thor
{

/// @addtogroup Animation
/// @{

/// @brief Changes a sprite's subrect over time. 
/// @details This class stores multiple frames that represent the sub-rectangle of a sf::Texture. The resulting animation consists
///  of a sequence of frames that are drawn one after another.
class THOR_API FrameAnimation : public Animation
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public types
	public:
		/// @brief Shared pointer type to FrameAnimation objects
		/// 
		typedef std::shared_ptr<FrameAnimation>		Ptr;
		
		
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public static member functions
	public:
		/// @brief Returns a shared pointer to a newly created FrameAnimation.
		/// @copydetails FrameAnimation::FrameAnimation()
		static Ptr					create();
		
		
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

		/// @brief Adds a frame to the animation, changes both texture and sub-rect.
		/// @param relativeDuration Duration of the frame relative to the other durations.
		/// @param texture Smart pointer to sf::Texture that is used for the new frame.
		/// @param subrect %Rectangle of the sf::Texture that is used for the new frame.
		void						addFrame(float relativeDuration, std::shared_ptr<const sf::Texture> texture,
										const sf::IntRect& subrect);

		virtual void				apply(sf::Sprite& target, float progress) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		// Frame with sub-rectangle and duration
		struct Frame
		{
											Frame(float duration, std::shared_ptr<const sf::Texture> texture, const sf::IntRect& subrect);

			mutable float					duration;
			sf::IntRect						subrect;
			std::shared_ptr<const sf::Texture>	texture;
		};

		// Functor to find current frame
		struct ExhaustTime
		{
			explicit					ExhaustTime(float time);
			bool						operator() (const Frame& frame);

			float						time;
		};


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private member functions
	private:
		void							ensureNormalized() const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::vector<Frame>			mFrames;
		mutable bool				mNormalized;
};

/// @}

} // namespace thor

#endif // THOR_FRAMEANIMATION_HPP
