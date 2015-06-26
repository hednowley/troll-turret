#pragma once
#include "visiblegameobject.h"


class Troll :
	public VisibleGameObject
{
public:
	Troll(float x, float y, float vx, float vy);
	virtual ~Troll();

	void Update(float elapsedTime);
	virtual sf::Vector2f GetVelocity();

private:
	float m_xVelocity, m_yVelocity;
};