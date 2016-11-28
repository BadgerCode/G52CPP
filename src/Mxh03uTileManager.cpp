#include "header.h"
#include "templates.h"

#include "Mxh03uTileManager.h"
#include "Mxh03uDisplayableObject.h"


Mxh03uTileManager::Mxh03uTileManager()
:TileManager(30,30)
{
	m_oFloorTileImage.LoadImage("floor.png");
	m_oWallTileImage.LoadImage("wall-tile.png");

	// Floor decorations
	m_oFloorBlood.LoadImage("floor_blood.png");
	m_oFloorBloodTrail.LoadImage("floor_bloodtrail.png");
	m_oFloorMoss.LoadImage("floor_moss.png");
	m_oFloorRock.LoadImage("floor_rock.png");
}


Mxh03uTileManager::~Mxh03uTileManager()
{
}

void Mxh03uTileManager::SetupTileTypes(){
	int leftCenterXTile = (m_iMapWidth - 1) / 2;
	int rightCenterXTile = (m_iMapWidth % 2 == 0) ? leftCenterXTile + 1 : leftCenterXTile;
	int topCenterYTile = (m_iMapHeight - 1) / 2;
	int bottomCenterYTile = (m_iMapHeight % 2 == 0) ? topCenterYTile + 1 : topCenterYTile;

	for (int i = 0; i < m_iMapWidth; i++){
		for (int j = 0; j < m_iMapHeight; j++){
			int tileValue = TILE_NORMAL;
			// left & right column
			if (i == 0 || i == m_iMapWidth - 1){
				if (j == topCenterYTile || j == bottomCenterYTile){
					tileValue = TILE_EXIT;
				}
				else {
					tileValue = TILE_WALL;
				}
			}
			// Top & bottom row
			else if (j == 0 || j == m_iMapHeight - 1){
				if (i == leftCenterXTile || i == rightCenterXTile){
					tileValue = TILE_EXIT;
				}
				else {
					tileValue = TILE_WALL;
				}
			}
			else{
				switch (rand() % 75){
				case 5:
					tileValue = TILE_ROCK;
					break;
				case 10:
				case 20:
					tileValue = TILE_MOSS;
					break;
				case 30:
					tileValue = TILE_BLOOD;
					break;
				case 40:
					tileValue = TILE_BLOODTRAIL;
					break;
				default:
					tileValue = TILE_NORMAL;
					break;
				}
			}

			SetValue(i, j, tileValue);
		}
	}
}


void Mxh03uTileManager::DrawTileAt(
	BaseEngine* pEngine,
	SDL_Surface* pSurface,
	int iMapX, int iMapY,
	int iStartPositionScreenX, int iStartPositionScreenY) const{
	int tileValue = GetValue(iMapX, iMapY);

	switch (tileValue){
	case TILE_WALL:
		m_oWallTileImage.RenderImage(pSurface, 0, 0, iStartPositionScreenX + 1, iStartPositionScreenY + 1, 30, 30);
		break;
	case TILE_EXIT:
		pEngine->DrawRectangle(iStartPositionScreenX, iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1, iStartPositionScreenY + GetTileHeight() - 1, 
			0x000000, pSurface);
		break;
	default:
		m_oFloorTileImage.RenderImage(pSurface, 0, 0, iStartPositionScreenX + 1, iStartPositionScreenY + 1, 30 ,30);
		switch (tileValue){
		case TILE_BLOOD:
			m_oFloorBlood.RenderImageWithMask(pSurface, 0, 0, iStartPositionScreenX + 1, iStartPositionScreenY + 1, 30, 30);
			break;
		case TILE_BLOODTRAIL:
			m_oFloorBloodTrail.RenderImageWithMask(pSurface, 0, 0, iStartPositionScreenX + 1, iStartPositionScreenY + 1, 30, 30);
			break;
		case TILE_MOSS:
			m_oFloorMoss.RenderImageWithMask(pSurface, 0, 0, iStartPositionScreenX + 1, iStartPositionScreenY + 1, 30, 30);
			break;
		case TILE_ROCK:
			m_oFloorRock.RenderImageWithMask(pSurface, 0, 0, iStartPositionScreenX + 1, iStartPositionScreenY + 1, 30, 30);
			break;
		}
		break;
	}
}