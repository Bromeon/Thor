
#include <Thor/Input/EventSystem.hpp>
#include <iostream>
#include <string>

// Enumeration for unit orders in a strategy game
namespace Command
{
	enum Type
	{
		Move,
		Attack,
		HoldPosition,
	};
}

// A user-defined event class, contains the name and command of a strategy game unit
struct UnitEvent
{
	UnitEvent(const char* unitName, Command::Type order)
	: unitName(unitName)
	, order(order)
	{
	}

	const char* unitName;
	Command::Type order;
};

// Function that returns the corresponding EventId to an Event
Command::Type getEventId(const UnitEvent& event)
{
	return event.order;
}

// Callback for Command::Move
void onMove(const UnitEvent& event)
{
	std::cout << "Unit " << event.unitName << " moves." << std::endl;
}

// Callback for Command::Attack
void onAttack(const UnitEvent& event)
{
	std::cout << "Unit " << event.unitName << " attacks." << std::endl;
}

// Callback for Command::HoldPosition
void onHoldPosition(const UnitEvent& event)
{
	std::cout << "Unit " << event.unitName << " holds its position." << std::endl;
}

int main()
{
	// Create event system with template parameters for Event and EventId
	thor::EventSystem<UnitEvent, Command::Type> system;

	// Connect event identifiers to the listeners
	system.connect(Command::Move,         &onMove);
	system.connect(Command::Attack,       &onAttack);
	system.connect(Command::HoldPosition, &onHoldPosition);

	// Create some events and fire them
	system.triggerEvent( UnitEvent("Tank",       Command::Attack) );
	system.triggerEvent( UnitEvent("Helicopter", Command::Move) );
	system.triggerEvent( UnitEvent("Battleship", Command::Attack) );
	system.triggerEvent( UnitEvent("Battleship", Command::HoldPosition) );
}
