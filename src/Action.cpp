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

#include <Thor/Input/Action.hpp>

#include <algorithm>
#include <cassert>


namespace thor
{

Action::Action()
: mOperation()
{
}

Action::Action(sf::Keyboard::Key key, ActionType action)
: mOperation()
{
	switch (action)
	{
		case Hold:
			mOperation = aurora::makeCopied<detail::RealtimeKeyLeaf>(key);
			break;

		case PressOnce:
		case ReleaseOnce:
			mOperation = aurora::makeCopied<detail::EventKeyLeaf>(key, action == PressOnce);
			break;
	}
}

Action::Action(sf::Mouse::Button mouseButton, ActionType action)
: mOperation()
{
	switch (action)
	{
		case Hold:
			mOperation = aurora::makeCopied<detail::RealtimeMouseLeaf>(mouseButton);
			break;

		case PressOnce:
		case ReleaseOnce:
			mOperation = aurora::makeCopied<detail::EventMouseLeaf>(mouseButton, action == PressOnce);
			break;
	}
}

Action::Action(JoystickButton joystick, ActionType action)
: mOperation()
{
	switch (action)
	{
		case Hold:
			mOperation = aurora::makeCopied<detail::RealtimeJoystickButtonLeaf>(joystick);
			break;

		case PressOnce:
		case ReleaseOnce:
			mOperation = aurora::makeCopied<detail::EventJoystickLeaf>(joystick, action == PressOnce);
			break;
	}
}

Action::Action(JoystickAxis joystickAxis)
: mOperation(aurora::makeCopied<detail::RealtimeJoystickAxisLeaf>(joystickAxis))
{
}

Action::Action(sf::Event::EventType eventType)
: mOperation(aurora::makeCopied<detail::MiscEventLeaf>(eventType))
{
}

bool Action::isActive(const detail::EventBuffer& buffer) const
{
	return mOperation->isActionActive(buffer);
}

bool Action::isActive(const detail::EventBuffer& buffer, detail::ActionResult& out) const
{
	return mOperation->isActionActive(buffer, out);
}

Action::Action(detail::ActionNode::CopiedPtr operation)
{
	mOperation = std::move(operation);
}

// TODO: Wait until aurora::CopiedPtr implements move derived-to-base more efficiently
Action operator|| (const Action& lhs, const Action& rhs)
{
	return Action( aurora::makeCopied<detail::OrNode>(lhs.mOperation, rhs.mOperation) );
}

Action operator&& (const Action& lhs, const Action& rhs)
{
	return Action( aurora::makeCopied<detail::AndNode>(lhs.mOperation, rhs.mOperation) );
}

Action operator! (const Action& action)
{
	return Action( aurora::makeCopied<detail::NotNode>(action.mOperation) );
}

Action eventAction(std::function<bool(const sf::Event&)> filter)
{
	return Action( aurora::makeCopied<detail::CustomEventLeaf>(std::move(filter)) );
}

Action realtimeAction(std::function<bool()> filter)
{
	return Action( aurora::makeCopied<detail::CustomRealtimeLeaf>(std::move(filter)) );
}

} // namespace thor
