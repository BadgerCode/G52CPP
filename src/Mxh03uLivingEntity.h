#pragma once
#include "Mxh03uDisplayableObject.h"
#include "JPGImage.h"

class Mxh03uMain;

class Mxh03uLivingEntity :
	public Mxh03uDisplayableObject
{
public:
	Mxh03uLivingEntity(Mxh03uMain* pEngine);
	~Mxh03uLivingEntity();

	// Extra functionality

	// Life & Death
	virtual void Spawn(int iSpawnX, int iSpawnY);
	virtual void Kill(){ this->m_bIsDead = true; SetVisible(false); }
	bool IsDead() const{ return this->m_bIsDead; }

	// Movement
	// Validates a living entity's request to move somewhere
	void DoMove(int iX, int iY);

	// Health
	int GetHealth() const{ return this->m_iHealth; };
	int GetMaxHealth() const{ return this->m_iMaxHealth; }
	// Allow for more complex objects which do more things on receiving damage
	virtual void Damage(int iAmount){ if((this->m_iHealth -= iAmount) <= 0)Kill(); };
	virtual void Heal(int iAmount){ if ((this->m_iHealth += iAmount) > m_iMaxHealth)m_iHealth = m_iMaxHealth; }
	void SetHealth(int iAmount){ this->m_iHealth = iAmount; };

	// Speed
	int GetSpeed() const{ return this->m_iSpeed; };
	void SetSpeed(int speed){ this->m_iSpeed = speed; };

	// Sprites
	void LoadSprites(char* baseSpritePath, char* baseLeftSpritePath = NULL, char* actionSpritePath = NULL, char* actionLeftSpritePath = NULL){
		if (!m_oBaseImage.LoadImage(baseSpritePath))return;

		m_bUseSprites = true;

		if (baseLeftSpritePath == NULL || !m_oBaseLeftImage.LoadImage(baseLeftSpritePath)){
			m_oBaseLeftImage = m_oBaseImage;
		}
		if (actionSpritePath == NULL || !m_oBaseActionImage.LoadImage(actionSpritePath)){
			m_oBaseActionImage = m_oBaseImage;
		}
		if (actionLeftSpritePath == NULL || !m_oBaseLeftActionImage.LoadImage(actionLeftSpritePath)){
			m_oBaseLeftActionImage = m_oBaseImage;
		}
	}

protected:
	bool m_bIsDead = true;

	int m_iMaxHealth = 100;
	int m_iHealth = 0;

	int m_iStartSpeed = 2;
	int m_iSpeed = 0;

	// Sprites
	bool m_bUseSprites = false;
	ImageData m_oBaseImage;
	ImageData m_oBaseLeftImage;
	ImageData m_oBaseActionImage;
	ImageData m_oBaseLeftActionImage;
};

