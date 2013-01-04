/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2013 Jan Haller
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
/// @brief Color-related functionality and class thor::ColorGradient

#ifndef THOR_COLORS_HPP
#define THOR_COLORS_HPP

#include <Thor/Graphics/Detail/ColorImpl.hpp>
#include <Thor/Config.hpp>

#include <SFML/Graphics/Color.hpp>


namespace thor
{
namespace detail
{

	class ColorGradientConvertible;

} // namespace detail


/// @addtogroup Graphics
/// @{

/// @brief Blends the colors @a firstColor and @a secondColor, according to the given interpolation.
/// @param firstColor The first color to blend.
/// @param secondColor The second color to blend.
/// @param interpolation Specifies how much of every color is taken. If it has value 0, the first color is
///  returned; with value 1, the second color is returned. Every value in ]0,1[ leads to a mixture. Other values
///  are not allowed.
sf::Color THOR_API			blendColors(const sf::Color& firstColor, const sf::Color& secondColor, float interpolation);

/// @relates ColorGradient
/// @brief Creates a color gradient based on a list of colors and transitions.
/// @details A color gradient consists of a list of N different colors and N-1 corresponding transition times (which
///  specify how long it takes to morph one color to the other). So, the gradient is an alternating chain of colors
///  and relative transition times, this is also the way you generate it:
///  @code
///   thor::ColorGradient gradient = thor::CreateGradient(sf::Color::Red)(3.f)(sf::Color::Blue)(1.f)(sf::Color::Cyan);
///  @endcode
///  In the above example, you have 3 colors and therefore 2 transitions. The gradient from red to blue takes 3 time
///  units (the number between the colors), the one from blue to cyan takes 1 time unit. As mentioned, the transition
///  times are relative to each other, i.e. the two partial gradients make up 75% and 25% of the whole gradient.
///  Like this, you can build complexly composed gradients using a simple syntax.
/// @return Implementation-defined type that is convertible to thor::ColorGradient. Do @b not use the type directly.
detail::ColorGradientConvertible THOR_API	createGradient(const sf::Color& color);

/// @brief Class to implement color gradients
/// @details This class can represent color gradients and calculate the color at a given point. A color gradient
///  consists of a list of N different colors and N-1 corresponding transition times (which specify how long it
///  takes to morph one color to the other).
class THOR_API ColorGradient
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Implicit constructor from single color
		/// @details Creates a gradient that contains only a single color.
									ColorGradient(const sf::Color& color);

		/// @brief Returns the color at a given progress of the gradient.
		/// @param interpolation Number in [0.f, 1.f] that stands for the point at which you want to access a color from
		///  the gradient. When you pass 0 (1), the color at the very beginning (end) is returned. If the gradient
		///  consists of a single color, it is always returned (independently of @a interpolation).
		sf::Color					getColor(float interpolation) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Implementation details
	public:
		// Construct from temporary color gradient
									ColorGradient(detail::ColorGradientConvertible origin);


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		// Normalizes the transition times so that their sum is 1
		void						normalize();


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::vector<sf::Color>		mColors;
		std::vector<float>			mTransitionTimes;
};


/// @}

} // namespace thor

#endif // THOR_COLORS_HPP
