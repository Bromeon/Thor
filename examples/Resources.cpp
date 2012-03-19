
#include <Thor/Resources.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

int main()
{
	// Create texture and sound manager
	thor::ResourceManager<sf::Texture> textureMgr;
	thor::ResourceManager<sf::SoundBuffer> soundMgr;

	// Import names for simplicity
	using thor::Resources::TextureKey;
	using thor::Resources::SoundBufferKey;

	// Create sf::Image to load texture from
	sf::Image image;
	image.create(529, 100, sf::Color(130, 70, 0));

	// Create keys to load resources
	TextureKey textureKey1  = TextureKey::fromImage(image);
	TextureKey textureKey2  = TextureKey::fromFile("Media/image.jpg");
	SoundBufferKey soundKey = SoundBufferKey::fromFile("Media/click.wav");

	// Create resource pointers to access the resources
	thor::ResourcePtr<sf::Texture>     texture1, texture2;
	thor::ResourcePtr<sf::SoundBuffer> soundBuffer;

	// Actually load resources, store them in resource pointers and react to loading errors
	try
	{
		texture1    = textureMgr.acquire(textureKey1);
		texture2    = textureMgr.acquire(textureKey2);
		soundBuffer = soundMgr.acquire(soundKey);
	}
	catch (thor::ResourceLoadingException& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}

	// Create instances that use the resources
	sf::Sprite sprite1(*texture1);
	sf::Sprite sprite2(*texture2);
	sf::Sound  sound(*soundBuffer);

	// Move second sprite so that the sprites don't overlap
	sprite2.move( 0.f, static_cast<float>(texture1->getHeight()) );

	// Create instruction text
	sf::Text instructions("Press return to play sound, escape to quit");
	instructions.setCharacterSize(12);
	instructions.setColor(sf::Color::White);

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