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
namespace detail
{

	bool THOR_API				isClockwiseOriented(sf::Vector2f v0, sf::Vector2f v1, sf::Vector2f v2);

	template <typename V>
	sf::Vector2f				getVertexPosition(const V& vertex);

} // namespace detail

// ---------------------------------------------------------------------------------------------------------------------------


template <typename V>
Edge<V>::Edge(V& corner0, V& corner1)
{
	assert(detail::getVertexPosition(corner0) != detail::getVertexPosition(corner1));

	mCorners[0] = &corner0;
	mCorners[1] = &corner1;
}

template <typename V>
V& Edge<V>::operator[] (std::size_t cornerIndex)
{
	return *mCorners[cornerIndex];
}

template <typename V>
const V& Edge<V>::operator[] (std::size_t cornerIndex) const
{
	return *mCorners[cornerIndex];
}

// ---------------------------------------------------------------------------------------------------------------------------


template <typename V>
Triangle<V>::Triangle(V& corner0, V& corner1, V& corner2)
{
	assert(detail::isClockwiseOriented(
		detail::getVertexPosition(corner0),
		detail::getVertexPosition(corner1),
		detail::getVertexPosition(corner2)));

	mCorners[0] = &corner0;
	mCorners[1] = &corner1;
	mCorners[2] = &corner2;
}

template <typename V>
V& Triangle<V>::operator[] (std::size_t cornerIndex)
{
	return *mCorners[cornerIndex];
}

template <typename V>
const V& Triangle<V>::operator[] (std::size_t cornerIndex) const
{
	return *mCorners[cornerIndex];
}

} // namespace thor
