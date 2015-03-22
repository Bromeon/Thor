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
/// @brief Class template thor::TrigonometricTraits and some trigonometry-related functions and constants.

#ifndef THOR_TRIGONOMETRY_HPP
#define THOR_TRIGONOMETRY_HPP

#include <Thor/Config.hpp>

#include <cmath>


namespace thor
{

/// @addtogroup Math
/// @{

/// @struct thor::TrigonometricTraits
/// @brief Trigonometric traits template
/// @details This template can be specialized in order to implement trigonometric constants and functions
///  for arbitrary types. Some of the definitions are required by the vector algebra functions. Thor supplies
///  predefined specializations for float, double and long double.@n@n
/// To define your own traits, specialize the template inside namespace @a thor. The full specialization
///  shall contain the following public static methods. You don't have to define all of them, depending on
///  the functionality you need. For example, computing the vector length requires sqrt(), while the polar
///  angle requires arcTan2(). @n
/// @arg T sin(T deg)
///	@arg T cos(T deg)
///	@arg T tan(T deg)
///	@arg T arcSin(T value)
///	@arg T arcCos(T value)
///	@arg T arcTan2(T valY, T valX)
///	@arg T sqrt(T value)
///	@arg T %pi()
///	@arg T radToDeg(T rad)
///	@arg T degToRad(T deg)
///
/// @attention All trigonometric functions take and return degrees, @b NOT radians.
template <typename T>
struct TrigonometricTraits
{
};

// Trigonometric traits: Specialization for float
template <>
struct TrigonometricTraits<float>
{
	typedef float Type;

	static Type sin(Type deg)					{ return std::sin(degToRad(deg));			}
	static Type cos(Type deg)					{ return std::cos(degToRad(deg));			}
	static Type tan(Type deg)					{ return std::tan(degToRad(deg));			}
	static Type arcSin(Type value)				{ return radToDeg(std::asin(value));		}
	static Type arcCos(Type value)				{ return radToDeg(std::acos(value));		}
	static Type arcTan2(Type valY, Type valX)	{ return radToDeg(std::atan2(valY, valX));	}
	static Type sqrt(Type value)				{ return std::sqrt(value);					}

	static Type pi()							{ return 3.141592653589793238462643383f;	}
	static Type radToDeg(Type rad)				{ return 180 / pi() * rad;					}
	static Type degToRad(Type deg)				{ return pi() / 180 * deg;					}
};

// Trigonometric traits: Specialization for double
template <>
struct TrigonometricTraits<double>
{
	typedef double Type;

	static Type sin(Type deg)					{ return std::sin(degToRad(deg));			}
	static Type cos(Type deg)					{ return std::cos(degToRad(deg));			}
	static Type tan(Type deg)					{ return std::tan(degToRad(deg));			}
	static Type arcSin(Type value)				{ return radToDeg(std::asin(value));		}
	static Type arcCos(Type value)				{ return radToDeg(std::acos(value));		}
	static Type arcTan2(Type valY, Type valX)	{ return radToDeg(std::atan2(valY, valX));	}
	static Type sqrt(Type value)				{ return std::sqrt(value);					}

	static Type pi()							{ return 3.141592653589793238462643383;		}
	static Type radToDeg(Type rad)				{ return 180 / pi() * rad;					}
	static Type degToRad(Type deg)				{ return pi() / 180 * deg;					}
};

// Trigonometric traits: Specialization for long double
template <>
struct TrigonometricTraits<long double>
{
	typedef long double Type;

	static Type sin(Type deg)					{ return std::sin(degToRad(deg));			}
	static Type cos(Type deg)					{ return std::cos(degToRad(deg));			}
	static Type tan(Type deg)					{ return std::tan(degToRad(deg));			}
	static Type arcSin(Type value)				{ return radToDeg(std::asin(value));		}
	static Type arcCos(Type value)				{ return radToDeg(std::acos(value));		}
	static Type arcTan2(Type valY, Type valX)	{ return radToDeg(std::atan2(valY, valX));	}
	static Type sqrt(Type value)				{ return std::sqrt(value);					}

	static Type pi()							{ return 3.141592653589793238462643383l;	}
	static Type radToDeg(Type rad)				{ return 180 / pi() * rad;					}
	static Type degToRad(Type deg)				{ return pi() / 180 * deg;					}
};

// ---------------------------------------------------------------------------------------------------------------------------


/// @brief Converts radians to degrees.
///
template <typename T>
T toDegree(T radian)
{
	return TrigonometricTraits<T>::radToDeg(radian);
}

/// @brief Converts degrees to radians.
///
template <typename T>
T toRadian(T degree)
{
	return TrigonometricTraits<T>::degToRad(degree);
}

/// @brief The number %Pi (3.1415...)
/// @details Pi has the same value as TrigonometricTraits<float>::pi().
extern const float THOR_API Pi;

/// @}

} // namespace thor

#endif // THOR_TRIGONOMETRY_HPP
