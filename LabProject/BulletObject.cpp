#include "stdafx.h"
#include "BulletObject.h"


CBulletObject::CBulletObject()
{
}


CBulletObject::~CBulletObject()
{
}

void CBulletObject::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	if (m_pMesh)
	{
		HPEN hPen = ::CreatePen(PS_SOLID, 2, m_dwColor);
		HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
		m_pMesh->Render(hDCFrameBuffer, m_xmf4x4World, pCamera);
		::SelectObject(hDCFrameBuffer, hOldPen);
		::DeleteObject(hPen);
	}
}

void CBulletObject::Animate(float fElapsedTime)
{
	XMFLOAT3 xmf3Position = GetPosition();

	m_xmf4x4Transforms = Matrix4x4::Identity();
	
	m_xmf4x4Transforms._41 = xmf3Position.x + m_xmf3BulletVectors.x * m_fElapsedTimes * m_fBulletSpeed;
	m_xmf4x4Transforms._42 = xmf3Position.y + m_xmf3BulletVectors.y * m_fElapsedTimes * m_fBulletSpeed;
	m_xmf4x4Transforms._43 = xmf3Position.z + m_xmf3BulletVectors.z * m_fElapsedTimes * m_fBulletSpeed;

	m_xmf4x4Transforms = Matrix4x4::Multiply(Matrix4x4::RotationAxis(m_xmf3BulletVectors, m_fBulletRotation * m_fElapsedTimes), m_xmf4x4Transforms);
}