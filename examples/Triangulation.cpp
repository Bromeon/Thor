
#include <Thor/Math/Triangulation.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include <Aurora/Tools/ForEach.hpp>
#include <SFML/Graphics.hpp>
#include <iterator>
#include <vector>

// Typedefs for simplicity. You can also use a different type than sf::Vector2f if you fulfill some requirements (see doc)
typedef std::vector<sf::Vector2f>					VertexContainer;
typedef std::vector<thor::Triangle<sf::Vector2f>>	TriangleContainer;

// Declarations of functions
bool	removeVertex(const VertexContainer& vertices, sf::Vector2f position);
bool	handleVertexClick(sf::Event::MouseButtonEvent mouseEvent, VertexContainer& vertices);
bool	handleEvents(sf::RenderWindow& window, VertexContainer& vertices, TriangleContainer& triangles);

int main()
{
	// Create render window
	sf::RenderWindow window(sf::VideoMode(640, 480), "Thor Triangulation", sf::Style::Close);
	window.setFramerateLimit(20);

	// Create containers in which we store the vertices and the computed triangles
	VertexContainer vertices;
	TriangleContainer triangles;
	
	sf::Font font;
	if (!font.loadFromFile("Media/sansation.ttf"))
		return 1;

	// Description with instructions
	sf::Text instructions(
		"Left click to add point\n"
		"Right click to remove point\n"
		"C key to clear everything",
		font, 14u);
		
	// Main loop
	for (;;)
	{
		// Event handling, possible quit
		if (!handleEvents(window, vertices, triangles))
			return 0;
	
		// Clear background
		window.clear();
		
		// Draw all triangles
		AURORA_FOREACH(const thor::Triangle<sf::Vector2f>& triangle, triangles)
		{
			sf::ConvexShape shape;
			shape.setPointCount(3);
			shape.setFillColor(sf::Color(0, 150, 255, 100));
			shape.setOutlineColor(sf::Color::Blue);
			shape.setOutlineThickness(1.f);

			for (unsigned int i = 0; i < 3; ++i)
				shape.setPoint(i, triangle[i]);
			
			window.draw(shape);
		}
		
		// Draw all points
		AURORA_FOREACH(sf::Vector2f vertex, vertices)
		{
			sf::CircleShape circle;
			circle.setPosition(vertex - sf::Vector2f(6.f, 6.f));
			circle.setRadius(6.f);
			circle.setFillColor(sf::Color(255, 0, 150));

			window.draw(circle);
		}
		
		// Draw description
		window.draw(instructions);
		
		// Update screen
		window.display();
	}
}

// Finds out whether a vertex is near a given position.
bool removeVertex(VertexContainer& vertices, sf::Vector2f position)
{
	// Find out which point was clicked on (tolerance radius is 6 pixels, as big as the circle's radius)
	for (auto itr = vertices.begin(); itr != vertices.end(); ++itr)
	{
		if (thor::squaredLength(position - *itr) <= 36.f)
		{
			vertices.erase(itr);
			return true;
		}
	}

	return false;
}

// Handles clicks on a vertex. Returns true if a new triangulation is required.
bool handleVertexClick(sf::Event::MouseButtonEvent mouseEvent, VertexContainer& vertices)
{
	sf::Vector2f clickPos(sf::Vector2i(mouseEvent.x, mouseEvent.y));
	
	// Add point when left-clicking
	if (mouseEvent.button == sf::Mouse::Left)
	{
		// Don't insert the same point twice
		AURORA_FOREACH(sf::Vector2f vertex, vertices)
		{
			if (vertex == clickPos)
				return false;
		}
		
		// If not contained yet, insert point
		vertices.push_back(clickPos);
		return true;
	}
	
	// Remove point when right-clicking
	else if (mouseEvent.button == sf::Mouse::Right)
	{
		if (removeVertex(vertices, clickPos))
			return true;
	}
	
	return false;
}

// Handles SFML events. Returns false if the application should be quit.
bool handleEvents(sf::RenderWindow& window, VertexContainer& vertices, TriangleContainer& triangles)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
			// Mouse buttons: Add or remove vertex
			case sf::Event::MouseButtonPressed:
			{
				// Compute new triangulation for points if necessary
				if (handleVertexClick(event.mouseButton, vertices))
				{
					triangles.clear();
					thor::triangulate(vertices.begin(), vertices.end(), std::back_inserter(triangles));
				}
			} break;

			// Keys (C -> Clear, Escape -> Quit)
			case sf::Event::KeyPressed:
			{
				switch (event.key.code)
				{
					case sf::Keyboard::Escape:
						return false;

					case sf::Keyboard::C:
						vertices.clear();
						triangles.clear();
				}
			} break;

			// [X] clicked
			case sf::Event::Closed:
				return false;
		}
	}

	return true;
}
