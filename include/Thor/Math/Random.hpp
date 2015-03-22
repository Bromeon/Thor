/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2015 Jan Haller
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
/// @brief Functions for random number generation

#ifndef THOR_RANDOM_HPP
#define THOR_RANDOM_HPP

#include <Thor/Config.hpp>


namespace thor
{

/// @addtogroup Math
/// @{

/// @brief Returns an int random number in the interval [min, max].
/// @pre min <= max
int THOR_API random(int min, int max);

/// @brief Returns an unsigned int random number in the interval [min, max].
/// @pre min <= max
unsigned int THOR_API random(unsigned int min, unsigned int max);

/// @brief Returns a float random number in the interval [min, max].
/// @pre min <= max
float THOR_API random(float min, float max);

/// @brief Returns a float random number in the interval [middle-deviation, middle+deviation].
/// @pre deviation >= 0
float THOR_API randomDev(float middle, float deviation);

/// @brief Sets the seed of the random number generator.
/// @details Setting the seed manually is useful when you want to reproduce a given sequence of random
///  numbers. Without calling this function, the seed is different at each program startup.
void THOR_API setRandomSeed(unsigned long seed);

/// @}

} // namespace thor

#endif // THOR_RANDOM_HPP
