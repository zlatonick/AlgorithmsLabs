#include "Render.h"

Render* Render::thisObject = nullptr;

Render::Render()
{
	if (thisObject != nullptr)
		exit(EXIT_FAILURE);
	thisObject = this;

	// Creating the window
	this->window = new sf::RenderWindow(sf::VideoMode(1366, 768), "Game", sf::Style::Fullscreen);

	// Creating the texture loader
	this->textureLoader = new TextureLoader();

	// Loading the textures
	this->initTextures();

	// Creating the world
	this->world = new World();
}

Render::~Render() {
	delete this->window;
	delete this->textureLoader;
	delete this->world;
}

void Render::initTextures() {
	// Loading the sprites
	this->textureLoader->addTexture("Resources/background_menu.png");		// 0
	this->textureLoader->addTexture("Resources/button3.png");				// 1
	this->textureLoader->addTexture("Resources/button3_h.png");				// 2
	this->textureLoader->addTexture("Resources/button4.png");				// 3
	this->textureLoader->addTexture("Resources/button4_h.png");				// 4
	this->textureLoader->addTexture("Resources/button5.png");				// 5
	this->textureLoader->addTexture("Resources/button5_h.png");				// 6
	this->textureLoader->addTexture("Resources/button6.png");				// 7
	this->textureLoader->addTexture("Resources/button6_h.png");				// 8
	this->textureLoader->addTexture("Resources/player.png");				// 9
	this->textureLoader->addTexture("Resources/player_h.png");				// 10
	this->textureLoader->addTexture("Resources/computer.png");				// 11
	this->textureLoader->addTexture("Resources/computer_h.png");			// 12
	this->textureLoader->addTexture("Resources/easy.png");					// 13
	this->textureLoader->addTexture("Resources/easy_h.png");				// 14
	this->textureLoader->addTexture("Resources/medium.png");				// 15
	this->textureLoader->addTexture("Resources/medium_h.png");				// 16
	this->textureLoader->addTexture("Resources/hard.png");					// 17
	this->textureLoader->addTexture("Resources/hard_h.png");				// 18

	this->textureLoader->addTexture("Resources/background_game.png");		// 19
	this->textureLoader->addTexture("Resources/back.png");					// 20
	this->textureLoader->addTexture("Resources/field3.png");				// 21
	this->textureLoader->addTexture("Resources/field4.png");				// 22
	this->textureLoader->addTexture("Resources/field5.png");				// 23
	this->textureLoader->addTexture("Resources/field6.png");				// 24
	this->textureLoader->addTexture("Resources/whiteDot3.png");				// 25
	this->textureLoader->addTexture("Resources/whiteDot4.png");				// 26
	this->textureLoader->addTexture("Resources/whiteDot5.png");				// 27
	this->textureLoader->addTexture("Resources/whiteDot6.png");				// 28
	this->textureLoader->addTexture("Resources/whiteEdgeHor3.png");			// 29
	this->textureLoader->addTexture("Resources/whiteEdgeHor4.png");			// 30
	this->textureLoader->addTexture("Resources/whiteEdgeHor5.png");			// 31
	this->textureLoader->addTexture("Resources/whiteEdgeHor6.png");			// 32
	this->textureLoader->addTexture("Resources/whiteEdgeVer3.png");			// 33
	this->textureLoader->addTexture("Resources/whiteEdgeVer4.png");			// 34
	this->textureLoader->addTexture("Resources/whiteEdgeVer5.png");			// 35
	this->textureLoader->addTexture("Resources/whiteEdgeVer6.png");			// 36
	this->textureLoader->addTexture("Resources/blackDot3.png");				// 37
	this->textureLoader->addTexture("Resources/blackDot4.png");				// 38
	this->textureLoader->addTexture("Resources/blackDot5.png");				// 39
	this->textureLoader->addTexture("Resources/blackDot6.png");				// 40
	this->textureLoader->addTexture("Resources/blackEdgeHor3.png");			// 41
	this->textureLoader->addTexture("Resources/blackEdgeHor4.png");			// 42
	this->textureLoader->addTexture("Resources/blackEdgeHor5.png");			// 43
	this->textureLoader->addTexture("Resources/blackEdgeHor6.png");			// 44
	this->textureLoader->addTexture("Resources/blackEdgeVer3.png");			// 45
	this->textureLoader->addTexture("Resources/blackEdgeVer4.png");			// 46
	this->textureLoader->addTexture("Resources/blackEdgeVer5.png");			// 47
	this->textureLoader->addTexture("Resources/blackEdgeVer6.png");			// 48
	this->textureLoader->addTexture("Resources/playerWon.png");				// 49
	this->textureLoader->addTexture("Resources/computerWon.png");			// 50
}

void Render::rendering() {
	sf::Clock timeDelay;
	unsigned int timeDelayMCS;				// Time passed from last iteration in MICROSECONDS

	// Time out for mouse
	bool mouseBlock = false;
	unsigned int mouseTime = 0;
	const int MOUSE_TIMEOUT = 300000;	// Time out durability

	while (this->window->isOpen()) {
		timeDelayMCS = timeDelay.getElapsedTime().asMicroseconds();
		timeDelay.restart();

		this->window->clear();
		this->world->display(this->window, timeDelayMCS);
		this->window->display();

		sf::Event ev;
		while (this->window->pollEvent(ev));
		{
			if (ev.type == sf::Event::Closed) {
				this->window->close();
			}

			// Mouse handling
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (!mouseBlock) {
					mouseBlock = true;

					sf::Vector2i position = sf::Mouse::getPosition();

					// DEBUG
					//std::cout << "Mouse click: " << position.x << ", " << position.y << std::endl;

					this->world->mouseClick(position.x, position.y);
				}
			}
			if (mouseBlock) {
				mouseTime += timeDelayMCS;

				if (mouseTime > MOUSE_TIMEOUT) {
					mouseTime = 0;
					mouseBlock = false;
				}
			}			
		}
	}
}
