#include "header.h"
#include "templates.h"

#include "Mxh03uMain.h"
#include "Mxh03uPlayer.h"
#include "Mxh03uEnemyBase.h"

Mxh03uPlayer::Mxh03uPlayer(Mxh03uMain* pEngine)
: Mxh03uLivingEntity(pEngine)
{
	m_iMaxHealth = START_HEALTH;
	m_iStartSpeed = START_SPEED;

	// Load sprites
	m_oBaseSprite.LoadImage("player.png");
	m_oMoveSprite.LoadImage("player-move.png");
	m_oAttackSprite.LoadImage("player-attack.png");

	m_oLeftBaseSprite.LoadImage("player-left.png");
	m_oLeftMoveSprite.LoadImage("player-move-left.png");
	m_oLeftAttackSprite.LoadImage("player-attack-left.png");
}


Mxh03uPlayer::~Mxh03uPlayer()
{
}

void Mxh03uPlayer::Spawn(int iSpawnX, int iSpawnY)
{
	// Displayable object properties
	m_iCurrentScreenX = iSpawnX;
	m_iCurrentScreenY = iSpawnY;
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	m_iStartDrawPosX = -20;
	m_iStartDrawPosY = -30;

	m_iDrawHeight = 60;
	m_iDrawWidth = 40;

	SetVisible(true);


	// Living entity properties
	m_bIsDead = false;
	m_iHealth = m_iMaxHealth;
	m_iSpeed = m_iStartSpeed;
}


void Mxh03uPlayer::Draw()
{
	if (!IsVisible() || m_bIsDead)
		return;

	// If the playing is attacking
	if (GetEngine()->GetModifiedTime() - m_iLastAttack < m_iAttackLength){
		if (m_iDirection == LEFT || m_iDirection == BOTLEFT || m_iDirection == TOPLEFT){
			m_oLeftAttackSprite.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0,
				m_iCurrentScreenX + m_iStartDrawPosX,
				m_iCurrentScreenY + m_iStartDrawPosY,
				40, 60);
		}
		else {
			m_oAttackSprite.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0,
				m_iCurrentScreenX + m_iStartDrawPosX,
				m_iCurrentScreenY + m_iStartDrawPosY,
				40, 60);
		}
	}
	// Or standing still
	else if (!m_bMoving){
		if (m_iDirection == LEFT || m_iDirection == BOTLEFT || m_iDirection == TOPLEFT){
			m_oLeftBaseSprite.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0,
				m_iCurrentScreenX + m_iStartDrawPosX,
				m_iCurrentScreenY + m_iStartDrawPosY,
				40, 60);
		}
		else {
			m_oBaseSprite.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0,
				m_iCurrentScreenX + m_iStartDrawPosX,
				m_iCurrentScreenY + m_iStartDrawPosY,
				40, 60);
		}
	}
	else {
		if (m_iDirection == LEFT || m_iDirection == BOTLEFT || m_iDirection == TOPLEFT){
			m_oLeftMoveSprite.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0,
				m_iCurrentScreenX + m_iStartDrawPosX,
				m_iCurrentScreenY + m_iStartDrawPosY,
				40, 60);
		}
		else {
			m_oMoveSprite.RenderImageWithMask(GetEngine()->GetForeground(), 0, 0,
				m_iCurrentScreenX + m_iStartDrawPosX,
				m_iCurrentScreenY + m_iStartDrawPosY,
				40, 60);
		}
	}

	StoreLastScreenPositionForUndraw();
}


void Mxh03uPlayer::DoUpdate(int iCurrentTime)
{
	if (m_bIsDead)return;

	int xDirection = 0, yDirection = 0, newFacingDirection = m_iDirection;
	m_bMoving = false;
	if (GetEngine()->IsKeyPressed(SDLK_w) || GetEngine()->IsKeyPressed(SDLK_UP)){
		yDirection--;
	}
	if (GetEngine()->IsKeyPressed(SDLK_s) || GetEngine()->IsKeyPressed(SDLK_DOWN)){
		yDirection++;
	}
	if (GetEngine()->IsKeyPressed(SDLK_a) || GetEngine()->IsKeyPressed(SDLK_LEFT)){
		xDirection--;
	}
	if (GetEngine()->IsKeyPressed(SDLK_d) || GetEngine()->IsKeyPressed(SDLK_RIGHT)){
		xDirection++;
	}

	// Update direction
	if (xDirection > 0){
		if (yDirection > 0)newFacingDirection = BOTRIGHT;
		else if (yDirection < 0)newFacingDirection = TOPRIGHT;
		else newFacingDirection = RIGHT;
	}
	else if (xDirection < 0){
		if (yDirection > 0)newFacingDirection = BOTLEFT;
		else if (yDirection < 0)newFacingDirection = TOPLEFT;
		else newFacingDirection = LEFT;
	}
	else {
		if (yDirection > 0)newFacingDirection = DOWN;
		else if (yDirection < 0)newFacingDirection = UP;
	}

	m_iDirection = newFacingDirection;
	if (xDirection || yDirection)m_bMoving = true;

	// Update position
	DoMove(m_iCurrentScreenX + xDirection * m_iSpeed, m_iCurrentScreenY + yDirection * m_iSpeed);

	// Attack
	if (GetEngine()->IsKeyPressed(SDLK_SPACE) && GetEngine()->GetModifiedTime() - m_iLastAttack > m_iAttackDelay){
		m_iLastAttack = GetEngine()->GetModifiedTime();
		DoAttack();
	}

	RedrawObjects();
}

void Mxh03uPlayer::DoAttack()
{
	Mxh03uEnemyBase* enemy;
	bool enemyInRange;
	for (int i = 0; (enemy = m_pRealEngine->GetEnemy(i)) != NULL; i++){
		int diffX = enemy->GetXCentre() - this->GetXCentre();
		int diffY = enemy->GetYCentre() - this->GetYCentre();
		enemyInRange = false;

		// Check range
		if (diffX * diffX + diffY * diffY <= m_iAttackRange * m_iAttackRange){
			// Check direction
			switch (m_iDirection){
			case UP:
				if (diffY <= diffX && diffX <= -diffY)enemyInRange = true;
				break;
			case TOPRIGHT:
				if (diffY <= 0 && diffX >= 0)enemyInRange = true;
				break;
			case RIGHT:
				if (-diffX <= diffY && diffY <= diffX)enemyInRange = true;
				break;
			case BOTRIGHT:
				if (diffY >= 0 && diffX >= 0)enemyInRange = true;
				break;
			case DOWN:
				if (-diffY <= diffX && diffX <= diffY)enemyInRange = true;
				break;
			case BOTLEFT:
				if (diffY >= 0 && diffX <= 0)enemyInRange = true;
				break;
			case LEFT:
				if (diffX <= diffY && diffY <= -diffX)enemyInRange = true;
				break;
			case TOPLEFT:
				if (diffY <= 0 && diffX <= 0)enemyInRange = true;
				break;
			}
		}

		if (enemyInRange){
			enemy->Kill();
			m_pRealEngine->AddKill();
			this->Heal(5);
		}
	}
}