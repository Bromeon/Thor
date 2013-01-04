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

#ifndef THOR_COLORIMPL_HPP
#define THOR_COLORIMPL_HPP

#include <Thor/Config.hpp>

#include <SFML/Graphics/Color.hpp>

#include <vector>


namespace thor
{

class ColorGradient;


namespace detail
{

	class ColorGradientConvertible;


	// Helper class for color gradients
	// Transition time step
	class THOR_API ColorGradientTransition
	{
		public:
			// Constructor
			explicit						ColorGradientTransition(ColorGradientConvertible& parent);

			// Call operator to append new color to chain
			ColorGradientConvertible&		operator() (const sf::Color& color);


		private:
			ColorGradientConvertible*		mParent;
	};


	// Helper class for color gradients
	// Color step
	class THOR_API ColorGradientConvertible
	{
		public:
			// Constructor
			explicit					ColorGradientConvertible(const sf::Color& color);

			// Call operator to append new transition to chain
			ColorGradientTransition&	operator() (float transitionTime);

			// Add color
			void						addColor(const sf::Color& color);


		private:
			ColorGradientTransition		mChild;

			std::vector<sf::Color>		mColors;
			std::vector<float>			mTransitionTimes;


		friend class thor::ColorGradient;
	};

} // namespace detail
} // namespace thor

#endif // THOR_COLORIMPL_HPP
