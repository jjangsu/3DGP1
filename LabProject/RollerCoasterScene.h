#pragma once
#include "Scene.h"
#include "WallsObject.h"
#include "RailObject.h"
#include <deque>

class CRollerCoasterScene : public CScene
{
public:
	CRollerCoasterScene();
	virtual ~CRollerCoasterScene();

	CWallsObject				*m_pWallsObject = NULL;
	std::deque <CRailObject*>	 m_pRailObject;

	virtual void BuildObjects();
	virtual void ReleaseObjects();

	void CheckObjectByWallCollisions();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);

	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

public:
	int nRail = 5;
	float RailGap = 4.0f;

	CRailMesh* pRailCubeMesh = nullptr;

	float accumulateTime = 0.0f;
};

