#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include <vector>

class CRailObject : public CGameObject
{
public:
	CRailObject();
	virtual ~CRailObject();
public:
	XMFLOAT4 m_pxmfRailPlanes[6];


	void Render(HDC hDCFrameBuffer, CCamera *pCamera);
	void Animate(float fElapsedTime);
};

