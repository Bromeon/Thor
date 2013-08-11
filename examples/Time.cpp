
#include <Thor/Time.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

// Converts a float to a string with two decimal places
std::string toString(sf::Time value)
{
	std::ostringstream stream;
	stream.setf(std::ios_base::fixed);
	stream.precision(2);
	stream << value.asSeconds();
	return stream.str();
}

int main() 
{
	// Create SFML window
	sf::RenderWindow window(sf::VideoMode(300, 200), "Thor Time", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	// Create stopwatch and timer
	const sf::Time initialTime = sf::seconds(4.f);
	thor::StopWatch stopWatch;
	thor::CallbackTimer timer;
	timer.reset(initialTime);

	sf::Font font;
	if (!font.loadFromFile("Media/sansation.ttf"))
		return 1;

	// Create texts that display instructions and current time
	sf::Text instructions(
		"S      Start/pause stopwatch\n"
		"T      Start/pause timer\n"
		"R      Reset stopwatch and timer\n"
		"Esc  Quit",
		font, 14u);

	sf::Text stopWatchMeasurement;
	stopWatchMeasurement.setCharacterSize(20);
	stopWatchMeasurement.setFont(font);
	stopWatchMeasurement.setPosition(70.f, 120.f);
	stopWatchMeasurement.setColor(sf::Color(0, 190, 140));
	
	sf::Text timerMeasurement;
	timerMeasurement.setFont(font);
	timerMeasurement.setCharacterSize(20);
	timerMeasurement.setPosition(70.f, 150.f);
	timerMeasurement.setColor(sf::Color(0, 140, 190));
	
	// Connect timer with callback (colorize yellow) invoked at expiration time
	timer.connect(std::bind(&sf::Text::setColor, &timerMeasurement, sf::Color::Yellow));

	// Main loop
	for (;;)
	{
		// Event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			// [X]: Quit
			if (event.type == sf::Event::Closed)
			{
				return 0;
			}

			// Key pressed
			else if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
					// Esc: Quit
					case sf::Keyboard::Escape:
						return 0;

					// S: Start/pause stopwatch
					case sf::Keyboard::S:
						if (stopWatch.isRunning())
							stopWatch.stop();
						else
							stopWatch.start();
						break;

					// T: Start/pause timer
					case sf::Keyboard::T:
						if (timer.isRunning())
							timer.stop();
						else
							timer.start();
						break;

					// R: Reset both stopwatch and timer
					case sf::Keyboard::R:
						stopWatch.reset();
						timer.reset(initialTime);
						timerMeasurement.setColor(sf::Color(0, 140, 190));
						break;
				}
			}
		}

		// Adapt texts for stopwatch and timer according to elapsed/remaining time
		stopWatchMeasurement.setString("Stopwatch:  " + toString(stopWatch.getElapsedTime()));
		timerMeasurement.setString("Timer:  " + toString(timer.getRemainingTime()));

		// Update TriggeringTimer that allows the callback to be invoked
		timer.update();

		// Draw everything
		window.clear();
		window.draw(instructions);
		window.draw(stopWatchMeasurement);
		window.draw(timerMeasurement);
		window.display();
	}	
}