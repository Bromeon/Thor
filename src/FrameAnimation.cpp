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

#include <Thor/Animation/FrameAnimation.hpp>
#include <Aurora/Tools/ForEach.hpp>

#include <algorithm>
#include <cassert>


namespace thor
{

FrameAnimation::Ptr FrameAnimation::create()
{
	return Ptr( new FrameAnimation() );
}

FrameAnimation::FrameAnimation()
: mFrames()
, mNormalized(false)
{
}

void FrameAnimation::addFrame(float relativeDuration, const sf::IntRect& subrect)
{
	addFrame(relativeDuration, thor::ResourcePtr<sf::Texture>(), subrect);
}

void FrameAnimation::addFrame(float relativeDuration, ResourcePtr<const sf::Texture> texture, const sf::IntRect& subrect)
{
	mFrames.push_back(Frame(relativeDuration, texture, subrect));
	mNormalized = false;
}

void FrameAnimation::apply(sf::Sprite& target, float progress) const
{
	assert(!mFrames.empty());
	assert(progress >= 0.f && progress <= 1.f);

	ensureNormalized();
	std::vector<Frame>::const_iterator currentFrame = std::find_if(mFrames.begin(), mFrames.end(), ExhaustTime(progress));
	if (currentFrame == mFrames.end())
		return;

	// Set texture, if necessary - don't adjust sub-rect to whole image
	if (currentFrame->texture)
		target.setTexture(*currentFrame->texture);

	// Set sub-rect
	target.setTextureRect(currentFrame->subrect);
}

void FrameAnimation::ensureNormalized() const
{
	if (mNormalized)
		return;

	float sum = 0.f;
	AURORA_CITR_FOREACH(std::vector<Frame>, mFrames, itr)
		sum += itr->duration;

	AURORA_CITR_FOREACH(std::vector<Frame>, mFrames, itr)
		itr->duration /= sum;

	mNormalized = true;
}

// ---------------------------------------------------------------------------------------------------------------------------


FrameAnimation::Frame::Frame(float duration, ResourcePtr<const sf::Texture> texture, const sf::IntRect& subrect)
: duration(duration)
, texture(texture)
, subrect(subrect)
{
}

FrameAnimation::ExhaustTime::ExhaustTime(float time)
: time(time)
{
}

bool FrameAnimation::ExhaustTime::operator() (const Frame& frame)
{
	time -= frame.duration;
	return time < 0.f;
}

} // namespace thor
