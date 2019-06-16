#pragma once
#include "Scene.h"
#include "WallsObject.h"
#include "RailObject.h"
#include "BulletObject.h"
#include <deque>

class CRollerCoasterScene : public CScene
{
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
	int nRail = 10;
	int nMaxRail = 40;
	float timeToMakeRail = 0.10f;
	float RailInterval = 4.0f;	// 레일 간격
	int OnPlayerFrontRailInterval = nRail - 1;

	CRailMesh* pRailCubeMesh = nullptr;

	float accumulateTime = 0.0f;

	XMFLOAT3 rotationAngle{ 0.f, 0.f, 0.f };
	XMFLOAT4X4 rotationMatrix = Matrix4x4::Identity();

	CCubeMesh* pBulletMesh = NULL;
	std::deque<CBulletObject*> m_dBulletObject;
	int BulletNum = 0;
	float fBulletMaxdistance = 50.f;
};

