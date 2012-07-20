
#include <Thor/Shapes.hpp>
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
	thor::ConcaveShape pie     = thor::Shapes::pie(60.f, 135.f, sf::Color::Green);
	sf::ConvexShape    polygon = thor::Shapes::polygon(7, 60.f, sf::Color::Transparent, 3.f, sf::Color(175, 40, 250));
	sf::ConvexShape    star    = thor::Shapes::star(7, 40.f, 60.f, sf::Color(255, 225, 10), 5.f, sf::Color(250, 190, 20));

	// Move star and polygon shapes
	pie.move(100.f, 400.f);
	star.move(500.f, 100.f);
	polygon.move(500.f, 100.f);

	// Create an arrow pointing towards the mouse cursor
	thor::Arrow arrow(sf::Vector2f(window.getSize()) / 2.f, sf::Vector2f(), sf::Color(0, 150, 200));

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

				case sf::Event::MouseMoved:
					arrow.setDirection(event.mouseMove.x - window.getSize().x / 2.f, event.mouseMove.y - window.getSize().y / 2.f);
					break;
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
		window.draw(pie);
		window.draw(polygon);
		window.draw(star);
		window.draw(arrow);
		window.display();	
	}
}
