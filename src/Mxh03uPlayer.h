#pragma once
#include "Mxh03uLivingEntity.h"
#include "JPGImage.h"

class Mxh03uMain;

class Mxh03uPlayer :
	public Mxh03uLivingEntity
{
public:
	Mxh03uPlayer(Mxh03uMain* pEngine);
	~Mxh03uPlayer();
	// Overrides
	void Draw();
	void DoUpdate(int iCurrentTime);
	void DoAttack();

	void Spawn(int iSpawnX, int iSpawnY);

	// Starting stats
	const static int START_SPEED = 2;
	const static int START_HEALTH = 100;

	// Attack stats
	int m_iLastAttack = 0;
	int m_iAttackLength = 200;
	int m_iAttackDelay = 750;
	int m_iAttackRange = 45;

	// Direction
	enum Direction{ UP, TOPRIGHT, RIGHT, BOTRIGHT, DOWN, BOTLEFT, LEFT, TOPLEFT };
	int m_iDirection = RIGHT;
	bool m_bMoving = false;

	// Sprite
	ImageData m_oBaseSprite;
	ImageData m_oMoveSprite;
	ImageData m_oAttackSprite;
	ImageData m_oLeftBaseSprite;
	ImageData m_oLeftMoveSprite;
	ImageData m_oLeftAttackSprite;
};

