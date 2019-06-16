#pragma once
#include "CollisionScene.h"
#include "ExplosiveObject.h"

class CExplosionScene : public CCollisionScene
{
public:
	CExplosionScene();
	virtual ~CExplosionScene();

	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};