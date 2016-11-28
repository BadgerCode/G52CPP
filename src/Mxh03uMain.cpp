// Must include these two first
#include "header.h"
#include "templates.h"

#include "Mxh03uMain.h"
#include "Mxh03uTileManager.h"
// Entity base classes
#include "Mxh03uDisplayableObject.h"
#include "Mxh03uLivingEntity.h"
#include "Mxh03uEnemyBase.h"
// Entities
#include "Mxh03uPlayer.h"
#include "Mxh03uEnemySkeleton.h"

#define MAX_DISPLAY_OBJECTS	50
#define GRID_SQUARE_SIZE	30
#define HUD_WIDTH			200
#define TEXT_PADDING		5
#define TEXT_INDENT			10
#define TEXT_COLOUR			0xFFFFFF
#define HEADER_COLOUR		0x000000


Mxh03uMain::Mxh03uMain()
	:BaseEngine(MAX_DISPLAY_OBJECTS)
{
	m_pcHighscoreFilePath = "highscore.txt";
}


Mxh03uMain::~Mxh03uMain()
{
}


/*
	INITIALISATION
*/


int Mxh03uMain::GameInit(void)
{
	// Setup our game portion of the screen
	SetGameScreen(0 + GRID_SQUARE_SIZE, 0 + GRID_SQUARE_SIZE,
		m_iScreenWidth - HUD_WIDTH - (GRID_SQUARE_SIZE * 2), m_iScreenHeight - (GRID_SQUARE_SIZE * 2));

	LoadHighScore();

	// Load fonts
	m_poHeaderFont = GetFont("Marav2.ttf", 50);
	m_poSubHeaderFont = GetFont("Cornerstone Regular.ttf", 21);
	m_poBasicTextFont = GetFont("Cornerstone Regular.ttf", 18);

	// Set up tiles
	m_oTileManager.SetSize((GetScreenWidth() - HUD_WIDTH) / GRID_SQUARE_SIZE, GetScreenHeight() / GRID_SQUARE_SIZE);
	m_oTileManager.SetupTileTypes();
	m_oTileManager.SetBaseTilesPositionOnScreen(0, 0);

	// Create the moving objects
	InitialiseObjects();
	// Set up the initial background
	SetupBackgroundBuffer();
	return 0; // success
}

void Mxh03uMain::GameAction()
{
	// If too early to act then do nothing
	if (!IsTimeToActWithSleep())
		return;

	// Don't act for another 15 ticks
	SetTimeToAct(15);


	switch (m_iState)
	{
	case STATE_MENU:
	case STATE_GAMEOVER:
		break;
	case STATE_GAME:
		// Check if we've survived a second
		if (GetModifiedTime() - m_iLastSecondSurvived >= 1000){
			m_iLastSecondSurvived = GetModifiedTime();
			m_iSecondsSurvived++;
		}
		
		if (GetModifiedTime() - m_iLastEnemySpawned >= m_iEnemySpawnDelay){
			m_iLastEnemySpawned = GetModifiedTime();
			AddEnemy();
		}

		UpdateAllObjects(GetModifiedTime());

		if (m_oPlayer->IsDead()){
			m_iState = STATE_GAMEOVER;
			CheckHighScore();
			Redraw(false);
		}
		break;
	}
}


int Mxh03uMain::InitialiseObjects()
{
	// We're making changes to the array of drawable objects
	DrawableObjectsChanged();

	DestroyOldObjects();

	int iObjectsCreated = 0;

	// Create player
	m_oPlayer = new Mxh03uPlayer(this);

	CreateObjectArray(MAX_DISPLAY_OBJECTS);

	StoreObjectInArray(0, m_oPlayer);	iObjectsCreated++;
	StoreObjectInArray(1, NULL);

	// Resize our enemies vector
	m_vpEnemies.resize(MAX_DISPLAY_OBJECTS - iObjectsCreated, NULL);
	return iObjectsCreated; // As per the comments in BaseEngine for InitialiseObjects
}


