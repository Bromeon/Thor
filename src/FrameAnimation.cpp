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


namespace thor
{

FrameAnimation::FrameAnimation()
: mFrames()
, mNormalized(false)
{
}

void FrameAnimation::addFrame(float relativeDuration, const sf::IntRect& subrect)
{
	mFrames.push_back(detail::Frame(relativeDuration, subrect));
	mNormalized = false;
}

void FrameAnimation::ensureNormalized() const
{
	if (mNormalized)
		return;

	float sum = 0.f;
	AURORA_CITR_FOREACH(std::vector<detail::Frame>, mFrames, itr)
		sum += itr->duration;

	AURORA_CITR_FOREACH(std::vector<detail::Frame>, mFrames, itr)
		itr->duration /= sum;

	mNormalized = true;
}

} // namespace thor
