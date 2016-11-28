#include "header.h"
#include "templates.h"

#include "Mxh03uEnemyBase.h"
#include "Mxh03uMain.h"


Mxh03uEnemyBase::Mxh03uEnemyBase(Mxh03uMain* pEngine)
:Mxh03uLivingEntity(pEngine)
{
}


Mxh03uEnemyBase::~Mxh03uEnemyBase()
{
}

void Mxh03uEnemyBase::Draw()
{
	if (!IsVisible() || m_bIsDead)
		return;
	if (!m_bUseSprites){
		Mxh03uLivingEntity::Draw();
	}
	else {
		if (GetEngine()->GetModifiedTime() - m_iLastAttack <= m_iAttackLength){
			if (m_bFacingLeft){
				m_oBaseLeftActionImage.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0,
					m_iCurrentScreenX + m_iStartDrawPosX,
					m_iCurrentScreenY + m_iStartDrawPosY,
					30, 50);
			}
			else {
				m_oBaseActionImage.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0,
					m_iCurrentScreenX + m_iStartDrawPosX,
					m_iCurrentScreenY + m_iStartDrawPosY,
					30, 50);
			}
		}
		else if (m_bFacingLeft){
			m_oBaseLeftImage.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0,
				m_iCurrentScreenX + m_iStartDrawPosX,
				m_iCurrentScreenY + m_iStartDrawPosY,
				30, 50);
		}
		else {
			m_oBaseImage.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0,
				m_iCurrentScreenX + m_iStartDrawPosX,
				m_iCurrentScreenY + m_iStartDrawPosY,
				30, 50);
		}
	}

	StoreLastScreenPositionForUndraw();
}


void Mxh03uEnemyBase::DoUpdate(int iCurrentTime)
{
	// Dead things don't move (normally)
	if (m_bIsDead)return;

	int newX = m_iCurrentScreenX, newY = m_iCurrentScreenY;

	// Move towards player
	Mxh03uPlayer& player = m_pRealEngine->GetPlayer();

	int diffX = player.GetXCentre() - GetXCentre();
	int diffY = player.GetYCentre() - GetYCentre();
	if (diffX == 0){
		newY += SDL_copysign(m_iSpeed, diffY);
	}
	else if (diffY == 0){
		newX += SDL_copysign(m_iSpeed, diffX);
	}
	else {
		newX += SDL_copysign(m_iSpeed / 2, diffX);
		newY += SDL_copysign(m_iSpeed / 2, diffY);
	}

	if (newX > m_iCurrentScreenX)m_bFacingLeft = true;
	else if (newX < m_iCurrentScreenX)m_bFacingLeft = false;

	// Update position
	DoMove(newX, newY);

	// Attack the player if they're in range and our cooldown has expired
	if (iCurrentTime - m_iLastAttack >= m_iAttackDelay && (diffX * diffX) + (diffY * diffY) < m_iAttackRange * m_iAttackRange){
		player.Damage(m_iAttackDamage);
		m_iLastAttack = iCurrentTime;
	}
	RedrawObjects();
}

void Mxh03uEnemyBase::RandomSpawn()
{
	int spawnX, spawnY;
	int iShouldSpawn = rand() % 5;
	switch (iShouldSpawn){
	case 0:
		spawnX = (m_pRealEngine->GetGameScreenMaxX() - m_pRealEngine->GetGameScreenMinX()) / 2;
		spawnY = m_pRealEngine->GetGameScreenMinY() + 20;
		break;
	case 1:
		spawnX = (m_pRealEngine->GetGameScreenMaxX() - m_pRealEngine->GetGameScreenMinX()) / 2;
		spawnY = m_pRealEngine->GetGameScreenMaxY() - 20;
		break;
	case 2:
		spawnX = m_pRealEngine->GetGameScreenMinX() + 20;
		spawnY = (m_pRealEngine->GetGameScreenMaxY() - m_pRealEngine->GetGameScreenMinY()) / 2;
		break;
	case 3:
		spawnX = m_pRealEngine->GetGameScreenMaxX() - 20;
		spawnY = (m_pRealEngine->GetGameScreenMaxY() - m_pRealEngine->GetGameScreenMinY()) / 2;
		break;
	default:
		// Don't spawn anything
		return;
	}

	Spawn(spawnX, spawnY);
}



