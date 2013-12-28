
#include <Thor/Shapes/ConcaveShape.hpp>
#include <Thor/Shapes/Shapes.hpp>
#include <SFML/Graphics.hpp>

int main()
{
	// Create render window
	sf::RenderWindow window(sf::VideoMode(600, 500), "Thor Shapes", sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	
	// Create a concave shape by directly inserting the polygon points
	thor::ConcaveShape concaveShape;
	concaveShape.setPointCount(5);
	concaveShape.setPoint(0, sf::Vector2f(50,  50));
	concaveShape.setPoint(1, sf::Vector2f(100, 100));
	concaveShape.setPoint(2, sf::Vector2f(150, 50));
	concaveShape.setPoint(3, sf::Vector2f(150, 200));
	concaveShape.setPoint(4, sf::Vector2f(50,  150));
	concaveShape.setOutlineThickness(2.f);
	concaveShape.setFillColor(sf::Color(150, 100, 100));
	concaveShape.setOutlineColor(sf::Color(200, 100, 100));
	
	// Create thor::ConcaveShape shape from sf::Shape
	thor::ConcaveShape circle = sf::CircleShape(60.f);
	circle.setFillColor(sf::Color(0, 200, 0));	
	circle.setPosition(40.f, 340.f);

	// Create a few predefined shapes
	sf::ConvexShape polygon = thor::Shapes::polygon(7, 60.f, sf::Color::Transparent, 3.f, sf::Color(175, 40, 250));
	sf::ConvexShape star = thor::Shapes::star(7, 40.f, 60.f, sf::Color(255, 225, 10), 5.f, sf::Color(250, 190, 20));
	sf::ConvexShape roundedRect = thor::Shapes::roundedRect(sf::Vector2f(200.f, 100.f), 30.f, sf::Color(200, 190, 120), 3.f, sf::Color(150, 140, 80));

	// Move star and polygon shapes
	star.move(480.f, 120.f);
	polygon.move(480.f, 120.f);
	roundedRect.move(380.f, 350.f);

	// Create clock to measure frame time
	sf::Clock frameClock;

	// Main loop
	for (;;)
	{
		// Event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
				case sf::Event::KeyPressed:
					return 0;
			}
		}

		// Rotate polygon and star
		const sf::Time elapsed = frameClock.restart();
		polygon.rotate(20.f * elapsed.asSeconds());
		star.rotate(45.f * elapsed.asSeconds());
	
		// Draw everything
		window.clear();
		window.draw(concaveShape);
		window.draw(circle);
		window.draw(polygon);
		window.draw(star);
		window.draw(roundedRect);
		window.display();	
	}
}
