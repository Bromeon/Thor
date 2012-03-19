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

#include <Thor/Multimedia/Colors.hpp>
#include <Aurora/Tools/ForEach.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <numeric>
#include <cassert>


namespace thor
{

extern const sf::Color Transparency = sf::Color(0, 0, 0, 0);

sf::Color blendColors(const sf::Color& firstColor, const sf::Color& secondColor, float interpolation)
{
	assert(interpolation >= 0.f && interpolation <= 1.f);

	float firstPart = 1.f - interpolation;
	
	return sf::Color(
		static_cast<sf::Uint8>(firstPart * firstColor.r + interpolation * secondColor.r),
		static_cast<sf::Uint8>(firstPart * firstColor.g + interpolation * secondColor.g),
		static_cast<sf::Uint8>(firstPart * firstColor.b + interpolation * secondColor.b),
		static_cast<sf::Uint8>(firstPart * firstColor.a + interpolation * secondColor.a));
}

detail::ColorGradientConvertible createGradient(const sf::Color& color)
{
	return detail::ColorGradientConvertible(color);
}

// ---------------------------------------------------------------------------------------------------------------------------


ColorGradient::ColorGradient(const sf::Color& color)
: mColors(1, color)
, mTransitionTimes()
{
}

ColorGradient::ColorGradient(detail::ColorGradientConvertible origin)
: mColors()
, mTransitionTimes()
{
	mColors.swap(origin.mColors);
	mTransitionTimes.swap(origin.mTransitionTimes);

	normalize();
}

sf::Color ColorGradient::getColor(float interpolation) const
{
	assert(interpolation >= 0.f && interpolation <= 1.f);

	// If the gradient consists of a single color, return it independently of when
	if (mColors.size() == 1)
		return mColors[0];

	// Find out to which transition the parameter belongs
	unsigned int index = 0;
	float sum = 0.f;
	AURORA_CITR_FOREACH(std::vector<float>, mTransitionTimes, itr)
	{
		sum += *itr;
		if (sum >= interpolation)
		{
			sum -= *itr;
			break;
		}

		++index;
	}

	// Return interpolation of colors at each end of the transition
	return blendColors(mColors[index], mColors[index+1], (interpolation - sum) / mTransitionTimes[index]);
}

void ColorGradient::normalize()
{
	float sum = std::accumulate(mTransitionTimes.begin(), mTransitionTimes.end(), 0.f);

	AURORA_ITR_FOREACH(std::vector<float>, mTransitionTimes, itr)
		*itr /= sum;
}

} // namespace thor
