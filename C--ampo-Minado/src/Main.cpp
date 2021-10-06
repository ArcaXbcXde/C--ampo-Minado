#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

using namespace std;

int main() {


	int const WindowX = 800;
	int const WindowY = 600;
	sf::RenderWindow window(sf::VideoMode(WindowX, WindowY), "SFML works!");

	sf::Texture texture;
	if (!texture.loadFromFile("resources/CoveredSpace.png")) {

		cout << "Couldn't load Image.\n";
	}

	float const shape1Radius = 100.0f;
	sf::CircleShape shape1(shape1Radius);
	shape1.setFillColor(sf::Color::Green);
	shape1.setPosition((WindowX / 2) - shape1Radius, (WindowY / 2) - shape1Radius);
	
	float const shape2Radius = 50.0f;
	sf::CircleShape shape2(shape2Radius);
	shape2.setFillColor(sf::Color::Red);
	shape2.setPosition((WindowX / 2) - shape2Radius, (WindowY / 2) - shape2Radius);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Blue);

		window.draw(shape1);

		window.draw(shape2);

		window.display();
	}

	return 0;
}