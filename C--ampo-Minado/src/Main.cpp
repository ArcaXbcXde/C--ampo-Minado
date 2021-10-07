#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

#pragma region Variables
//Window config variables
int const WINDOW_X = 800; // window width
int const WINDOW_Y = 600; // window height
string const WINDOW_TITLE = "C++ampo Minado"; // window name
sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), WINDOW_TITLE); // window config
//------------------

//Game config
int const fieldX = 9; // number of columns in-game
int const fieldY = 9; // number of rows in-game

int bombAmount = 10; // number of bombs in-game

sf::Color backgroundColoration; // for the randomization of the background color (not the background sprite)
//-----------------

//Sprites and textures config
int const SPRITESHEET_X = 4; // amount of columns of sprites
int const SPRITESHEET_Y = 4; // amount of rows of sprites

int const RECT_SIZE[2] = { 16, 16 }; // Size of each texture

int fieldStatus[fieldX][fieldY];

string const SPRITESHEET_FILE = "resources/FieldsSpritesheet.png"; // used spritesheet place

sf::Texture textures[SPRITESHEET_X][SPRITESHEET_Y]; // matrix with all textures from the spritesheet

sf::Texture spacesTex[fieldX][fieldY];
sf::Sprite spacesSpr[fieldX][fieldY];

sf::Texture bgTex;
sf::Sprite bgSpr;
//----------------
#pragma endregion

class Settings {
	public:

		void getSpacesTextures() {
			for (int i = 0; i < SPRITESHEET_X; ++i) {
				for (int j = 0; j < SPRITESHEET_Y; ++j) {
					if (!textures[i][j].loadFromFile(SPRITESHEET_FILE, sf::IntRect(RECT_SIZE[0] * i, RECT_SIZE[1] * j, RECT_SIZE[0], RECT_SIZE[1]))) {

						cout << "Couldn't load Image.\n";
					}
				}
			}
		}

