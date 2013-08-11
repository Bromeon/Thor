
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

int main()
{
	// Create resource cache for both sf::Texture and sf::SoundBuffer
	thor::MultiResourceCache cache;

	// Create sf::Image to load texture from
	sf::Image image;
	image.create(529, 100, sf::Color(130, 70, 0));

	// Create keys to load resources
	thor::ResourceKey<sf::Texture>		textureKey1 = thor::Resources::fromImage<sf::Texture>(image);
	thor::ResourceKey<sf::Texture>		textureKey2 = thor::Resources::fromFile<sf::Texture>("Media/image.jpg");
	thor::ResourceKey<sf::SoundBuffer>	soundKey	= thor::Resources::fromFile<sf::SoundBuffer>("Media/click.wav");
	thor::ResourceKey<sf::Font>			fontKey		= thor::Resources::fromFile<sf::Font>("Media/sansation.ttf");

	// Create resource pointers to access the resources
	std::shared_ptr<sf::Texture>		texture1, texture2;
	std::shared_ptr<sf::SoundBuffer>	soundBuffer;
	std::shared_ptr<sf::Font>			font;

	// Actually load resources, store them in resource pointers and react to loading errors
	try
	{
		texture1    = cache.acquire(textureKey1);
		texture2    = cache.acquire(textureKey2);
		soundBuffer = cache.acquire(soundKey);
		font        = cache.acquire(fontKey);
	}
	catch (thor::ResourceLoadingException& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	// Create instances that use the resources
	sf::Sprite sprite1(*texture1);
	sf::Sprite sprite2(*texture2);
	sf::Sound sound(*soundBuffer);
	sf::Text instructions("Press return to play sound, escape to quit", *font, 14u);

	// Move second sprite so that the sprites don't overlap
	sprite2.move( 0.f, static_cast<float>(texture1->getSize().y) );

	// Create render window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Thor Resources");
	window.setVerticalSyncEnabled(true);

	for (;;)
	{
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				return 0;
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
					case sf::Keyboard::Escape:
						return 0;

					case sf::Keyboard::Return:
						sound.play();
						break;
				}
			}
		}

		// Draw sprites of the loaded textures
		window.clear();
		window.draw(sprite1);
		window.draw(sprite2);
		window.draw(instructions);
		window.display();
	}
}