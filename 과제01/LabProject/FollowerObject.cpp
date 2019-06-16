#include "stdafx.h"
#include "FollowerObject.h"

CFollowerObject::CFollowerObject(CGameObject *pTarget)
{
	m_pTarget = pTarget;
}

CFollowerObject::~CFollowerObject()
{
}

void CFollowerObject::Animate(float fElapsedTime)
{
	XMFLOAT3 xmf3TargetPosition = m_pTarget->GetPosition();
	XMFLOAT3 xmf3Position = GetPosition();

	XMFLOAT3 xmf3Look = GetLook();
	XMFLOAT3 xmf3ToTarget = Vector3::Subtract(xmf3TargetPosition, xmf3Position, true);
	float fDotProduct = Vector3::DotProduct(xmf3Look, xmf3ToTarget);
	float fAngle = ::IsEqual(fDotProduct, 1.0f) ? 0.0f : ((fDotProduct > 0.0f) ? XMConvertToDegrees(acos(fDotProduct)) : 90.0f);
	XMFLOAT3 xmf3CrossProduct = Vector3::CrossProduct(xmf3Look, xmf3ToTarget);
	//	if (fAngle != 0.0f) Rotate(0.0f, fAngle * fElapsedTime * ((xmf3CrossProduct.y > 0.0f) ? 1.0f : -1.0f), 0.0f);
	Rotate(0.0f, fAngle * fElapsedTime * ((xmf3CrossProduct.y > 0.0f) ? 1.0f : -1.0f), 0.0f);

	if (Vector3::Distance(xmf3TargetPosition, xmf3Position) > DISTANCE_TO_TARGET_OBJECT)
	{
		XMFLOAT3 xmf3Look = GetLook();
		SetPosition(Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Look, m_fMovingSpeed * fElapsedTime, false)));
	}

	//	CGameObject::Animate(fElapsedTime);
}