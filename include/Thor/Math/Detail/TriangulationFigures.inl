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

namespace thor
{
namespace detail
{

	bool THOR_API isClockwiseOriented(sf::Vector2f v0, sf::Vector2f v1, sf::Vector2f v2);

} // namespace detail


inline Vertex::Vertex(sf::Vector2f position)
: mPosition(position)
{
}

inline Vertex::Vertex(float x, float y)
: mPosition(x, y)
{
}

inline sf::Vector2f Vertex::getPosition() const
{
	return mPosition;
}

template <class VertexType>
Edge<VertexType>::Edge(const VertexType& startPoint, const VertexType& endPoint)
{
	assert(startPoint.getPosition() != endPoint.getPosition());

	mCorners[0] = &startPoint;
	mCorners[1] = &endPoint;
}

template <class VertexType>
const VertexType& Edge<VertexType>::operator[] (unsigned int cornerIndex) const
{
	return *mCorners[cornerIndex];
}

template <class VertexType>
Triangle<VertexType>::Triangle(const VertexType& v0, const VertexType& v1, const VertexType& v2)
{
	assert(detail::isClockwiseOriented(v0.getPosition(), v1.getPosition(), v2.getPosition()));
	
	mCorners[0] = &v0;
	mCorners[1] = &v1;
	mCorners[2] = &v2;
}

template <class VertexType>
const VertexType& Triangle<VertexType>::operator[] (unsigned int cornerIndex) const
{
	return *mCorners[cornerIndex];
}

} // namespace thor
