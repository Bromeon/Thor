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

#include <Thor/Time/Timer.hpp>

#include <algorithm>
#include <cassert>


namespace thor
{

Timer::Timer()
: mStopWatch()
, mLimit(sf::Time::Zero)
{
}

Timer::~Timer()
{
}

sf::Time Timer::getRemainingTime() const
{
	return std::max(mLimit - mStopWatch.getElapsedTime(), sf::Time::Zero);
}

bool Timer::isRunning() const
{
	return mStopWatch.isRunning() && !isExpired();
}

bool Timer::isExpired() const
{
	return mStopWatch.getElapsedTime() >= mLimit;
}

void Timer::start()
{
	mStopWatch.start();
}

void Timer::stop()
{
	mStopWatch.stop();
}

void Timer::reset(sf::Time timeLimit)
{
	assert(timeLimit > sf::Time::Zero);

	mLimit = timeLimit;
	mStopWatch.reset();
}

void Timer::restart(sf::Time timeLimit)
{
	reset(timeLimit);
	start();
}

} // namespace thor
