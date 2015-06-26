#include "StdAfx.h"
#include "Turret.h"
#include "Game.h"
#include "Troll.h"
#include "GameBall.h"
#include "GameObjectManager.h"

#define PI 3.141592653589793238462f

//
// Turret class definitions
//
// Static constant definitions

const float CTurret::KTurretConsts::RotationSpeed = PI * 0.5f;
const float CTurret::KTurretConsts::ProjectileSpeed = 300.0f;

// Reload time in seconds
const float CTurret::KTurretConsts::ReloadTime = 1.0f;

// Constructor
CTurret::CTurret()
	: m_fOrientation(0.0f)
	, m_iAmmunition(KTurretConsts::MaxAmmunition)
	, m_reloadTimer(KTurretConsts::ReloadTime)
	, m_ballCounter(1)
	, m_targetIndex(-1)
	, m_turretState(Idle)
{
	NullTargetArray();

	Load("images/turret.png");
	assert(IsLoaded());
	GetSprite().setOrigin(100, 85);
}

// Destructor
CTurret::~CTurret()
{
}

// Initialise target array to null
void CTurret::NullTargetArray()
{
	for (int i = 0; i < KTurretConsts::TargetArraySize; ++i)
		m_ptTargets[i] = 0;
}

void CTurret::UpdateTargets()
{
	// Set existing target array to null
	NullTargetArray();

	// Iterate through troll objects and copy position and velocity info to array
	std::map<std::string, VisibleGameObject*> Trolls = Game::_gameObjectManager.ReturnTrolls();
	std::map<std::string, VisibleGameObject*>::const_iterator itr = Trolls.begin();	
	int i = 0;
	while (itr != Trolls.end() && i < KTurretConsts::TargetArraySize)
	{
		m_ptTargets[i] = new TTarget;

		// Convert to coordinates with origin at turret (NB also correcting for SFML y axis being upside down)
		m_ptTargets[i]->tPosition.x = itr->second->GetPosition().x - GetSprite().getPosition().x;
		m_ptTargets[i]->tPosition.y = GetSprite().getPosition().y - itr->second->GetPosition().y;
		m_ptTargets[i]->tVelocity.x = itr->second->GetVelocity().x;
		m_ptTargets[i]->tVelocity.y = -1 * itr->second->GetVelocity().y;

		itr++;
		i++;
	}
}

// Finds target which will reach base first
bool CTurret::FindTarget()
{
	float t, s;
	int j = -1;

	for (int i = 0; i < KTurretConsts::TargetArraySize; ++i)
	{
		if (m_ptTargets[i] != 0)
		{
			// Only need to use y component
			s = -m_ptTargets[i]->tPosition.y / m_ptTargets[i]->tVelocity.y;

			if (j == -1)
				t = s;

			if (s <= t)
			{
				t = s;
				j = i;
			}
		}
	}

	// If at least one target found
	if (j != -1)
	{
		m_targetIndex = j;
		m_target = *m_ptTargets[j];
		return true;
	}
	else
		return false;
}

bool CTurret::Aim()
{
	//
	// We want time at which troll and bullet coincide, taking into account time for the turret to rotate.
	// This algorithm finds the time by a binary search:
	//

	// Initial limits and guess between both
	float LimitLower = 0;
	float LimitUpper = 10;
	float Guess = (LimitLower + LimitUpper) / 2;
	float LastGuess;

	// Maximum iterations
	int TimeOut = 50;

	for (int i = 0; i < TimeOut; i++)
	{
		// Troll position at guess
		float xTroll = m_target.tPosition.x + Guess*m_target.tVelocity.x;
		float yTroll = m_target.tPosition.y + Guess*m_target.tVelocity.y;

		// Calculate required rotation for turret
		float angle = atan2(xTroll, yTroll);

		while (angle <= -PI)
			angle += 2 * PI;
		while (angle >= PI)
			angle -= 2 * PI;

		float tRot = abs(m_fOrientation - angle) / KTurretConsts::RotationSpeed;

		// Calculate required time for bullet to travel to troll
		float tTravel = sqrt(xTroll * xTroll + yTroll * yTroll) / KTurretConsts::ProjectileSpeed;

		LastGuess = Guess;

		if (Guess - (tRot + tTravel) > 0.0f)
		// We can shoot the troll's position before he gets there. Hence our guess is too big
		{
			LimitUpper = Guess;
			Guess = (LimitLower + LimitUpper) / 2;
		}
		else
			// We can't shoot the troll's position before he gets there. Hence our guess is too small
		{
			LimitLower = Guess;
			Guess = (LimitLower + LimitUpper) / 2;
		}
		
		// Check if successive guesses are converging
		if (abs(Guess - LastGuess) < 0.0001f)
		{
			// Check troll will be within screen area at guess time
			sf::Rect<float> screenRect(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);
			if (screenRect.contains(xTroll + GetSprite().getPosition().x, -yTroll + GetSprite().getPosition().y))
			{
				m_targetAngle = angle;
				m_turretState = Aimed;
				return true;
			}
			else
			{
				m_turretState = Idle;
				return false;
			}
		}
	}
	return false;
}

void CTurret::Fire()
{
	// Decrement ammunition supply. Early out if we’re out of ammo.
	if (m_iAmmunition < 0)
	{
		// Out of ammo
		return;
	}

	else
	{
		// Create new bullet object and set start position to turret
		GameBall *ball = new GameBall(m_fOrientation, KTurretConsts::ProjectileSpeed);
		ball->SetPosition(GetSprite().getPosition().x, GetSprite().getPosition().y);

		// Create unique name for use by object manager
		Game::_gameObjectManager.Add("Ball" + std::to_string(m_ballCounter), ball);

		++m_ballCounter;
		--m_iAmmunition;
		m_turretState = Reloading;
	}
}

void CTurret::Update(float const i_fDeltaT)
{
	// Early out if reloading
	if (m_turretState == Reloading)
	{
		if (m_reloadTimer < i_fDeltaT)
		{
			m_reloadTimer = KTurretConsts::ReloadTime;
			m_turretState = Idle;
		}
		else
		{
			m_reloadTimer = m_reloadTimer - i_fDeltaT;
			return;
		}
	}

	if (m_turretState = Idle)
	{
		m_turretState = Aiming;

		// Update our target array with (32) trolls
		UpdateTargets();

		while (m_turretState == Aiming)
		{
			if (FindTarget())
			// At least one target
			{
				if (!Aim())
				// Target was not shootable (too fast / off-screen at impact etc). Remove this target from array and try again
				{
					m_ptTargets[m_targetIndex] = 0;
					m_turretState = Aiming;
				}
				else
					m_turretState = Aimed;
			}
			else
			// No targets!
				m_turretState = Idle;
		}
	}

	if (m_turretState == Aimed)
	// Rotate turret towards m_targetAngle
	{
		if (m_fOrientation < m_targetAngle)
			m_fOrientation += KTurretConsts::RotationSpeed * i_fDeltaT;
		if (m_fOrientation > m_targetAngle)
			m_fOrientation -= KTurretConsts::RotationSpeed * i_fDeltaT;

		if (abs(m_fOrientation - m_targetAngle) <= KTurretConsts::RotationSpeed * i_fDeltaT)
		// Turret reached target rotation so fire
		{
			m_fOrientation = m_targetAngle;
			Fire();
		}
	}

	GetSprite().setRotation(m_fOrientation * 180 / PI);
}