/*
	RENDERING
*/


void Mxh03uMain::SetupBackgroundBuffer()
{
	// We use the basic game background on our menu 
	//  and only need to reset it after game over
	FillBackground(0x000000);
	// HUD
	DrawBackgroundRectangle(GetScreenWidth() - HUD_WIDTH, 0, GetScreenWidth() , GetScreenHeight(), 0x964209);
	DrawBackgroundRectangle(GetScreenWidth() - HUD_WIDTH + TEXT_PADDING, 0 + TEXT_PADDING, GetScreenWidth() - TEXT_PADDING, GetScreenHeight() - TEXT_PADDING, 0x5a2501);

	// Tiles
	m_oTileManager.DrawAllTiles(this, this->GetBackground(), 0, 0, m_oTileManager.GetMapWidth() - 1, m_oTileManager.GetMapHeight() - 1);
}


void Mxh03uMain::DrawObjects()
{
	if (m_iState != STATE_MENU){
		BaseEngine::DrawObjects();
	}
}


void Mxh03uMain::UndrawObjects()
{
	if (m_iState != STATE_MENU){
		BaseEngine::UndrawObjects();
	}
}

void Mxh03uMain::DrawStringsOnTop()
{
	int screenX, screenY;
	char tempBuffer[50];
	int textHeight = m_poBasicTextFont->GetSize();
	int subHeaderHeight = m_poSubHeaderFont->GetSize();
	int headerHeight = m_poHeaderFont->GetSize();

	switch (m_iState){
	case STATE_MENU:
		// Add title
		screenX = (GetScreenWidth() - HUD_WIDTH) / 2;
		screenY = GetScreenHeight() / 2 - headerHeight;
		DrawScreenStringCentered(screenX, screenY, "THE PIT", HEADER_COLOUR, m_poHeaderFont);

		// Controls menu
		screenX = GetScreenWidth() - (HUD_WIDTH - 1) / 2;
		screenY = TEXT_PADDING * 2 + subHeaderHeight / 2;
		DrawScreenStringCentered(screenX, screenY, "Controls:", HEADER_COLOUR, m_poSubHeaderFont);

		screenX = GetScreenWidth() - HUD_WIDTH + TEXT_PADDING * 2;
		screenY += TEXT_PADDING + subHeaderHeight / 2;

		// Movement (wasd or arrows)
		DrawScreenString(screenX, screenY, "Movemement", TEXT_COLOUR, m_poBasicTextFont);
		screenY += textHeight;
		DrawScreenString(screenX + TEXT_INDENT, screenY, "W, A, S, D", HEADER_COLOUR, m_poBasicTextFont);
		screenY += textHeight;
		DrawScreenString(screenX + TEXT_INDENT, screenY, "or", HEADER_COLOUR, m_poBasicTextFont);
		screenY += textHeight;
		DrawScreenString(screenX + TEXT_INDENT, screenY, "Arrow Keys", HEADER_COLOUR, m_poBasicTextFont);
		screenY += textHeight + TEXT_PADDING;

		// Attack- spawn
		DrawScreenString(screenX, screenY, "Attack", TEXT_COLOUR, m_poBasicTextFont);
		screenY += textHeight;
		DrawScreenString(screenX + TEXT_INDENT, screenY, "SPACEBAR", HEADER_COLOUR, m_poBasicTextFont);
		screenY += textHeight + TEXT_PADDING;

		// Pause
		DrawScreenString(screenX, screenY, "Pause", TEXT_COLOUR, m_poBasicTextFont);
		screenY += textHeight;
		DrawScreenString(screenX + TEXT_INDENT, screenY, "ESCAPE", HEADER_COLOUR, m_poBasicTextFont);
		screenY += textHeight;


		// Press enter to start
		screenX = (GetScreenWidth() - HUD_WIDTH) / 2;
		screenY = GetScreenHeight() / 2;
		DrawScreenStringCentered(screenX, screenY, "Press ENTER,", HEADER_COLOUR, m_poSubHeaderFont);
		screenY += subHeaderHeight / 2 + TEXT_PADDING;
		DrawScreenStringCentered(screenX, screenY, "To begin your journey.", TEXT_COLOUR, m_poBasicTextFont);
		break;
	case STATE_GAME:
		// Clear HUD
		CopyBackgroundPixels(GetScreenWidth() - HUD_WIDTH, 0, HUD_WIDTH, GetScreenHeight());
		screenY = DrawScoreBoard() + textHeight + TEXT_PADDING * 4;

		// Health
		screenX = GetScreenWidth() - HUD_WIDTH + TEXT_PADDING;
		DrawScreenString(screenX, screenY, "Health:", TEXT_COLOUR, m_poBasicTextFont);
		screenY += TEXT_PADDING + textHeight;
		DrawScreenRectangle(screenX, screenY, 
			(int) screenX + (double) (HUD_WIDTH - TEXT_PADDING * 2) * ((double)m_oPlayer->GetHealth() / m_oPlayer->GetMaxHealth()),
			screenY + 20, 0xFF0000);

		// Add pause message
		screenX = GetScreenWidth() - (HUD_WIDTH - 1) / 2;
		screenY = GetScreenHeight() - TEXT_PADDING * 2 - textHeight - subHeaderHeight;
		DrawScreenStringCentered(screenX, screenY, "To pause, press", TEXT_COLOUR, m_poBasicTextFont);

		screenY += TEXT_PADDING + textHeight;
		DrawScreenStringCentered(screenX, screenY, "ESCAPE", HEADER_COLOUR, m_poSubHeaderFont);
		break;
	case STATE_PAUSED:
		// Press enter to resume
		screenX = (GetScreenWidth() - HUD_WIDTH) / 2;
		screenY = GetScreenHeight() / 2 - headerHeight;
		DrawScreenStringCentered(screenX, screenY, "PAUSED", 0xFF0000, m_poHeaderFont);
		screenY += headerHeight / 2 + TEXT_PADDING * 2;
		DrawScreenStringCentered(screenX, screenY, "Press ESC or ENTER to resume.", TEXT_COLOUR, m_poBasicTextFont);
		break;
	case STATE_GAMEOVER:
		// Clear HUD
		CopyBackgroundPixels(GetScreenWidth() - HUD_WIDTH, 0, HUD_WIDTH, GetScreenHeight());

		// Draw score board and update our position
		screenY = DrawScoreBoard() + textHeight + TEXT_PADDING * 4;

		// Check for high score
		if (m_iKills > m_iPreviousHighestKills || m_iSecondsSurvived > m_iPreviousLongestLife){
			DrawScreenStringCentered(GetScreenWidth() - HUD_WIDTH + TEXT_PADDING * 2, screenY, "NEW HIGH SCORE!", HEADER_COLOUR, m_poBasicTextFont);
		}

		// Press enter to resume
		screenX = (GetScreenWidth() - HUD_WIDTH) / 2;
		screenY = GetScreenHeight() / 2 - headerHeight;
		DrawScreenStringCentered(screenX, screenY, "GAMEOVER", 0xFF0000, m_poHeaderFont);
		screenY += headerHeight / 2 + TEXT_PADDING * 2;
		DrawScreenStringCentered(screenX, screenY, "Press ENTER to try again.", TEXT_COLOUR, m_poBasicTextFont);
		break;
	}
}





