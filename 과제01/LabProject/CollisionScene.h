#pragma once
#include "Scene.h"
#include "WallsObject.h"


class CCollisionScene : public CScene
{
public:
	CCollisionScene();
	virtual ~CCollisionScene();

	CWallsObject				*m_pWallsObject = NULL;

	virtual void BuildObjects();
	virtual void ReleaseObjects();

	void CheckObjectByWallCollisions();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);
};
