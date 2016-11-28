#include "header.h"
#include "templates.h"

#include "Mxh03uMain.h"

#include "Mxh03uEnemySkeleton.h"


Mxh03uEnemySkeleton::Mxh03uEnemySkeleton(Mxh03uMain* pEngine)
	:Mxh03uEnemyBase(pEngine)
{
	m_iMaxHealth = 10;
	m_iStartSpeed = 2;

	LoadSprites("skeleton.png", "skeleton-left.png", "skeleton-attack.png", "skeleton-attack-left.png");
}


Mxh03uEnemySkeleton::~Mxh03uEnemySkeleton()
{
}
