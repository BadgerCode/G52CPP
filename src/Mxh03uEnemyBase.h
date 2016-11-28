#pragma once
#include "Mxh03uLivingEntity.h"
#include "JPGImage.h"

class Mxh03uMain;

class Mxh03uEnemyBase :
	public Mxh03uLivingEntity
{
public:
	Mxh03uEnemyBase(Mxh03uMain* pEngine);
	~Mxh03uEnemyBase();
	// Displayable object overrides
	void Draw();
	void DoUpdate(int iCurrentTime);

	// Mxh03uLivingEntity overrides
	void RandomSpawn();

	// Added functionality.
	int GetDamage() const{ return this->m_iAttackDamage; }

protected:
	int m_iAttackRange = 15;
	int m_iAttackDamage = 25;
	int m_iLastAttack = 0;
	int m_iAttackDelay = 1000;
	int m_iAttackLength = 500;

	int m_bFacingLeft = false;
};

