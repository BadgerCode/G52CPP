#include "header.h"
#include "templates.h"

#include "Mxh03uLivingEntity.h"
#include "Mxh03uMain.h"

Mxh03uLivingEntity::Mxh03uLivingEntity(Mxh03uMain* pEngine)
:Mxh03uDisplayableObject(pEngine)
{
	// Give these safe values until it is spawned
	m_iCurrentScreenX = 0;
	m_iCurrentScreenY = 0;

	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;

	m_iDrawHeight = 0;
	m_iDrawWidth = 0;

	SetVisible(false);
}


Mxh03uLivingEntity::~Mxh03uLivingEntity()
{
}


void Mxh03uLivingEntity::Spawn(int iSpawnX, int iSpawnY)
{
	// Displayable object properties
	m_iCurrentScreenX = iSpawnX;
	m_iCurrentScreenY = iSpawnY;
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	m_iDrawWidth = 30;
	m_iDrawHeight = 50;

	m_iStartDrawPosX = -(m_iDrawWidth / 2);
	m_iStartDrawPosY = -(m_iDrawHeight / 2);

	SetVisible(true);


	// Living entity properties
	m_bIsDead = false;
	m_iHealth = m_iMaxHealth;
	m_iSpeed = m_iStartSpeed;
}


// Validates a living entity's request to move somewhere
void Mxh03uLivingEntity::DoMove(int iX, int iY){
	int newX = iX;
	int newY = iY;
	int minX = m_pRealEngine->GetGameScreenMinX();
	int maxX = m_pRealEngine->GetGameScreenMaxX();
	int minY = m_pRealEngine->GetGameScreenMinY();
	int maxY = m_pRealEngine->GetGameScreenMaxY();

	// Moving right
	if (newX > m_iCurrentScreenX && (newX + m_iStartDrawPosX + m_iDrawWidth) > maxX){
		newX = maxX - m_iStartDrawPosX - m_iDrawWidth;
	}
	// Moving left
	else if (newX < m_iCurrentScreenX && (newX + m_iStartDrawPosX) < minX){
		newX = minX - m_iStartDrawPosX;
	}

	// Moving down
	if (newY > m_iCurrentScreenY && (newY + m_iStartDrawPosY + m_iDrawHeight) > maxY){
		newY = maxY - m_iStartDrawPosY - m_iDrawHeight;
	}
	// Moving up
	else if (newY < m_iCurrentScreenY && (newY + m_iStartDrawPosY) < minY){
		newY = minY - m_iStartDrawPosY;
	}

	// Update our position
	m_iCurrentScreenX = newX;
	m_iCurrentScreenY = newY;
}