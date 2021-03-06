#pragma once

#include "tilemanager.h"

class Demo4TileManager :
	public TileManager
{
public:
	Demo4TileManager(void);
	~Demo4TileManager(void);

	// Draw a tile at a location on the screen determined by the tile
	// position and the base X and Y co-ordinates
	// OVERRIDE THIS TO DRAW YOUR TILE!!!
	// Draw a tile
	virtual void DrawTileAt( 
		BaseEngine* pEngine, 
		SDL_Surface* pSurface, 
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY ) const;
};
