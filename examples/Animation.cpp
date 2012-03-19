
#include <Thor/Animation.hpp>
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(300, 200), "Thor Animation");
	window.setFramerateLimit(20);
	window.setKeyRepeatEnabled(false);

	// Instruction text
	sf::Text instructions(
		"D:  Play drive animation (loop)\n"
		"F:  Play fire animation\n"
		"S:  Stop current animation\n"
		"Esc:  Quit");
	instructions.setCharacterSize(12);
	instructions.setColor(sf::Color::White);

	// Load image that contains animation steps
	sf::Image image;
	if (!image.loadFromFile("Media/animation.png"))
		return 1;
	image.createMaskFromColor(sf::Color::White);

	// Create texture based on sf::Image
	sf::Texture texture;
	if (!texture.loadFromImage(image))
		return 1;

	// Create sprite and object that animates it
	sf::Sprite sprite(texture);
	sprite.setPosition(100.f, 100.f);
	thor::Animator animator;

	// Specify static subrect which is shown unless an other animation is active
	thor::FrameAnimation::Ptr defaultAnim = thor::FrameAnimation::create();
	defaultAnim->addFrame(1.f, sf::IntRect(0, 21, 44, 21));

	// Create first animation: Drive
	thor::FrameAnimation::Ptr drive = thor::FrameAnimation::create();
	for (unsigned int i = 0; i < 3; ++i)
		drive->addFrame(1.f, sf::IntRect(0, 21*i, 44, 21));

	// Create second animation: Fire
	thor::FrameAnimation::Ptr fire = thor::FrameAnimation::create();
	for (unsigned int i = 0; i < 4; ++i)
		fire->addFrame(1.f, sf::IntRect(44, 21*i, 49, 21));

	// Register animations with their corresponding durations
	animator.setDefaultAnimation(defaultAnim, sf::seconds(1.f));
	animator.addAnimation("drive", drive, sf::seconds(0.4f));
	animator.addAnimation("fire", fire, sf::seconds(0.3f));

	// Create clock to measure frame time
	sf::Clock frameClock;

	// Main loop
	for (;;)
	{
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
					case sf::Keyboard::D:		animator.playAnimation("drive", true);			break;
					case sf::Keyboard::F:		animator.playAnimation("fire");					break;
					case sf::Keyboard::S:		animator.stopAnimation();						break;
					case sf::Keyboard::Escape:	return 0;
				}
			}
			else if (event.type == sf::Event::Closed)
			{
				return 0;
			}
		}

		// Update animator and apply current animation state to the sprite
		animator.update(frameClock.restart());
		animator.animate(sprite);

		// Draw everything
		window.clear(sf::Color(50, 50, 50));
		window.draw(instructions);
		window.draw(sprite);
		window.display();
	}	
}