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
/// @brief Class thor::ColorGradient

#ifndef THOR_COLORGRADIENT_HPP
#define THOR_COLORGRADIENT_HPP

#include <Thor/Config.hpp>

#include <SFML/Graphics/Color.hpp>

#include <map>


namespace thor
{

/// @addtogroup Graphics
/// @{

/// @brief Class to implement color gradients
/// @details This class can represent color gradients and calculate the interpolated color at a given point. A color gradient
///  consists of a list of positions (float values in [0, 1]) and corresponding colors. At each position, a color is
///  sampled exactly; between two positions, a color is interpolated linearly.
class THOR_API ColorGradient
{
	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		/// @details Creates an empty, invalid color gradient. To initialize it correctly, set the colors via
		///  ColorGradient::operator[].
									ColorGradient();

		/// @brief Inserts a color to the gradient.
		/// @param position Number in [0, 1] that specifies a position in the gradient.
		/// @details A map-like syntax allows you to specify the color at a certain position. For example, the following code
		///  creates a gradient consisting of two parts: Red to blue and blue to cyan. The first partial gradient (red to blue)
		///  takes up 80% of the whole gradient.
		/// @code
		///  thor::ColorGradient gradient;
		///  gradient[0.0f] = sf::Color::Red;
		///  gradient[0.8f] = sf::Color::Blue;
		///  gradient[1.0f] = sf::Color::Cyan;
		/// @endcode
		/// @warning At least both positions 0.f and 1.f must be set before a gradient is valid.
		sf::Color&					operator[] (float position);

		/// @brief Interpolates a color in the gradient.
		/// @param position Number in [0, 1] that specifies a position in the gradient.
		///  When you pass 0 (1), the color at the very beginning (end) is returned.
		sf::Color					sampleColor(float position) const;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		std::map<float, sf::Color>	mColors;
};

/// @relates ColorGradient
/// @brief Blends the colors @a firstColor and @a secondColor, according to the given interpolation.
/// @param firstColor The first color to blend.
/// @param secondColor The second color to blend.
/// @param interpolation Specifies how much of every color is taken. If it has value 0, the first color is
///  returned; with value 1, the second color is returned. Every value in ]0,1[ leads to a mixture. Other values
///  are not allowed.
sf::Color THOR_API			blendColors(const sf::Color& firstColor, const sf::Color& secondColor, float interpolation);


/// @}

} // namespace thor

#endif // THOR_COLORGRADIENT_HPP
