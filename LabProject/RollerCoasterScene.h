#pragma once
#include "Scene.h"
#include "WallsObject.h"
#include "RailObject.h"
#include <deque>

class CRollerCoasterScene : public CScene
{
	// CWallsObject				*m_pWallsObject = NULL;
	std::deque <CRailObject*>	 m_pRailObject;

public:
	CRollerCoasterScene();
	virtual ~CRollerCoasterScene();


	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);

	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

public:
	int nRail = 5;
	float timeToMakeRail = 0.15f;
	float RailSpeed = 4.0f;

	CRailMesh* pRailCubeMesh = nullptr;

	float accumulateTime = 0.0f;

	XMFLOAT3 rotationAngle{ 0.f, 0.f, 0.f };
	XMFLOAT4X4 rotationMatrix;
};