		void setSpacesSprites() {

			for (int i = 0; i < fieldX; ++i) {
				for (int j = 0; j < fieldY; ++j) {

					spacesTex[i][j] = textures[0][0]; // set all as default tile

					spacesTex[i][j].setSmooth(true);

					spacesSpr[i][j].setTexture(spacesTex[i][j]);

					/*
					* For the final position for each square, independent of how many squares or its size or screen size, to keep centered is:
					*
					* half the window size (will center everything on its pivot)
					* (-) half its size (will center everything on its sprite)
					* (+) its size multiplied by its position (will put everything side to side starting from the position 0
					* (-) an offset that is half the size of the total squares
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
		}
};

class Tests {
	public:

		// gets textures and sets the sprites
		void setTestSprites() {

			for (int i = 0; i < SPRITESHEET_X; ++i) {
				for (int j = 0; j < SPRITESHEET_Y; ++j) {
					spacesSpr[i][j].setTexture(textures[i][j]);
				}
			}
		}
		
		// Extremely heavy because "setTestSprites" is here being called every frame, but just for testing
		void drawAllTextures() {

			setTestSprites();

			for (int i = 0; i < SPRITESHEET_X; ++i) {
				for (int j = 0; j < SPRITESHEET_Y; ++j) {
					window.draw(spacesSpr[i][j]);

					spacesSpr[i][j].setPosition(
						(WINDOW_X / 2) - (RECT_SIZE[0] / 2) + (RECT_SIZE[0] * i) - (RECT_SIZE[0] * (SPRITESHEET_X / 2)),
						(WINDOW_Y / 2) - (RECT_SIZE[1] / 2) + (RECT_SIZE[1] * j) - (RECT_SIZE[1] * (SPRITESHEET_Y / 2))
					);
				}
			}
		}

		void coutBombsPlacement() {
			cout << "|__|__|__|__|__|__|__|__|__|" << endl;
			for (int i = 0; i < fieldX; ++i) {
				for (int j = 0; j < fieldY; ++j) {
					if (fieldStatus[i][j] < 10) {
					cout << "|0" << fieldStatus[i][j];
					} else {
						cout << "|xx";
					}
				}
				cout << "|\n|__|__|__|__|__|__|__|__|__|" << endl;
			}
			/*for (int i = 0; i < fieldX; ++i) {
				for (int j = 0; j < fieldY; ++j) {
					cout << "value at (" << i << ", " << j << ") :" << fieldStatus[i][j] << endl;
				}
			}*/
		}

		void drawTesting() {

			// draw the basic default field
			for (int i = 0; i < fieldX; ++i) {
				for (int j = 0; j < fieldY; ++j) {
					window.draw(spacesSpr[i][j]);
				}
			}
			/*
			window.draw(defaultSpaceSpr);
			window.draw(defaultSpaceSpr2);
			window.draw(pressedSpaceSpr);
			window.draw(pressedSpaceSpr2);
			//window.draw(shape2);
			*/
		}
};

class Screen {
	private:

		sf::Color randomizeBackgroundColor() {

			srand(time(NULL));
			int const r = rand() % 256;
			srand(time(NULL) * 2);
			int const g = rand() % 256;
			srand(time(NULL) * 3);
			int const b = rand() % 256;

			return sf::Color(r, g, b);
		}

	public:

		void setBackground() {

			backgroundColoration = { randomizeBackgroundColor() };

			bgTex = textures[2][1];
			bgTex.setSmooth(true);
			bgTex.setRepeated(true);

			bgSpr.setTextureRect(sf::IntRect(0, 0, WINDOW_X, WINDOW_Y));
			bgSpr.setTexture(bgTex);
		}

		void drawScreen() {

			window.clear(backgroundColoration);
			window.draw(bgSpr);

			Tests test;
			//test.drawAllTextures();
			test.drawTesting();

			window.display();
		}

};

class Game {
	private:

		// sets all fields with the value 0 (nothing nearby)
		void zeroFieldStatus() {
			for (int i = 0; i < fieldX; ++i) {
				for (int j = 0; j < fieldY; ++j) {
					fieldStatus[i][j] = 0;
				}
			}
		}

	public:

		void setBombs() {

			
			zeroFieldStatus();

			int unplantedBombs = bombAmount;
			int randomX;
			int randomY;

			srand(time(NULL));

			while (unplantedBombs > 0) {

				randomX = rand() % fieldX;
				
				randomY = rand() % fieldY;
				
				if (fieldStatus[randomX][randomY] < 10) {
					
					// easily optimizable
					if (randomX > 0 && randomY > 0) {
						fieldStatus[randomX - 1][randomY - 1] += 1;
					}
					if (randomY > 0) {
						fieldStatus[randomX    ][randomY - 1] += 1;
					}
					if (randomX < fieldX && randomY > 0) {
						fieldStatus[randomX + 1][randomY - 1] += 1;
					}
					if (randomX > 0) {
						fieldStatus[randomX - 1][randomY    ] += 1;
					}
					
					fieldStatus[randomX    ][randomY    ] += 10;
					
					if (randomX < fieldX) {
						fieldStatus[randomX + 1][randomY    ] += 1;
					}
					if (randomX > 0 && randomY < fieldY) {
						fieldStatus[randomX - 1][randomY + 1] += 1;
					}
					if (randomY < fieldY) {
						fieldStatus[randomX    ][randomY + 1] += 1;
					}
					if (randomX < fieldX && randomY < fieldY) {
						fieldStatus[randomX + 1][randomY + 1] += 1;
					}

					unplantedBombs--;
				}
			}
		}
};

int main() {


	Settings settings;
	settings.getSpacesTextures(); // initialize the "textures" variable
	settings.setSpacesSprites(); // set what is each sprite from the field

	Tests test;

	Screen screen;
	screen.setBackground(); // sets how the background will appear

	Game game;
	game.setBombs();

	test.coutBombsPlacement();
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
	while (window.isOpen())	{

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

				window.close();
			}
		}


		// screen manipulation
		screen.drawScreen();
		//-------------------
	}

	return 0;
}
