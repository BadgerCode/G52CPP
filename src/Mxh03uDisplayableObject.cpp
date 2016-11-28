#include "header.h"
#include "templates.h"

#include "Mxh03uDisplayableObject.h"
#include "Mxh03uMain.h"

Mxh03uDisplayableObject::Mxh03uDisplayableObject(Mxh03uMain* pEngine)
:DisplayableObject(pEngine)
, m_pRealEngine(pEngine)
{
}


Mxh03uDisplayableObject::~Mxh03uDisplayableObject()
{
}


// Added a check if the object is visible
// The rest is copied from DisplayableObject
void Mxh03uDisplayableObject::Draw()
{
	if (!m_bVisible)return;

	// Draw the object
	for (int iX = m_iCurrentScreenX + m_iStartDrawPosX; iX < (m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth); iX++)
		for (int iY = m_iCurrentScreenY + m_iStartDrawPosY; iY < (m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight); iY++)
			m_pEngine->SafeSetScreenPixel(iX, iY, 0xff00ff);

	// Store the position at which the object was last drawn.
	StoreLastScreenPositionForUndraw();
}