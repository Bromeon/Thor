
#include <Thor/Math/Triangulation.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include <Aurora/Tools/ForEach.hpp>
#include <SFML/Graphics.hpp>
#include <iterator>
#include <vector>


// Typedefs for simplicity. Instead of thor::Vertex, you may also use a class derived from
// thor::Vertex to store more information than just the position. Just replace the types.
typedef std::vector<sf::Vector2f>					VertexContainer;
typedef std::vector<thor::Triangle<sf::Vector2f>>	TriangleContainer;

// Declarations of functions
VertexContainer::const_iterator findVertex(const VertexContainer& vertices, sf::Vector2f position);
bool							handleVertexClick(sf::Event::MouseButtonEvent mouseEvent, VertexContainer& vertices);
bool							handleEvents(sf::RenderWindow& window, VertexContainer& vertices, TriangleContainer& triangles);

int main()
{
	// Create render window
	sf::RenderWindow window(sf::VideoMode(640, 480), "Thor Triangulation", sf::Style::Close);
	window.setFramerateLimit(20);

	// Create containers in which we store the vertices and the computed triangles
	VertexContainer		vertices;
	TriangleContainer	triangles;
	
	sf::Font font;
	if (!font.loadFromFile("Media/sansation.ttf"))
		return 1;

	// Description with instructions
	sf::Text instructions(
		"Left click to add point\n"
		"Right click to remove point\n"
		"C key to clear everything",
		font, 12);
		
	// Main loop
	for (;;)
	{
		// Event handling, possible quit
		if (!handleEvents(window, vertices, triangles))
			return 0;
	
		// Clear background
		window.clear();
		
		// Draw all triangles
		AURORA_CITR_FOREACH(itr, triangles)
		{
			sf::ConvexShape triangle;
			triangle.setPointCount(3);
			triangle.setFillColor(sf::Color(0, 150, 255, 100));
			triangle.setOutlineColor(sf::Color::Blue);
			triangle.setOutlineThickness(1.f);

			for (unsigned int i = 0; i < 3; ++i)
				triangle.setPoint(i, (*itr)[i]);
			
			window.draw(triangle);
		}
		
		// Draw all points
		AURORA_CITR_FOREACH(itr, vertices)
		{
			sf::CircleShape circle;
			circle.setPosition(*itr - sf::Vector2f(6.f, 6.f));
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
VertexContainer::iterator findVertex(VertexContainer& vertices, sf::Vector2f position)
{
	// Find out which point was clicked on (tolerance radius is 6 pixels, as big as the circle's radius)
	AURORA_ITR_FOREACH(itr, vertices)
	{
		if (thor::squaredLength(position - *itr) <= 36.f)
			return itr;
	}
	
	return vertices.end();
}

// Handles clicks on a vertex. Returns true if a new triangulation is required.
bool handleVertexClick(sf::Event::MouseButtonEvent mouseEvent, VertexContainer& vertices)
{
	sf::Vector2f clickPos(static_cast<float>(mouseEvent.x), static_cast<float>(mouseEvent.y));
	
	// Add point when left-clicking
	if (mouseEvent.button == sf::Mouse::Left)
	{
		// Don't insert the same point twice
		AURORA_CITR_FOREACH(itr, vertices)
		{
			if (*itr == clickPos)
				return false;
		}
		
		// If not contained yet, insert point
		vertices.push_back(clickPos);
		return true;
	}
	
	// Remove point when right-clicking
	else if (mouseEvent.button == sf::Mouse::Right)
	{
		VertexContainer::iterator vertexItr = findVertex(vertices, clickPos);
		if (vertexItr != vertices.end())
		{
			vertices.erase(vertexItr);
			return true;
		}
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
