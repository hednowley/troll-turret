#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "GameBall.h"
#include "GameObjectManager.h"
#include "Turret.h"
#include "Troll.h"

class Game
{
public:
	static void Start();
	static sf::RenderWindow& GetWindow();
	const static int SCREEN_WIDTH = 1024;
	const static int SCREEN_HEIGHT = 768;
	const static sf::Time TimePerFrame;
	static GameObjectManager _gameObjectManager;

private:
	static bool IsExiting();
	static void GameLoop();
	static int TrollNo;
	
	static void ShowSplashScreen();

	enum GameState { Uninitialized, ShowingSplash, Paused, Playing, Exiting };
	static GameState _gameState;

	enum MouseState { Idle, LeftPressed };
	static MouseState _mouseState;

	static sf::RenderWindow _mainWindow;
};
