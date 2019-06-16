#pragma once
#include "Scene.h"
#include "TerrainObject.h"
#include "FollowerObject.h"

class CRunAwayScene : public CScene
{
public:
	CRunAwayScene();
	virtual ~CRunAwayScene();

	CTerrainObject				*m_pTerrainObject = NULL;

	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
};
