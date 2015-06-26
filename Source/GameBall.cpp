#include "StdAfx.h"
#include "GameBall.h"
#include "Game.h"


GameBall::GameBall(float angle, float speed)
{
	  Load("images/ball.png");
	  assert(IsLoaded());

	  GetSprite().setOrigin(15, 15);

	  _velocity = sf::Vector2f(speed * sin(angle), speed * cos(angle));
}

void GameBall::Update(float elapsedTime)
{
	GetSprite().move(elapsedTime * _velocity.x, -1 * elapsedTime * _velocity.y);

	// Check all trolls for collision
	std::map<std::string, VisibleGameObject*> Trolls = Game::_gameObjectManager.ReturnTrolls();
	std::map<std::string, VisibleGameObject*>::const_iterator itr = Trolls.begin();
	while (itr != Trolls.end())
	{
		sf::Rect<float> trollBB = itr->second->GetBoundingRect();

		if (trollBB.intersects(GetBoundingRect()))
		// Remove troll if collided
		{
			Game::_gameObjectManager.Remove(itr->first);
		}
		itr++;
	}
}

GameBall::~GameBall()
{
}