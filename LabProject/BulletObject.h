#pragma once
#include "GameObject.h"
class CBulletObject : public CGameObject
{
public:
	CBulletObject();
	~CBulletObject();

	XMFLOAT4X4					m_xmf4x4Transforms;
	XMFLOAT3					m_xmf3BulletVectors;
	float						m_fElapsedTimes = 0.2f;
	float						m_fBulletSpeed = 10.0f;

	XMFLOAT3					m_xmf3Color;

	void Render(HDC hDCFrameBuffer, CCamera *pCamera);
	void Animate(float fElapsedTime);
};