/*
	INPUT
*/


void Mxh03uMain::KeyDown(int iKeyCode)
{
	switch (iKeyCode){
	case SDLK_ESCAPE:
		if (m_iState == STATE_GAME){
			m_iPauseStarted = GetTime();
			m_iState = STATE_PAUSED;
			Redraw(false);
		}
		else if (m_iState == STATE_PAUSED){
			UnPauseGame();
		}
		break;
	case SDLK_RETURN:
		if (m_iState == STATE_MENU){
			m_iState = STATE_GAME;
			StartGame();
			Redraw(true);
		}
		else if (m_iState == STATE_PAUSED){
			UnPauseGame();
		}
		else if (m_iState == STATE_GAMEOVER){
			m_iState = STATE_MENU;
			ResetGame();
			Redraw(true);
		}
		break;
	}
}


/*
	ADDED FUNCTIONALITY
*/


void Mxh03uMain::StartGame()
{
	m_iLastSecondSurvived = GetModifiedTime();
	m_iLastEnemySpawned = GetModifiedTime();
	m_oPlayer->Spawn(300, 300);
}


void Mxh03uMain::ResetGame()
{
	// Update local copy of high score
	if (m_iKills > m_iPreviousHighestKills)m_iPreviousHighestKills = m_iKills;
	if (m_iSecondsSurvived > m_iPreviousLongestLife)m_iPreviousLongestLife = m_iSecondsSurvived;

	// Reset variables
	m_iSecondsSurvived = 0;
	m_iKills = 0;
	m_iLastSecondSurvived = 0;

	// Kill all monsters
	KillAllEnemies();
}


