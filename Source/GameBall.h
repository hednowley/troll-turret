#pragma once
#include "visiblegameobject.h"


class GameBall : public VisibleGameObject
{
public:
	GameBall(float angle, float speed);
	virtual ~GameBall();
	void Update(float elapsedTime);


private:
	sf::Vector2f _velocity;
};