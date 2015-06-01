
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

namespace Textures
{
	enum Type
	{
		ThorLogo,
		BrownRectangle,
	};
}

namespace Sounds
{
	enum Type
	{
		Click,
	};
}

int main()
{
	// Use enum to differentiate between textures/sounds, use string to differentiate between fonts
	thor::ResourceHolder<sf::Texture,     Textures::Type> textures;
	thor::ResourceHolder<sf::SoundBuffer, Sounds::Type>   sounds;
	thor::ResourceHolder<sf::Font,        std::string>    fonts;

	// Create sf::Image to load one texture from
	sf::Image image;
	image.create(872, 100, sf::Color(130, 70, 0));

	// Load resources, store them in resource pointers and react to loading errors
	try
	{
		textures.acquire(Textures::BrownRectangle, thor::Resources::fromImage<sf::Texture>(image));
		textures.acquire(Textures::ThorLogo, thor::Resources::fromFile<sf::Texture>("Media/thor.png"));
		sounds.acquire(Sounds::Click, thor::Resources::fromFile<sf::SoundBuffer>("Media/click.wav"));
		fonts.acquire("MainFont", thor::Resources::fromFile<sf::Font>("Media/sansation.ttf"));
	}
	catch (thor::ResourceLoadingException& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	// Create instances that use the resources
	sf::Sprite sprite1(textures[Textures::BrownRectangle]);
	sf::Sprite sprite2(textures[Textures::ThorLogo]);
	sf::Sound sound(sounds[Sounds::Click]);
	sf::Text instructions("Press return to play sound, escape to quit", fonts["MainFont"], 14u);

	// Move second sprite so that the sprites don't overlap
	sprite2.move(0.f, sprite1.getGlobalBounds().height);

	// Create render window
	sf::RenderWindow window(sf::VideoMode(872, 370), "Thor Resources");
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