void Mxh03uMain::SetGameScreen(int iMinX, int iMinY, int iWidth, int iHeight)
{
	this->m_iMinGameScreenX = iMinX;
	this->m_iMinGameScreenY = iMinY;
	this->m_iMaxGameScreenX = iMinX + iWidth;
	this->m_iMaxGameScreenY = iMinY + iHeight;
}


void Mxh03uMain::CheckHighScore()
{
	int newKills = m_iPreviousHighestKills;
	int newSecondsSurvived = m_iPreviousLongestLife;
	bool saveFile = false;

	if (m_iKills > newKills){
		newKills = m_iKills;
		saveFile = true;
	}
	if (m_iSecondsSurvived > newSecondsSurvived){
		newSecondsSurvived = m_iSecondsSurvived;
		saveFile = true;
	}

	if (saveFile) SaveHighScore(newKills, newSecondsSurvived);
}


void Mxh03uMain::LoadHighScore()
{
	FILE* scoresFile = fopen(m_pcHighscoreFilePath, "rb");

	if (scoresFile){
		int previousKills;
		int previousSecondsSurvived;
		int previousBonusPoints;
		fread(&m_iPreviousHighestKills, sizeof(int), 1, scoresFile);
		fread(&m_iPreviousLongestLife, sizeof(int), 1, scoresFile);
		fclose(scoresFile);
	}
	else {
		m_iPreviousHighestKills = 0;
		m_iPreviousLongestLife = 0;
	}
}


void Mxh03uMain::SaveHighScore(int iKills, int iSeconds)
{
	FILE* scoresFile = fopen(m_pcHighscoreFilePath, "wb");
	char* test;
	fwrite(&iKills, sizeof(int), 1, scoresFile);
	fwrite(&iSeconds, sizeof(int), 1, scoresFile);
	fclose(scoresFile);
}


void Mxh03uMain::AddEnemy()
{
	Mxh03uEnemyBase* newEnemy = NULL;
	Mxh03uEnemyBase* oldEnemy = NULL;

	// Check our vector of enemies
	for (int i = 0; i < m_vpEnemies.capacity(); i++){
		if (m_vpEnemies[i] == NULL){
			// Try to add the enemy to the displayable object array before creating it
			for (int j = 0; j < MAX_DISPLAY_OBJECTS; j++){
				if (GetDisplayableObject(j) == NULL){
					Mxh03uEnemyBase* newEnemy = new Mxh03uEnemySkeleton(this);
					m_vpEnemies.at(i) = newEnemy;
					StoreObjectInArray(j, newEnemy);
					StoreObjectInArray(j + 1, NULL);
					break;
				}
			}
			DrawableObjectsChanged();
			m_vpEnemies[i]->RandomSpawn();
			break;
		}
		else if (m_vpEnemies[i]->IsDead()){
			m_vpEnemies[i]->RandomSpawn();
			break;
		}
	}
}


