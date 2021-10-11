
#pragma region includes and imports

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

#pragma endregion

#pragma region Variables

//Window config variables
int const WINDOW_X = 800; // window width
int const WINDOW_Y = 600; // window height
string const WINDOW_TITLE = "C++ampo Minado"; // window name
sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), WINDOW_TITLE); // window config

sf::Color backgroundColoration; // for the randomization of the background color (not the background sprite)
//------------------

//Game config
bool gameSet = false; // A flag to define if the game need to be set

int const fieldX = 10; // number of in-game columns, works for any positive amount, but fit only up to 49 on-screen
int const fieldY = 9; // number of in-game rows, works for any positive amount, but fit only up to 37 on-screen

int bombAmount = 12; // number of bombs in-game, works for any amount but will be capped 1 lower than the spaces amount (fieldX * fieldY)

int safeSpacesAmount; // number of safe spaces left

int pliersAmout = 3; // number of pliers to possibly disarm bombs
//-----------------

//Sprites and textures config
int const SPRITESHEET_X = 4; // amount of columns of sprites
int const SPRITESHEET_Y = 4; // amount of rows of sprites

int const RECT_SIZE[2] = { 16, 16 }; // Size of each texture

string const SPRITESHEET_FILE = "resources/FieldsSpritesheet.png"; // used spritesheet place

bool spacesRevealed[fieldX][fieldY]; // Flags for the entire field if it is revealed or not.

bool flagged[fieldX][fieldY]; // Flag to flag if a space is with a flag

int fieldStatus[fieldX][fieldY]; // Marks for each bomb and number

sf::Texture textures[SPRITESHEET_X][SPRITESHEET_Y]; // matrix with all textures from the spritesheet

sf::Sprite spacesSpr[fieldX][fieldY];
//----------------

#pragma endregion

class GameSettings {

private:

	sf::Texture spacesTex;

public:

	// picks each texture from the Spritesheet and put ordered in the same way inside the textures array
	void getSpacesTextures() {
		for (int i = 0; i < SPRITESHEET_X; ++i) {
			for (int j = 0; j < SPRITESHEET_Y; ++j) {
				if (!textures[i][j].loadFromFile(
					SPRITESHEET_FILE,
					sf::IntRect(RECT_SIZE[0] * i, RECT_SIZE[1] * j, RECT_SIZE[0], RECT_SIZE[1]))
				){

					cout << "Couldn't load Image.\nPosition: (" << i << ", " << j << ")";
				}
			}
		}
	}

