#pragma once
#include "TileManager.h"
#include "JPGImage.h"

class Mxh03uDisplayableObject;

class Mxh03uTileManager :
	public TileManager
{
public:
	Mxh03uTileManager();
	~Mxh03uTileManager();

	// Size
	int GetMapWidth(){ return this->m_iMapWidth; }
	int GetMapHeight(){ return this->m_iMapHeight; }

	// Tile types
	const static int TILE_NORMAL	= 0;
	const static int TILE_WALL		= 1;
	const static int TILE_EXIT		= 2;
	const static int TILE_BLOOD		= 3;
	const static int TILE_BLOODTRAIL = 4;
	const static int TILE_MOSS		= 5;
	const static int TILE_ROCK		= 6;
	void SetupTileTypes();

	// Drawing
	void DrawTileAt(
		BaseEngine* pEngine,
		SDL_Surface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY) const;

private:
	Mxh03uDisplayableObject* m_poTileObject = NULL;
	ImageData m_oFloorTileImage;
	ImageData m_oWallTileImage;

	// Tile decorations
	ImageData m_oFloorBlood;
	ImageData m_oFloorBloodTrail;
	ImageData m_oFloorMoss;
	ImageData m_oFloorRock;
};

