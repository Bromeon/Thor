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

#include <Thor/Graphics/BigSprite.hpp>
#include <Thor/Graphics/BigTexture.hpp>

#include <Aurora/Tools/ForEach.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cassert>


namespace thor
{

BigSprite::BigSprite()
: sf::Drawable()
, sf::Transformable()
, mSplitSprites()
, mSize(0.f, 0.f)
, mColor(sf::Color::White)
{
}

BigSprite::BigSprite(const BigTexture& texture)
: sf::Drawable()
, sf::Transformable()
, mSplitSprites()
, mSize()
, mColor(sf::Color::White)
{
	setTexture(texture);
}

void BigSprite::setTexture(const BigTexture& texture)
{
	mSize = texture.fillSprites(mColor, mSplitSprites);
}

void BigSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	AURORA_FOREACH(const sf::Sprite& sprite, mSplitSprites)
		target.draw(sprite, states);
}

void BigSprite::setColor(const sf::Color& color)
{
	mColor = color;
	AURORA_FOREACH(sf::Sprite& sprite, mSplitSprites)
		sprite.setColor(color);
}

sf::Color BigSprite::getColor() const
{
	return mColor;
}

sf::FloatRect BigSprite::getLocalBounds() const
{
	return sf::FloatRect(0.f, 0.f, mSize.x, mSize.y);
}

sf::FloatRect BigSprite::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

} // namespace thor
