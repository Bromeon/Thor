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

#include <Thor/Graphics/ColorGradient.hpp>

#include <Aurora/Tools/ForEach.hpp>

#include <iterator>
#include <cassert>


namespace thor
{

ColorGradient::ColorGradient()
: mColors()
{
}

sf::Color& ColorGradient::operator[] (float position)
{
	assert(position >= 0.f && position <= 1.f);

	return mColors[position];
}

sf::Color ColorGradient::sampleColor(float position) const
{
	// Make sure the positions 0 and 1 are set
	assert(mColors.count(0.f) && mColors.count(1.f));

	// Find first entry >= position, return color if entry == position
	auto nextColor = mColors.lower_bound(position);
	if (nextColor->first == position)
		return nextColor->second;

	// Find first entry < position
	auto prevColor = std::prev(nextColor);

	// Interpolate color between 2 entries
	float interpolation = (position - prevColor->first) / (nextColor->first - prevColor->first);
	return blendColors(prevColor->second, nextColor->second, interpolation);
}

// ---------------------------------------------------------------------------------------------------------------------------


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

} // namespace thor
