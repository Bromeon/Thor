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

namespace thor
{

template <typename T>
PolarVector2<T>::PolarVector2()
: r()
, phi()
{
}

template <typename T>
PolarVector2<T>::PolarVector2(T radius, T angle)
: r(radius)
, phi(angle)
{
}

template <typename T>
PolarVector2<T>::PolarVector2(const sf::Vector2<T>& vector)
: r(length(vector))
, phi(vector == sf::Vector2<T>() ? 0.f : polarAngle(vector))
{
}

template <typename T>
PolarVector2<T>::operator sf::Vector2<T>() const
{
	return sf::Vector2<T>(
		r * TrigonometricTraits<T>::cos(phi),
		r * TrigonometricTraits<T>::sin(phi));
}

template <typename T>
T length(const PolarVector2<T>& vector)
{
	return vector.r;
}

template <typename T>
T polarAngle(const PolarVector2<T>& vector)
{
	return vector.phi;
}

} // namespace thor
