
#include <SFML/Graphics.hpp>
#include <Thor/Particles/ParticleSystem.hpp>
#include <Thor/Particles/EmissionInterface.hpp>
#include <Thor/Vectors/PolarVector2.hpp>
#include <Thor/Math/Random.hpp>
#include <Thor/Time/CallbackTimer.hpp>
#include <array>
#include <memory>

// Various constants as firework parameters
const sf::Time	explosionInterval	= sf::seconds(1.f);
const sf::Time	explosionDuration	= sf::seconds(0.2f);
const sf::Time	tailDuration		= sf::seconds(2.5f);
const sf::Int64	tailsPerExplosion	= 15;
const float		gravity				= 30.f;

// Array with possible colors for explosions
const std::array<sf::Color, 9> fireworkColors =
{
	sf::Color(100, 255, 135),	// light green
	sf::Color(175, 255, 135),	// lime green
	sf::Color(85, 190, 255),	// light blue
	sf::Color(255, 145, 255),	// pink
	sf::Color(100, 100, 255),	// indigo
	sf::Color(140, 250, 190),	// turquoise
	sf::Color(255, 135, 135),	// red
	sf::Color(240, 255, 135),	// light yellow
	sf::Color(245, 215, 80),	// light orange
};

// Custom emitter that groups particles in tails
class FireworkEmitter
{
	public:
		explicit FireworkEmitter(sf::Vector2f position)
		: mAccumulatedTime(sf::Time::Zero)
		, mPosition(position)
		, mColor(fireworkColors[thor::random(0u, fireworkColors.size() - 1u)])
		{
		}

		void operator() (thor::EmissionInterface& system, sf::Time dt)
		{
			const sf::Time tailInterval = explosionDuration / tailsPerExplosion;

			// Accumulate passed time. If enough time has passed (tailInterval), emit a new tail and decrease accumulator.
			mAccumulatedTime += dt;
			while (mAccumulatedTime - tailInterval > sf::Time::Zero)
			{
				emitTail(system);
				mAccumulatedTime -= tailInterval;
			}
		}

	private:
		void emitTail(thor::EmissionInterface& system)
		{
			// Create initial direction with random vector length and angle
			thor::PolarVector2f velocity(thor::random(30.f, 70.f), thor::random(0.f, 360.f));

			// Create particle at position of explosion, with emitter-specific color and at 80% initial scale
			thor::Particle particle(tailDuration);
			particle.position = mPosition;
			particle.color = mColor;
			particle.scale *= 0.8f;

			// A tail contains 25 particles with different speeds and scales.
			// The largest particles move fastest, leading to a comet-like tail effect.
			for (sf::Int64 i = 0; i < 25; ++i)
			{
				// Decrease scale continuously
				particle.scale *= 0.95f;

				// Decrease speed continuously
				velocity.r *= 0.96f;
				particle.velocity = velocity;

				// Add adapted particle to particle system
				system.emitParticle(particle);
			}
		}

	private:
		sf::Time		mAccumulatedTime;
		sf::Vector2f	mPosition;
		sf::Color		mColor;
};

// Custom affector that fades particles out and accelerates them according to scale
class FireworkAffector
{
	public:
		void operator() (thor::Particle& particle, sf::Time dt)
		{
			// Apply gravity, where particles with greater scale are affected stronger (quadratic influence)
			particle.velocity += dt.asSeconds() * sf::Vector2f(0.f, gravity) * particle.scale.x * particle.scale.y;

			// Let particles continuously fade out (particles with smaller scale have already lower alpha value at beginning)
			particle.color.a = static_cast<sf::Uint8>(256 * thor::getRemainingRatio(particle) * particle.scale.x);
		}
};

int main()
{
	// Create render window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Thor Fireworks", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	// Load texture
	sf::Texture texture;
	if (!texture.loadFromFile("Media/particle.png"))
		return EXIT_FAILURE;

	// Instantiate particle system and add custom affector
	thor::ParticleSystem system;
	system.setTexture(texture);
	system.addAffector(FireworkAffector());

	// Create timer that can be connected to callbacks. Initial time limit is 1 second, timer immediately starts
	thor::CallbackTimer explosionTimer;
	explosionTimer.restart(sf::seconds(1.f));

	// Connect timer to a lambda expression which restarts the timer every time it expires
	explosionTimer.connect( [] (thor::CallbackTimer& trigger)
	{
		trigger.restart(explosionInterval);
	});

	// Connect timer to a lambda expression that creates an explosion at expiration
	explosionTimer.connect0( [&system] ()
	{
		// Compute random position on screen
		sf::Vector2f position(thor::randomDev(400.f, 300.f), thor::randomDev(300.f, 200.f));

		// Add a temporary emitter to the particle system
		system.addEmitter(FireworkEmitter(position), explosionDuration);
	});

	// Main loop
	sf::Clock frameClock;
	while (window.isOpen())
	{
		// Event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed
			 || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				return 0;
		}

		// Update particle system and timer
		system.update(frameClock.restart());
		explosionTimer.update();

		// Draw everything, using additive blend mode for better visual effects
		window.clear();
		window.draw(system, sf::BlendAdd);
		window.display();
	}
}