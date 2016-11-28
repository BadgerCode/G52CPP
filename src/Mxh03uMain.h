#pragma once

#include <vector>

#include "BaseEngine.h"
#include "Mxh03uTileManager.h"
#include "Mxh03uPlayer.h"
#include "Mxh03uEnemyBase.h"

class Mxh03uMain :
	public BaseEngine
{
public:
	Mxh03uMain();
	~Mxh03uMain();

	// BaseEngine overrides
	// Inits
	int GameInit(void);
	void GameAction();
	int InitialiseObjects();
	//Rendering
	void SetupBackgroundBuffer();
	void DrawObjects();
	void UndrawObjects();
	void DrawStringsOnTop();
	// Input
	void KeyDown(int iKeyCode);



	// Added functionality
	// States
	void StartGame(); // Called when the user starts the game from the main menu
	void ResetGame(); // Should be called before you start the game
	void UnPauseGame(){
		SetTimeOffset(m_iPauseStarted - GetTime());
		m_iState = STATE_GAME;
		Redraw(true);
	}

	// Game screen
	void SetGameScreen(int iMinX, int iMinY, int iWidth, int iHeight);
	int GetGameScreenMinX(){ return this->m_iMinGameScreenX; }
	int GetGameScreenMinY(){ return this->m_iMinGameScreenY; }
	int GetGameScreenMaxX(){ return this->m_iMaxGameScreenX; }
	int GetGameScreenMaxY(){ return this->m_iMaxGameScreenY; }

	// Tile manager
	Mxh03uTileManager& GetTileManager() { return m_oTileManager; }

	// Player
	Mxh03uPlayer& GetPlayer() const{ return *this->m_oPlayer; }

	// Scoring
	void CheckHighScore();
	void LoadHighScore();
	void SaveHighScore(int iKills, int iSeconds);
	void AddKill(){ this->m_iKills++; }

	// Enemies
	void AddEnemy();
	void KillAllEnemies();
	Mxh03uEnemyBase* GetEnemy(int index) const{
		if (m_vpEnemies.empty() || m_vpEnemies.size() <= index){
			return NULL;
		}
		return m_vpEnemies.at(index);
	}

	// Drawing
	int DrawScoreBoard(); // Returns y coordinate of last item of text
	
	/* Draw a centered string to the background */
	void DrawBackgroundStringCentered(int iX, int iY, const char* pText, unsigned int uiColour, Font* pFont = NULL)
	{
		DrawStringCentered(iX, iY, pText, uiColour, pFont, m_pBackgroundSurface);
	}

	/* Draw a centered string to the foreground */
	void DrawScreenStringCentered(int iX, int iY, const char* pText, unsigned int uiColour, Font* pFont = NULL)
	{
		DrawStringCentered(iX, iY, pText, uiColour, pFont, m_pActualScreen);
	}
	void DrawStringCentered(int iX, int iY, const char* pText,
		unsigned int uiColour, Font* pFont, SDL_Surface* pTarget);

protected:
	int m_iMinGameScreenX = 0;
	int m_iMinGameScreenY = 0;
	int m_iMaxGameScreenX = 0;
	int m_iMaxGameScreenY = 0;

private:
	// State
	enum state {STATE_MENU, STATE_GAME, STATE_PAUSED, STATE_GAMEOVER};
	int m_iState = STATE_MENU;
	int m_iPauseStarted;

	Mxh03uTileManager m_oTileManager;
	Mxh03uPlayer* m_oPlayer = NULL;

	// Scoring
	int m_iLastSecondSurvived = 0;
	int m_iKills = 0;
	int m_iSecondsSurvived = 0;
	int m_iPreviousHighestKills = 0;
	int m_iPreviousLongestLife = 0;
	const char* m_pcHighscoreFilePath;

	// Enemies
	std::vector<Mxh03uEnemyBase*> m_vpEnemies;
	int m_iLastEnemySpawned = 0;
	int m_iEnemySpawnDelay = 1000;

	// Font
	Font* m_poHeaderFont;
	Font* m_poSubHeaderFont;
	Font* m_poBasicTextFont;
};

