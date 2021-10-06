#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

using namespace std;

int main() {

	//Window config variables
	int const WINDOW_X = 800;
	int const WINDOW_Y = 600;
	string const WINDOW_TITLE = "C++ampo Minado";
	sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), WINDOW_TITLE);
	//------------------


	//Textures variables (on progress)
	int const SPRITESHEET_X = 4;
	int const SPRITESHEET_Y = 4;

	sf::Texture textures[SPRITESHEET_X][SPRITESHEET_Y];
	//-----------------


	// Sprites config (on progress)
	int const RECT_SIZE[2] = { 16, 16 };
	int const fieldX = 9;
	int const fieldY = 9;
	int fieldStatus[fieldX][fieldY];

	int bombAmount = 10;
	
	sf::Texture spacesTex[fieldX][fieldY];
	sf::Sprite spacesSpr[fieldX][fieldY];

	for (int i = 0; i < fieldX; ++i) {
		for (int j = 0; j < fieldY; ++j) {
			
			if (!spacesTex[i][j].loadFromFile("resources/FieldsSpritesheet.png", sf::IntRect(RECT_SIZE[0] * 0, RECT_SIZE[1] * 0, RECT_SIZE[0], RECT_SIZE[1]))) {

				cout << "Couldn't load Image.\n";
			}
			spacesTex[i][j].setSmooth(true);

			spacesSpr[i][j].setTexture(spacesTex[i][j]);
			
			/*
			* For the final position, independent of how many squares or its size or screen size to keep centered is:
			*
			* half the window size (will center everything on its pivot)
			* - half its size (will center everything on its sprite)
			* + its size multiplied by its position (will put everything side to side starting from the position 0
			* - an offset that is half the size of the total squares
			*
			* with this the central position will be always on center
			* counts for both X and Y axis.
			*/
			spacesSpr[i][j].setPosition(
				(WINDOW_X / 2) - (RECT_SIZE[0] / 2) + (RECT_SIZE[0] * i) - (RECT_SIZE[0] * (fieldX / 2)),
				(WINDOW_Y / 2) - (RECT_SIZE[1] / 2) + (RECT_SIZE[1] * j) - (RECT_SIZE[1] * (fieldY / 2))
			);
		}
	}


	#pragma	region testing textures
	/*
	sf::Texture defaultSpaceTex;
	if (!defaultSpaceTex.loadFromFile("resources/FieldsSpritesheet.png", sf::IntRect(RECT_SIZE[0] * 0, RECT_SIZE[1] * 0, RECT_SIZE[0], RECT_SIZE[1]))) {

		cout << "Couldn't load Image.\n";
	}
	defaultSpaceTex.setSmooth(true);

	sf::Sprite defaultSpaceSpr;
	defaultSpaceSpr.setScale(sf::Vector2f(2.0f, 2.0f));
	defaultSpaceSpr.setTexture(defaultSpaceTex);
	defaultSpaceSpr.setPosition ((WINDOW_X / 2) - (RECT_SIZE[0] / 2) - (RECT_SIZE[0] * 2), (WINDOW_Y / 2) - (RECT_SIZE[1] / 2));

	sf::Texture defaultSpaceTex2;
	if (!defaultSpaceTex2.loadFromFile("resources/FieldsSpritesheet.png", sf::IntRect(RECT_SIZE[0] * 0, RECT_SIZE[1] * 0, RECT_SIZE[0], RECT_SIZE[1]))) {

		cout << "Couldn't load Image.\n";
	}
	defaultSpaceTex2.setSmooth(false);

	sf::Sprite defaultSpaceSpr2;
	defaultSpaceSpr2.setScale(sf::Vector2f(2.0f, 2.0f));
	defaultSpaceSpr2.setTexture(defaultSpaceTex2);
	defaultSpaceSpr2.setPosition((WINDOW_X / 2) - (RECT_SIZE[0] / 2) - (RECT_SIZE[0] * 4), (WINDOW_Y / 2) - (RECT_SIZE[1] / 2));


	sf::Texture pressedSpaceTex;
	if (!pressedSpaceTex.loadFromFile("resources/FieldsSpritesheet.png", sf::IntRect(RECT_SIZE[0] * 3, RECT_SIZE[1] * 0, RECT_SIZE[0], RECT_SIZE[1]))) {

		cout << "Couldn't load Image.\n";
	}
	pressedSpaceTex.setSmooth(true);

	sf::Sprite pressedSpaceSpr;
	pressedSpaceSpr.setScale(sf::Vector2f(1.0f, 1.0f));
	pressedSpaceSpr.setTexture(pressedSpaceTex);
	pressedSpaceSpr.setPosition((WINDOW_X / 2) - (RECT_SIZE[0] / 2) + (RECT_SIZE[0] * 2), (WINDOW_Y / 2) - (RECT_SIZE[1] / 2));

	sf::Texture pressedSpaceTex2;
	if (!pressedSpaceTex2.loadFromFile("resources/FieldsSpritesheet.png", sf::IntRect(RECT_SIZE[0] * 3, RECT_SIZE[1] * 0, RECT_SIZE[0], RECT_SIZE[1]))) {

		cout << "Couldn't load Image.\n";
	}
	pressedSpaceTex2.setSmooth(true);

	sf::Sprite pressedSpaceSpr2;
	pressedSpaceSpr2.setScale(sf::Vector2f(2.0f, 2.0f));
	pressedSpaceSpr2.setTexture(pressedSpaceTex2);
	pressedSpaceSpr2.setPosition((WINDOW_X / 2) - (RECT_SIZE[0] / 2) + (RECT_SIZE[0] * 4), (WINDOW_Y / 2) - (RECT_SIZE[1] / 2));
	*/
	#pragma endregion

	#pragma region starting drawings
	/*
	float const shape1Radius = 100.0f;
	sf::CircleShape shape1(shape1Radius);
	shape1.setFillColor(sf::Color::Green);
	shape1.setPosition((WindowX / 2) - shape1Radius, (WindowY / 2) - shape1Radius);
	*/
	/*
	float const shape2Radius = 50.0f;
	sf::CircleShape shape2(shape2Radius);
	shape2.setFillColor(sf::Color::Red);
	shape2.setPosition((WindowX / 2) - shape2Radius, (WindowY / 2) - shape2Radius);
	*/
	#pragma endregion

	// main game loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

				window.close();
			}
		}

		window.clear(sf::Color::Blue);

		for (int i = 0; i < fieldX; ++i) {
			for (int j = 0; j < fieldY; ++j) {
				window.draw(spacesSpr[i][j]);
			}
		}

		#pragma region testing drawings (from testing textures)
		/*
		window.draw(defaultSpaceSpr);
		window.draw(defaultSpaceSpr2);
		window.draw(pressedSpaceSpr);
		window.draw(pressedSpaceSpr2);
		//window.draw(shape2);
		*/
		#pragma endregion

		window.display();
	}

	return 0;
}