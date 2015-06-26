#include "StdAfx.h"
#include "Troll.h"


Troll::Troll(float x, float y, float vx, float vy)
{
	Load("images/troll.png");
	assert(IsLoaded());

	SetPosition(x, y);

	m_xVelocity = vx;
	m_yVelocity = vy;

	GetSprite().setOrigin(15, 15);
}


Troll::~Troll()
{
}


sf::Vector2f Troll::GetVelocity()
{
	sf::Vector2f v(m_xVelocity, m_yVelocity);
	return(v);
}

void Troll::Update(float elapsedTime)
{
	GetSprite().move(m_xVelocity * elapsedTime, m_yVelocity * elapsedTime);
}