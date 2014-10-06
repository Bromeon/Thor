
#include <Thor/Animations.hpp>
#include <SFML/Graphics.hpp>

// Adds a range of frames, assuming they are aligned as rectangles in the texture.
// animation:      FrameAnimation to modify
// x:              Column index of the texture rectangle
// [yFirst,yLast]: Bounds for row indices (if yLast < yFirst, add frames in reverse order)
// duration:       Relative duration of current frame (1 by default)
void addFrames(thor::FrameAnimation& animation, int x, int yFirst, int yLast, float duration = 1.f)
{
	const int step = (yFirst < yLast) ? +1 : -1;
	yLast += step; // so yLast is excluded in the range

	for (int y = yFirst; y != yLast; y += step)
		animation.addFrame(duration, sf::IntRect(36*x, 39*y, 36, 39));
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(300, 200), "Thor Animation");
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);

	sf::Font font;
	if (!font.loadFromFile("Media/sansation.ttf"))
		return 1;

	// Instruction text
	sf::Text instructions(
		"W:     Play walk animation (loop)\n"
		"A:      Play attack animation\n"
		"S:      Stop current animation\n"
		"Esc:  Quit",
		font, 14u);

	sf::Text animationText("", font, 14u);
	animationText.setPosition(100.f, 150.f);

	// Load image that contains animation steps
	sf::Image image;
	if (!image.loadFromFile("Media/animation.png"))
		return 1;
	image.createMaskFromColor(sf::Color::White);

	// Create texture based on sf::Image
	sf::Texture texture;
	if (!texture.loadFromImage(image))
		return 1;

	// Create sprite which is animated
	sf::Sprite sprite(texture);
	sprite.setPosition(100.f, 100.f);

	// Define walk animation
	thor::FrameAnimation walk;
	addFrames(walk, 0, 0, 7);			// Frames 0..7	Right leg moves forward
	addFrames(walk, 0, 6, 0);			// Frames 6..0	Right leg moves backward

	// Define attack animation
	thor::FrameAnimation attack;
	addFrames(attack, 1, 0, 3);			// Frames 0..3	Lift gun
	addFrames(attack, 1, 4, 4, 5.f);	// Frame  4		Aim (5 times normal frame duration)
	for (int i = 0; i < 3; ++i)
		addFrames(attack, 1, 5, 7);		// Frame  5..7	Fire (repeat 3 times)
	addFrames(attack, 1, 4, 4, 5.f);	// Frame  4		Wait
	addFrames(attack, 1, 3, 0);			// Frame  3..1	Lower gun

	// Define static frame for stand animation
	thor::FrameAnimation stand;
	addFrames(stand, 0, 0, 0);

	// Register animations with their corresponding durations
	thor::Animator<sf::Sprite, std::string> animator;
	animator.addAnimation("walk", walk, sf::seconds(1.f));
	animator.addAnimation("stand", stand, sf::seconds(1.f));
	animator.addAnimation("attack", attack, sf::seconds(1.f));

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
					case sf::Keyboard::W:		animator.playAnimation("walk", true);			break;
					case sf::Keyboard::A:		animator.playAnimation("attack");				break;
					case sf::Keyboard::S:		animator.stopAnimation();						break;
					case sf::Keyboard::Escape:	return 0;
				}
			}
			else if (event.type == sf::Event::Closed)
			{
				return 0;
			}
		}

		// If no other animation is playing, play stand animation
		if (!animator.isPlayingAnimation())
			animator.playAnimation("stand");

		// Output playing animation (general case; at the moment an animation is always playing)
		if (animator.isPlayingAnimation())
			animationText.setString("Animation: " + animator.getPlayingAnimation());
		else
			animationText.setString("");

		// Update animator and apply current animation state to the sprite
		animator.update(frameClock.restart());
		animator.animate(sprite);

		// Draw everything
		window.clear(sf::Color(50, 50, 50));
		window.draw(instructions);
		window.draw(animationText);
		window.draw(sprite);
		window.display();
	}	
}