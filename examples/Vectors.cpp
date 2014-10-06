#include <SFML/Graphics.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include <Thor/Shapes/Arrow.hpp>
#include <array>

// Computes a vector from the center of the window to the mouse position
sf::Vector2f vectorToMouse(const sf::RenderWindow& window, const sf::Event& event)
{
	sf::Vector2f windowCenter(window.getSize() / 2u);
	sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

	return mousePosition - windowCenter;
}

// Different types of vectors that are shown
enum VectorType
{
	Original,
	RotatedScaled,
	ProjectionAxis,
	Projected,
	FixedLength,
	Perpendicular,
	Count,
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Thor Vectors", sf::Style::Close);
	window.setFramerateLimit(20);

	// Persistent vectors and scalars
	sf::Vector2f original(200.f, 0.f);
	sf::Vector2f axis(80.f, 80.f);
	float angle = 25.f;
	float scale = 0.7f;

	// Colors for different vectors
	std::array<sf::Color, Count> colors;
	colors[Original] = sf::Color(255, 255, 255);
	colors[FixedLength] = sf::Color(255, 0, 100);
	colors[Perpendicular] = sf::Color(0, 150, 0);
	colors[RotatedScaled] = sf::Color(255, 220, 0);
	colors[ProjectionAxis] = sf::Color(0, 60, 220);
	colors[Projected] = sf::Color(0, 150, 250);

	sf::Font font;
	if (!font.loadFromFile("Media/sansation.ttf"))
		return 1;

	// Description texts
	std::array<sf::Text, Count> texts;
	texts[Original].setString("Original vector (left mouse button)");
	texts[FixedLength].setString("100 * unit vector");
	texts[Perpendicular].setString("Perpendicular vector");
	texts[RotatedScaled].setString("Rotated/scaled vector (middle mouse button)");
	texts[ProjectionAxis].setString("Projection axis (right mouse button)");
	texts[Projected].setString("Vector projected onto axis");

	// Arrow shapes to visualize vectors
	std::array<thor::Arrow, Count> arrows;

	// Initialize texts and arrows
	for (unsigned int i = 0; i < Count; ++i)
	{
		texts[i].setColor(colors[i]);
		texts[i].setCharacterSize(14u);
		texts[i].setFont(font);
		texts[i].setPosition(0.f, 16.f * i);

		arrows[i].setColor(colors[i]);
		arrows[i].setPosition(sf::Vector2f(window.getSize() / 2u));
		arrows[i].setThickness(3.f);
	}

	arrows[ProjectionAxis].setThickness(5.f);
	arrows[Original].setThickness(5.f);

	// Main loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Quit
			if (event.type == sf::Event::Closed
			 || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				return 0;
			}

			// Reset vector when mouse button clicked
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					original = vectorToMouse(window, event);
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					axis = vectorToMouse(window, event);
				}
				else if (event.mouseButton.button == sf::Mouse::Middle)
				{
					// Get angle and scale relative to original vector
					sf::Vector2f vector = vectorToMouse(window, event);
					angle = thor::signedAngle(original, vector);
					scale = thor::length(vector) / thor::length(original);
				}
			}
		}

		// Rotate original vector
		thor::rotate(original, 1.f);

		// Assign vectors to arrows
		arrows[Original].setDirection(original);
		arrows[FixedLength].setDirection(100.f * thor::unitVector(original));
		arrows[ProjectionAxis].setDirection(axis);
		arrows[Perpendicular].setDirection(thor::perpendicularVector(original));
		arrows[RotatedScaled].setDirection(scale * thor::rotatedVector(original, angle));
		arrows[Projected].setDirection(thor::projectedVector(original, axis));

		// Draw everything
		window.clear();
		for (unsigned int i = 0; i < Count; ++i)
		{
			window.draw(arrows[i]);
			window.draw(texts[i]);
		}
		window.display();
	}
}