// Kills all the enemies so the instances can be reused
void Mxh03uMain::KillAllEnemies()
{
	if (!m_vpEnemies.empty()){
		for (std::vector<Mxh03uEnemyBase*>::const_iterator i = m_vpEnemies.begin();
			i != m_vpEnemies.end() && *i != NULL; i++){
			(*i)->Kill();
		}
	}
}


int Mxh03uMain::DrawScoreBoard()
{
	int textHeight = m_poBasicTextFont->GetSize();
	int subHeaderHeight = m_poSubHeaderFont->GetSize();
	int headerHeight = m_poHeaderFont->GetSize();
	int screenX = GetScreenWidth() - HUD_WIDTH + TEXT_PADDING * 2;
	int screenY = TEXT_PADDING * 2 + subHeaderHeight / 2;
	char tempBuffer[50];

	// Current score
	DrawScreenStringCentered(GetScreenWidth() - HUD_WIDTH / 2, screenY, "Score", HEADER_COLOUR, m_poSubHeaderFont);
	screenY += subHeaderHeight;

	// Current kills
	DrawScreenString(screenX, screenY, "Kills:", HEADER_COLOUR, m_poBasicTextFont);
	screenY += textHeight;
	sprintf(tempBuffer, "%d", m_iKills);
	DrawScreenString(screenX + TEXT_INDENT, screenY, tempBuffer, TEXT_COLOUR, m_poBasicTextFont);
	screenY += textHeight + TEXT_PADDING;

	// Current time
	DrawScreenString(screenX, screenY, "Time:", HEADER_COLOUR, m_poBasicTextFont);
	screenY += textHeight;
	sprintf(tempBuffer, "%d", m_iSecondsSurvived);
	DrawScreenString(screenX + TEXT_INDENT, screenY, tempBuffer, TEXT_COLOUR, m_poBasicTextFont);
	screenY += textHeight + TEXT_PADDING * 8;


	// Previous best
	DrawScreenStringCentered(GetScreenWidth() - HUD_WIDTH / 2, screenY, "Best Score", HEADER_COLOUR, m_poSubHeaderFont);
	screenY += subHeaderHeight;

	// Most kills
	DrawScreenString(screenX, screenY, "Kills:", HEADER_COLOUR, m_poBasicTextFont);
	screenY += textHeight;
	sprintf(tempBuffer, "%d", m_iPreviousHighestKills);
	DrawScreenString(screenX + TEXT_INDENT, screenY, tempBuffer, TEXT_COLOUR, m_poBasicTextFont);
	screenY += textHeight + TEXT_PADDING;

	// Longest time
	DrawScreenString(screenX, screenY, "Time:", HEADER_COLOUR, m_poBasicTextFont);
	screenY += textHeight;
	sprintf(tempBuffer, "%d", m_iPreviousLongestLife);
	DrawScreenString(screenX + TEXT_INDENT, screenY, tempBuffer, TEXT_COLOUR, m_poBasicTextFont);

	return screenY+ TEXT_PADDING * 4;
}



void Mxh03uMain::DrawStringCentered(int iX, int iY, const char* pText,
	unsigned int uiColour, Font* pFont, SDL_Surface* pTarget)
{
	if (pFont == NULL)
		pFont = m_poBasicTextFont;

	// We need a valid font to work out the text size
	if ((pFont != NULL) && (pFont->GetFont() != NULL))
	{
		SDL_Surface *sText = TTF_RenderText_Solid(pFont->GetFont(), pText, {0,0,0});

		// Center the text
		iX -= sText->w / 2;
		iY -= sText->h / 2;
	}

	// Draw with new position
	DrawString(iX, iY, pText, uiColour, pFont, pTarget);
}