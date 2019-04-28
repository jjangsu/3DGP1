#pragma once
#include "GameObject.h"

class CFollowerObject : public CGameObject
{
public:
	CFollowerObject(CGameObject *pTarget);
	virtual ~CFollowerObject();

	CGameObject					*m_pTarget = NULL;
	XMFLOAT3					m_xmf3InitialPosition;

	virtual void Animate(float fElapsedTime);
};