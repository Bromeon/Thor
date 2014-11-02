
#include <Thor/Particles.hpp>
#include <Thor/Animations.hpp>
#include <Thor/Vectors/PolarVector2.hpp>
#include <Thor/Math/Distributions.hpp>
#include <SFML/Graphics.hpp>

int main()
{
	// Create window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Thor Particles");
	
	// Load texture
	sf::Texture texture;
	if (!texture.loadFromFile("Media/particle.png"))
		return EXIT_FAILURE;
	
	// Create emitter
	thor::UniversalEmitter emitter;
	emitter.setEmissionRate(30.f);
	emitter.setParticleLifetime(sf::seconds(5.f));

	// Create particle system, add reference to emitter
	thor::ParticleSystem system;
	system.setTexture(texture);
	system.addEmitter(thor::refEmitter(emitter));

	// Build color gradient (green -> teal -> blue)
	thor::ColorGradient gradient;
	gradient[0.f] = sf::Color(0, 150, 0);
	gradient[0.5f] = sf::Color(0, 150, 100);
	gradient[1.f] = sf::Color(0, 0, 150);

	// Create color and fade in/out animations
	thor::ColorAnimation colorizer(gradient);
	thor::FadeAnimation fader(0.1f, 0.1f);

	// Add particle affectors
	system.addAffector( thor::AnimationAffector(colorizer) );
	system.addAffector( thor::AnimationAffector(fader) );
	system.addAffector( thor::TorqueAffector(100.f) );
	system.addAffector( thor::ForceAffector(sf::Vector2f(0.f, 100.f)) );

	// Attributes that influence emitter
	thor::PolarVector2f velocity(200.f, -90.f);
	bool paused = false;

	sf::Font font;
	if (!font.loadFromFile("Media/sansation.ttf"))
		return 1;

	// Instruction text
	sf::Text instructions(
		"Left click: Pause\n"
		"Mouse wheel: Change direction\n",
		font, 14u);

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
						paused = !paused;
					break;

				// Mouse wheel: Change emission direction
				case sf::Event::MouseWheelMoved:
					velocity.phi += 12.f * event.mouseWheel.delta;
					break;
			}
		}
		
		// Update particle system and emitter
		const sf::Time frameTime = frameClock.restart();
		if (!paused)
			system.update(frameTime);

		// Set initial particle position and velocity, rotate vector randomly by maximal 10 degrees
		emitter.setParticlePosition( window.mapPixelToCoords(sf::Mouse::getPosition(window)) );
		emitter.setParticleVelocity( thor::Distributions::deflect(velocity, 10.f) );

		// Draw everything
		window.clear(sf::Color(30, 30, 30));
		window.draw(instructions);
		window.draw(system);
		window.display();
	}
}
