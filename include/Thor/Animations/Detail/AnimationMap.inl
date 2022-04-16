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

namespace thor
{

template <class Animated, typename Id>
AnimationMap<Animated, Id>::AnimationMap()
: mAnimationMap()
{
}

template <class Animated, typename Id>
void AnimationMap<Animated, Id>::addAnimation(Id id, std::function<void(Animated&, float)> animation, sf::Time duration)
{
	assert(mAnimationMap.find(id) == mAnimationMap.end());
	assert(duration > sf::Time::Zero);

	mAnimationMap.insert(std::make_pair(std::move(id), TimedAnimation(std::move(animation), duration)));
}

template <class Animated, typename Id>
const typename AnimationMap<Animated, Id>::TimedAnimation& AnimationMap<Animated, Id>::getAnimation(const Id& id) const
{
	auto itr = mAnimationMap.find(id);
	assert(itr != mAnimationMap.end());

	return itr->second;
}

} // namespace thor
