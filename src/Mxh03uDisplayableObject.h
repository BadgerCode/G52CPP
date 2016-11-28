#pragma once
#include "DisplayableObject.h"

class Mxh03uMain;

class Mxh03uDisplayableObject :
	public DisplayableObject
{
public:
	Mxh03uDisplayableObject(Mxh03uMain* pEngine);
	~Mxh03uDisplayableObject();
	// Displayable object overrides
	void Draw();

	// Extra functionality
	int GetWidth(){ return this->m_iDrawWidth; }
	int GetHeight(){ return this->m_iDrawHeight; }
	
protected:
	Mxh03uMain* m_pRealEngine;
};

