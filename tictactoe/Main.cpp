#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

const int CELL_SIZE = 150;
const int WIN_SIZE = CELL_SIZE * 3;
const int CURSOR_SIZE = 30;
const std::string RSC_PATH = "..\\resources\\";

sf::RectangleShape bg(sf::Vector2f(WIN_SIZE, WIN_SIZE)), x(sf::Vector2f(CELL_SIZE, CELL_SIZE)), o = x, cursor(sf::Vector2f(CURSOR_SIZE, CURSOR_SIZE));
sf::RenderWindow window(sf::VideoMode(WIN_SIZE, WIN_SIZE), "tic-tac-toe", sf::Style::Close);
sf::Texture bgTxt, xTxt, oTxt, xCursorTxt, oCursorTxt;
sf::SoundBuffer clickSB, gameOverSB;
sf::Sound clickSound, gameOverSound;
sf::Font font;
sf::Text text;

int game[3][3];
int emptyCellVal;
int emptyCellsLeft = 9;
bool isTurnOfX = true;

void renderCursor() {
	sf::Vector2i pos = sf::Mouse::getPosition(window);
	cursor.setPosition(pos.x, pos.y);
	window.draw(cursor);
}

void renderGame() {
	window.draw(bg);
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++)
		{
			int cellVal = game[col][row];
			if (cellVal == 1) {
				x.setPosition(col * CELL_SIZE, row * CELL_SIZE);
				window.draw(x);
			}
			else if (cellVal == -1) {
				o.setPosition(col * CELL_SIZE, row * CELL_SIZE);
				window.draw(o);
			}
		}
	}
}

void init() {
	bgTxt.setSmooth(true);
	xTxt.setSmooth(true);
	oTxt.setSmooth(true);

	//ew
	if (!bgTxt.loadFromFile(RSC_PATH + "bg.png") || !xTxt.loadFromFile(RSC_PATH + "x.png") || !oTxt.loadFromFile(RSC_PATH + "o.png")
		||!xCursorTxt.loadFromFile(RSC_PATH + "cursorX.png") || !oCursorTxt.loadFromFile(RSC_PATH + "cursorO.png"))
	{
		window.setTitle("TEXTURE ERROR!");
	}
	if (!clickSB.loadFromFile(RSC_PATH + "click.wav") || !gameOverSB.loadFromFile(RSC_PATH + "gameOver.wav")) {
		window.setTitle("SOUND ERROR!");
	}
	if (!font.loadFromFile(RSC_PATH + "BrisaSans.otf")) {
		window.setTitle("FONT ERROR!");
	}

	bg.setTexture(&bgTxt);
	x.setTexture(&xTxt);
	o.setTexture(&oTxt);

	cursor.setTexture(&xCursorTxt);
	cursor.setOrigin(CURSOR_SIZE / 2, CURSOR_SIZE / 2);

	clickSound.setBuffer(clickSB);
	gameOverSound.setBuffer(gameOverSB);

	text.setFont(font);

	emptyCellVal = game[0][0];

	window.draw(bg);
	window.display();
}

void gameLoop();

void reset() {
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			game[x][y] = emptyCellVal;
		}
	}
	emptyCellsLeft = 9;
	isTurnOfX = true;
}

void gameOverScreen(std::string message) {
	text.setString(message + " Tap to Restart");
	text.setFillColor(sf::Color(15, 15, 15));
	text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
	text.setPosition(WIN_SIZE / 2, WIN_SIZE / 2);
	gameOverSound.play();

	sf::Event event;
	sf::RectangleShape panel(sf::Vector2f(WIN_SIZE, WIN_SIZE));
	panel.setFillColor(sf::Color(255, 255, 255, 190));

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				gameLoop();
			}
		}

		renderGame();
		window.draw(panel);
		window.draw(text);
		renderCursor();
		window.display();
	}
}

void gameLoop() {
	reset();

	window.setMouseCursorVisible(false);
	sf::Event event;

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				int mouseX = sf::Mouse::getPosition(window).x / CELL_SIZE;
				int mouseY = sf::Mouse::getPosition(window).y / CELL_SIZE;

				if (game[mouseX][mouseY] == emptyCellVal) {
					game[mouseX][mouseY] = isTurnOfX ? 1 : -1;
					emptyCellsLeft--;

					//Checking if someone won the game
					bool isGameWon =
						game[1][1] != emptyCellVal && (game[0][0] == game[1][1] && game[0][0] == game[2][2] || game[0][2] == game[1][1] && game[0][2] == game[2][0]) || // \ and /
						game[mouseX][0] == game[mouseX][1] && game[mouseX][0] == game[mouseX][2] || game[0][mouseY] == game[1][mouseY] && game[0][mouseY] == game[2][mouseY]; // - and | 

					if (isGameWon)
						gameOverScreen(isTurnOfX ? "'X' Won!" : "'O' Won!");
					else if (emptyCellsLeft == 0)
						gameOverScreen("Tie.");

					isTurnOfX = !isTurnOfX;
					cursor.setTexture(isTurnOfX ? &xCursorTxt : &oCursorTxt);
					clickSound.setPitch(rand() % 10 * 0.1f + 0.75f);
					clickSound.play();
				}
			}
		}

		renderGame();
		renderCursor();
		window.display();
	}
}

int main() {
	init();
	gameLoop();
}