
#include <Thor/Input.hpp>
#include <Thor/Graphics/ToString.hpp>
#include <SFML/Window.hpp>
#include <iostream>

// Enumeration for user-defined actions
enum MyAction
{
	Run,
	Shoot,
	Quit,
	Resize,
};

// Callback function for Resize events
void onResize(thor::ActionContext<MyAction> context);

// Callback function for Shoot (mouse click) events
void onShoot(thor::ActionContext<MyAction> context);

int main()
{
	// Create and initialize window
	sf::Window window(sf::VideoMode(400, 300), "Thor Action Demo");
	window.setFramerateLimit(20);
	window.setKeyRepeatEnabled(false);

	// Create thor::ActionMap that maps MyAction values to thor::Action instances
	thor::ActionMap<MyAction> map;
	using thor::Action;

	// Run: Press one of the shift keys and R (realtime input)
	map[Run] = (Action(sf::Keyboard::LShift) || Action(sf::Keyboard::RShift)) && Action(sf::Keyboard::R);

	// Shoot: Press left mouse button or button 2 of joystick number 0 (single events)
	map[Shoot] = Action(sf::Mouse::Left, Action::PressOnce) || Action(thor::Joy(0).button(2), Action::PressOnce);

	// Quit: Release the escape key or click the [X] (single events)
	map[Quit] = Action(sf::Keyboard::Escape, Action::ReleaseOnce) || Action(sf::Event::Closed);

	// Resize: Change window size (single event)
	map[Resize] = Action(sf::Event::Resized);
	
	// Create thor::EventSystem to connect Resize and Shoot actions with callbacks
	thor::ActionMap<MyAction>::CallbackSystem system;
	system.connect(Resize, &onResize);
	system.connect(Shoot, &onShoot);

	// Main loop
	for (;;)
	{
		// Generate new actions (calls window.pollEvent(...))
		map.update(window);

		// Check which actions are currently in effect, react correspondingly
		if (map.isActive(Run))
			std::cout << "Run!" << std::endl;
		if (map.isActive(Quit))
			return 0;

		// Forward actions to callbacks: Invokes onResize() in case of sf::Event::Resized events
		map.invokeCallbacks(system, &window);

		// Update window
		window.display();
	}	
}

void onResize(thor::ActionContext<MyAction> context)
{
	// The sf::Event member variable called type has always the value sf::Event::Resized, as specified in the thor::Action
	// constructor. Since the Resize action has been triggered by an sf::Event (and not by a sf::Keyboard, sf::Mouse or
	// sf::Joystick), we can also be sure that context.event is no null pointer.
	sf::Event event = *context.event;
	std::cout << "Resized!   New size = (" << event.size.width << ", " << event.size.height << ")" << std::endl;
}

void onShoot(thor::ActionContext<MyAction> context)
{
	// context.Window is a pointer to the sf::Window passed to the thor::ActionMap constructor. It can
	// be used for mouse input relative to a window, as follows:
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*context.window);
	std::cout << "Shoot: " << thor::toString(mousePosition) << std::endl;
}