	// initially set all as default tile
	void setSpacesSprites() {

		spacesTex = textures[0][0]; // (0, 0) is the default tile position in the spritesheet
		spacesTex.setSmooth(true);

		for (int i = 0; i < fieldX; ++i) {
			for (int j = 0; j < fieldY; ++j) {

				spacesSpr[i][j].setTexture(spacesTex);

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

	// sets all fields with the value 0 (nothing nearby) and marks the entire field as untouched
	void zeroFieldStatus() {

		for (int i = 0; i < fieldX; ++i) {
			for (int j = 0; j < fieldY; ++j) {
				fieldStatus[i][j] = 0;
				spacesRevealed[i][j] = 0;
			}
		}
	}

};

class Screen {

private:

	sf::Texture bgTex;
	sf::Sprite bgSpr;

	// returns a random color
	sf::Color randomColor() {

		srand(time(NULL));
		int const r = rand() % 256;
		srand(time(NULL) * 2);
		int const g = rand() % 256;
		srand(time(NULL) * 3);
		int const b = rand() % 256;

		return sf::Color(r, g, b);
	}

	// draw the initial default field
	void drawField() {

		for (int i = 0; i < fieldX; ++i) {
			for (int j = 0; j < fieldY; ++j) {
				window.draw(spacesSpr[i][j]);
			}
		}
	}

public:

	// handles the initial background details
	void setBackground() {

		backgroundColoration = { randomColor() };

		bgTex = textures[2][1];
		bgTex.setSmooth(true);
		bgTex.setRepeated(true);

		bgSpr.setTextureRect(sf::IntRect(0, 0, WINDOW_X, WINDOW_Y));
		bgSpr.setTexture(bgTex);
	}

	// draws everything on screen
	void drawScreen() {

		window.clear(backgroundColoration);
		window.draw(bgSpr);

		drawField();

		window.display();
	}
};

class Game {

private:

	bool breakLoop = false; // just to be sure to break both loops in mouse handlers
	bool holding = false; // to hold just a single space

	int heldSpace[2] = { -1, -1 };

	string spaces;

	sf::FloatRect spacesHitbox[fieldX][fieldY]; // hitbox for each space in the field

	// reveal what is the space value to the player
	void changeSprite(int i, int j) {

		if (fieldStatus[i][j] == 1) {

			spacesSpr[i][j].setTexture(textures[0][2]); // number 1
		}
		else if (fieldStatus[i][j] == 2) {

			spacesSpr[i][j].setTexture(textures[1][2]); // number 2
		}
		else if (fieldStatus[i][j] == 3) {

			spacesSpr[i][j].setTexture(textures[2][2]); // number 3
		}
		else if (fieldStatus[i][j] == 4) {

			spacesSpr[i][j].setTexture(textures[3][2]); // number 4
		}
		else if (fieldStatus[i][j] == 5) {

			spacesSpr[i][j].setTexture(textures[0][3]); // number 5
		}
		else if (fieldStatus[i][j] == 6) {

			spacesSpr[i][j].setTexture(textures[1][3]); // number 6
		}
		else if (fieldStatus[i][j] == 7) {

			spacesSpr[i][j].setTexture(textures[2][3]); // number 7
		}
		else if (fieldStatus[i][j] == 8) {

			spacesSpr[i][j].setTexture(textures[3][3]); // number 8
		}
	}

	// reveal a single space
	void revealSpace(int i, int j) {

		spacesRevealed[i][j] = true;

		if (fieldStatus[i][j] >= 10) { // game over (clicked on a bomb)

			spacesSpr[i][j].setTexture(textures[1][1]); // exploded bomb
			gameLost();
		} else { // not a bomb

			safeSpacesAmount--;
			if (fieldStatus[i][j] != 0) {

				changeSprite(i, j);
			} else {

				spacesSpr[i][j].setTexture(textures[2][0]); // empty space

				// reveal all nearby spaces (could do with a for)

				if (i > 0 && j > 0 && spacesRevealed[i - 1][j - 1] == false && flagged[i - 1][j - 1] == false) {
					revealSpace(i - 1, j - 1);
				}
				if (i > 0 && spacesRevealed[i - 1][j] == false && flagged[i - 1][j] == false) {
					revealSpace(i - 1, j);
				}
				if (i > 0 && j < fieldY - 1 && spacesRevealed[i - 1][j + 1] == false && flagged[i - 1][j + 1] == false) {
					revealSpace(i - 1, j + 1);
				}
				if (j > 0 && spacesRevealed[i][j - 1] == false && flagged[i][j - 1] == false) {
					revealSpace(i, j - 1);
				}
				if (j < fieldY - 1 && spacesRevealed[i][j + 1] == false && flagged[i][j + 1] == false) {
					revealSpace(i, j + 1);
				}
				if (i < fieldX - 1 && j > 0 && spacesRevealed[i + 1][j - 1] == false && flagged[i + 1][j - 1] == false) {
					revealSpace(i + 1, j - 1);
				}

				if (i < fieldX - 1 && spacesRevealed[i + 1][j] == false && flagged[i + 1][j] == false) {
					revealSpace(i + 1, j);
				}
				if (i < fieldX - 1 && j < fieldY - 1 && spacesRevealed[i + 1][j + 1] == false && flagged[i + 1][j + 1] == false) {
					revealSpace(i + 1, j + 1);
				}
			}

			// game won (all spaces were discovered)
			if (safeSpacesAmount <= 0) {
				gameWon();
			}
		}
	}

	// reveal the entire field (including bombs)
	void revealAllBombs() {

		for (int i = 0; i < fieldX; ++i) {
			for (int j = 0; j < fieldY; ++j) {
				if (fieldStatus[i][j] >= 10 && spacesRevealed[i][j] == false) {

					spacesSpr[i][j].setTexture(textures[0][1]); // normal bomb
				}
				spacesRevealed[i][j] = true;
			}
		}
	}

	void gameWon() {
		revealAllBombs();
	}

	void gameLost() {
		revealAllBombs();
	}

	// print all positions of the field showing the number of each space or if its a bomb (xx)
	void coutFieldStatus() {

		spaces = "";

		for (int i = 0; i < fieldX; i++) {
			spaces += "|_____";
		}
		spaces += "|\n";

		cout << "Field status:\n" << spaces;
		for (int i = 0; i < fieldY; ++i) {
			for (int j = 0; j < fieldX; ++j) {
				if (fieldStatus[j][i] < 10) {
					cout << "| 0" << fieldStatus[j][i] << "  ";
				}
				else {
					cout << "| xx  ";
				}
			}
			cout << "|\n" << spaces;
		}
		cout << endl;
	}

	// print all positions of the field showing if its revealed or not (0 or 1)
	void coutSpacesRevealed() {

		spaces = "";

		for (int i = 0; i < fieldX; i++) {
			spaces += "|_____";
		}
		spaces += "|\n";


		cout << "Spaces revealed:\n" << spaces;
		for (int i = 0; i < fieldY; ++i) {
			for (int j = 0; j < fieldX; ++j) {
				cout << "|  " << spacesRevealed[j][i] << "  ";
			}
			cout << "|\n" << spaces;
		}
		cout << endl;
	}

	// print all positions of the field with its coordinates
	void coutPositions() {

		spaces = "";

		for (int i = 0; i < fieldX; i++) {
			spaces += "|_____";
		}
		spaces += "|\n";

		cout << "Field positions:\n" << spaces;
		for (int i = 0; i < fieldY; ++i) {
			for (int j = 0; j < fieldX; ++j) {
				cout << "|(" << j << "," << i << ")";
			}
			cout << "|\n" << spaces;
		}
		cout << endl;
	}

	void coutSpacesLeft() {

		cout << safeSpacesAmount << " spaces left\n";
	}

public:

	// set all initial bombs and resets spaces
	void setBombs() {

		if (bombAmount > (fieldX * fieldY)) {
			bombAmount = (fieldX * fieldY) - 1;
		}

		int unplantedBombs = bombAmount;
		int randomX;
		int randomY;

		srand(time(NULL));

		while (unplantedBombs > 0) {

			randomX = rand() % fieldX;
			randomY = rand() % fieldY;

			if (fieldStatus[randomX][randomY] < 10) {

				// easily optimizable, could do with another for, also would really avoid bugs and iterations if it was an array
				if (randomX > 0 && randomY > 0) {
					fieldStatus[randomX - 1][randomY - 1] += 1;
				}
				if (randomY > 0) {
					fieldStatus[randomX][randomY - 1] += 1;
				}
				if (randomX < fieldX - 1 && randomY > 0) {
					fieldStatus[randomX + 1][randomY - 1] += 1;
				}
				if (randomX > 0) {
					fieldStatus[randomX - 1][randomY] += 1;
				}

				fieldStatus[randomX][randomY] += 10;

				if (randomX < fieldX - 1) {
					fieldStatus[randomX + 1][randomY] += 1;
				}
				if (randomX > 0 && randomY < fieldY - 1) {
					fieldStatus[randomX - 1][randomY + 1] += 1;
				}
				if (randomY < fieldY - 1) {
					fieldStatus[randomX][randomY + 1] += 1;
				}
				if (randomX < fieldX - 1 && randomY < fieldY - 1) {
					fieldStatus[randomX + 1][randomY + 1] += 1;
				}

				unplantedBombs--;
			}
		}
	}

	// to check if any space is being clicked, it needs a hitbox, and its better to set it a single time since the field doesn't do any transform operation
	void getSpacesHitbox() {
		for (int i = 0; i < fieldX; ++i) {
			for (int j = 0; j < fieldY; ++j) {
				spacesHitbox[i][j] = spacesSpr[i][j].getGlobalBounds();
			}
		}
	}

	// handles all key presses except esc because it is handled where closes the game
	void handleKeyboard(sf::Event event) {

		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::R) { // R to reset the game

				gameSet = false;
			} else if (event.key.code == sf::Keyboard::Z) { // Z for a clue for where each bomb is

				coutFieldStatus();
			} else if (event.key.code == sf::Keyboard::X) { // X to show what is revealed or not in the field

				coutSpacesRevealed();
			} else if (event.key.code == sf::Keyboard::C) {	// C to show all coordinates in the field

				coutPositions();
			} else if (event.key.code == sf::Keyboard::Space) {	// Space bar to show how many spaces are left

				coutSpacesLeft();
			}
		}
	}

	// the moment the mouse button is pressed over a non-revealed space, marks it as held
	void leftMousePressed(sf::Event event) {
		if ((holding == false) && (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Middle))) {

			// transform the actual mouse position from window coordinates to world coordinates
			sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

			holding = true;
			breakLoop = false;

			// if any hitbox contains the mouse in the moment the button is pressed
			for (int i = 0; i < fieldX; ++i) {
				for (int j = 0; j < fieldY; ++j) {

					if (spacesHitbox[i][j].contains(mouse)) {

						// if its an unrevealed spot
						if (spacesRevealed[i][j] == false) {
							spacesSpr[i][j].setTexture(textures[1][0]); // pressed space sprite
							heldSpace[0] = i;
							heldSpace[1] = j;
						}

						//else (or when finished) flags to break both loops
						breakLoop = true;
						break;
					}
				}
				if (breakLoop == true) {
					break;
				}
			}

		}
	}

