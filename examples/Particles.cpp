
#include <Thor/Particles.hpp>
#include <Thor/Vectors/PolarVector.hpp>
#include <Thor/Math/Distributions.hpp>
#include <SFML/Graphics.hpp>


// Functor that returns the mouse position in float coordinates
struct MousePosition
{
	MousePosition(sf::Window& window)
	: window(window)
	{
	}

	sf::Vector2f operator() ()
	{
		return sf::Vector2f(sf::Mouse::getPosition(window));
	}

	sf::Window& window;
};


int main()
{
	// Create window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Thor Particles");
	
	// Load image and initialize sprite
	std::shared_ptr<sf::Texture> texture(new sf::Texture());
	if (!texture->loadFromFile("Media/particle.png"))
		return EXIT_FAILURE;
	
	// Create emitter
	thor::UniversalEmitter::Ptr emitter = thor::UniversalEmitter::create();
	emitter->setEmissionRate(30.f);
	emitter->setLifetime(sf::seconds(5.f));
	emitter->setPosition(MousePosition(window));

	// Create particle system
	thor::ParticleSystem system(texture);
	system.addEmitter(emitter);

	// Build color gradient (green -> teal -> blue)
	const thor::ColorGradient gradient = thor::createGradient
		(sf::Color(0, 150, 0))		(1)
		(sf::Color(0, 150, 100))	(1)
		(sf::Color(0, 0, 150));

	// Add particle affectors
	system.addAffector( thor::ColorAffector::create(gradient) );
	system.addAffector( thor::FadeInAffector::create(0.1f) );
	system.addAffector( thor::FadeOutAffector::create(0.1f) );
	system.addAffector( thor::TorqueAffector::create(100.f) );
	system.addAffector( thor::ForceAffector::create(sf::Vector2f(0.f, 100.f))  );

	// Attributes that influence emitter
	sf::Vector2f		position;
	thor::PolarVector2f velocity(200.f, -90.f);
	bool				paused = false;

	// Instruction text
	sf::Text instructions(
		"Left click: Enable/disable glow effect\n"
		"Right click: Pause\n"
		"Mouse wheel: Change direction\n");
	instructions.setCharacterSize(12);
	instructions.setColor(sf::Color::White);

	// Create clock to measure frame time
	sf::Clock frameClock;

	// Main loop
	for (;;)
	{
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				// [X]: Quit
				case sf::Event::Closed:
					return 0;
					
				// Escape: Quit
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
						return 0;

				// Left mouse button: Enable/disable glow
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left)
						system.setGlowing(!system.isGlowing());
					else if (event.mouseButton.button == sf::Mouse::Right)
						paused = !paused;
					break;

				// Mouse wheel: Change emission direction
				case sf::Event::MouseWheelMoved:
					velocity.phi += 12.f * event.mouseWheel.delta;
					break;
			}
		}
		
		// Get mouse position
		position = window.convertCoords(sf::Mouse::getPosition(window));

		// Update particle system and emitter
		const sf::Time frameTime = frameClock.restart();
		if (!paused)
			system.update(frameTime);

		// Set initial particle velocity, rotate vector randomly by maximal 10 degrees
		emitter->setVelocity( thor::Distributions::deflect(velocity, 10.f) );

		// Draw everything
		window.clear(sf::Color(30, 30, 30));
		window.draw(instructions);
		system.draw(window);
		window.display();
	}
}
