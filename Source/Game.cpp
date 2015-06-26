#include "stdafx.h"
#include "Game.h"
#include "SplashScreen.h"

void Game::Start(void)
{
	if(_gameState != Uninitialized)
		return;
	
	_mainWindow.create(sf::VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,32),"Troll Turret");

	std::srand((unsigned)time(0));

	CTurret *turret = new CTurret();
	turret->SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT);
	_gameObjectManager.Add("Turret", turret);

	_gameState= Game::ShowingSplash;

	while(!IsExiting()) 
		GameLoop();

	_mainWindow.close();
}

bool Game::IsExiting()
{
	if(_gameState == Game::Exiting) 
		return true;
	else 
		return false;
}


sf::RenderWindow& Game::GetWindow()
{
	return _mainWindow;
}



void Game::GameLoop()
{
	sf::Event currentEvent;
	_mainWindow.pollEvent(currentEvent);
	
	
	switch(_gameState)
	{
		case Game::ShowingSplash:
		{
			ShowSplashScreen();
			break;
		}
		case Game::Playing:
		{
			// Draw background
			sf::Texture image;
			image.loadFromFile("images/GameScreen.png");
			sf::Sprite sprite(image);
			_mainWindow.draw(sprite);

			//Draw troll on mouse click (restricted to top half of screen)
			if (currentEvent.type == sf::Event::MouseButtonPressed && currentEvent.mouseButton.button == sf::Mouse::Left)
			{
				if (_mouseState == Idle)
				{
					_mouseState = LeftPressed;

					if (currentEvent.mouseButton.y < SCREEN_HEIGHT / 2)
					{
						// Randomise velocity
						Troll *troll = new Troll(currentEvent.mouseButton.x, currentEvent.mouseButton.y, rand() % 200 - 100, rand() % 200);
						Game::_gameObjectManager.Add("Troll" + std::to_string(TrollNo), troll);
						++TrollNo;
					}
				}
			}
			if (currentEvent.type == sf::Event::MouseButtonReleased && currentEvent.mouseButton.button == sf::Mouse::Left)
				_mouseState = Idle;

			_gameObjectManager.UpdateAll();
			_gameObjectManager.DrawAll(_mainWindow);

			_mainWindow.display();
			if(currentEvent.type == sf::Event::Closed) _gameState = Game::Exiting;

			break;
		}
	}
}

void Game::ShowSplashScreen()
{
	SplashScreen splashScreen;
	splashScreen.Show(_mainWindow);
	_gameState = Game::Playing;
}

Game::MouseState Game::_mouseState = Idle;
int Game::TrollNo = 1;
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;
GameObjectManager Game::_gameObjectManager;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);