	// the moment the left mouse button get released, if it is over a non-revealed space, reveals it
	void leftMouseReleased(sf::Event event) {

		if ((event.type == sf::Event::MouseButtonReleased) && (event.mouseButton.button == sf::Mouse::Left)) {

			// transform the actual mouse position from window coordinates to world coordinates
			sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

			holding = false;
			breakLoop = false;

			// if the held space is the same the mouse got released, reveals the space, else it turns back to default texture
			if (spacesHitbox[heldSpace[0]][heldSpace[1]].contains(mouse)) {

				revealSpace(heldSpace[0], heldSpace[1]);
			} else {

				spacesSpr[heldSpace[0]][heldSpace[1]].setTexture(textures[0][0]);
			}
			heldSpace[0] = -1;
			heldSpace[1] = -1;
		}
	}

	// if a non-revealed space is right-clicked by the player, mark it with a flag or remove the flag
	void rightMouseClick(sf::Event event) {

		if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Right)) {

			// transform the actual mouse position from window coordinates to world coordinates
			sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

			breakLoop = false;

			// if any hitbox contains the mouse in the moment the button is pressed
			for (int i = 0; i < fieldX; ++i) {
				for (int j = 0; j < fieldY; ++j) {

					if (spacesHitbox[i][j].contains(mouse)) {

						// if its an unrevealed spot
						if (spacesRevealed[i][j] == false) {

							flagged[i][j] = -flagged[i][j]; // !flagged[i][j] not working

							if (flagged[i][j] == false) {

								spacesSpr[i][j].setTexture(textures[3][0]); // flagged space sprite
								flagged[i][j] = true;
							} else if (flagged[i][j] == true) {

								spacesSpr[i][j].setTexture(textures[0][0]); // default space sprite
								flagged[i][j] = false;
							}
						}

						//else (or when finished) flags to break both loops
						breakLoop = true;
						break;
					}
				}
				if (breakLoop == true) {
					break;
				}
			}

		}
	}

	// if a non-revealed space is middle-clicked by the player (mouse-wheel click) and the player have pliers, reveals the spot, and if there is a bomb disarms it.
	void middleMouseClick(sf::Event event) {

		if ((event.type == sf::Event::MouseButtonReleased) && (event.mouseButton.button == sf::Mouse::Middle)) {
			
			holding = false;
			if (pliersAmout > 0) {

				// transform the actual mouse position from window coordinates to world coordinates
				sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				breakLoop = false;

				if (spacesHitbox[heldSpace[0]][heldSpace[1]].contains(mouse)) {

					// if its an unrevealed spot
					if (spacesRevealed[heldSpace[0]][heldSpace[1]] == false) {

						pliersAmout--;
						spacesRevealed[heldSpace[0]][heldSpace[1]] = true;

						if (fieldStatus[heldSpace[0]][heldSpace[1]] >= 10) {

							spacesSpr[heldSpace[0]][heldSpace[1]].setTexture(textures[0][1]); // normal bomb
						}
						else {
							safeSpacesAmount--;
							spacesSpr[heldSpace[0]][heldSpace[1]].setTexture(textures[3][1]); // disarmer
						}

					}
				} else {

					spacesSpr[heldSpace[0]][heldSpace[1]].setTexture(textures[0][0]);
				}
				heldSpace[0] = -1;
				heldSpace[1] = -1;
			} else {

				spacesSpr[heldSpace[0]][heldSpace[1]].setTexture(textures[0][0]);
			}
		}
	}
};

int main() {

	// Declared classes as objects
	GameSettings settings;
	Screen screen;
	Game game;
	//---------------------------

	while (window.isOpen()) {

		// Setup the starting game
		if (gameSet == false) {

			settings.getSpacesTextures(); // initialize the "textures" variable
			settings.setSpacesSprites(); // set all sprites from the field with the default space texture

			settings.zeroFieldStatus(); // guarantee that all arrays are with default values

			screen.setBackground(); // sets how the background will appear

			game.setBombs(); // setup the initial bombs
			game.getSpacesHitbox(); // get all hitboxes for the spaces 

			safeSpacesAmount = fieldX * fieldY - bombAmount;
			pliersAmout = 3;

			gameSet = true;
		}

		sf::Event event;
		while (window.pollEvent(event)) {

			// close app
			if ((event.type == sf::Event::Closed) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))) {

				window.close();
			}
			//------------------

			// keyboard check
			game.handleKeyboard(event);
			//-------------------

			// mouse check
			game.leftMousePressed(event);
			game.leftMouseReleased(event);

			game.rightMouseClick(event);

			game.middleMouseClick(event);
			//-------------------

			// screen manipulation
			screen.drawScreen();
			//-------------------
		}
	}

	return 0;
}