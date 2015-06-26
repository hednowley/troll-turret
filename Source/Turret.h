#pragma once
#include "visiblegameobject.h"
#include "Troll.h"

//
// Declaration of a structure representing a 2D position or vector
//
struct TVector2D
{
	float x;
	float y;
};

//
// Declaration of the turret class
//
class CTurret : public VisibleGameObject
{
public:
	// Construction and destruction
	CTurret();
	~CTurret();
	// Per­frame update function (assume this is called for you each frame)
	void Update(float const i_fDeltaT);

private:

	void NullTargetArray();
	void UpdateTargets();
	bool FindTarget();
	bool Aim();
	void Fire();

	// A structure containing constants
	struct KTurretConsts
	{
		// The size of the target array
		static const int TargetArraySize = 32;
		// Starting/maximum ammunition
		static const int MaxAmmunition = 100;
		// Turret rotation speed in radians per second
		static const float RotationSpeed;
		// Turret projectile speed in metres per second
		static const float ProjectileSpeed;
		// Turret reload time
		static const float ReloadTime;
	};

	// Helper object representing an entry in the array of targets
	struct TTarget
	{
		TVector2D tPosition;
		TVector2D tVelocity;
	};

	// The array of targets
	TTarget* m_ptTargets[KTurretConsts::TargetArraySize];

	// Turret orientation (i.e. aiming direction) in radians (0 = straight up)
	float m_fOrientation;

	// Current ammunition supply
	int m_iAmmunition;
	
	float m_targetAngle;
	float m_reloadTimer;
	int m_ballCounter;
	int m_targetIndex;
	TTarget m_target;

	enum TurretState { Aiming, Aimed, Idle, Reloading };
	TurretState m_turretState;
};