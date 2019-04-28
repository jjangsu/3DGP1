#include "stdafx.h"
#include "RollerCoasterScene.h"

CRollerCoasterScene::CRollerCoasterScene()
{
}

CRollerCoasterScene::~CRollerCoasterScene()
{
}

void CRollerCoasterScene::BuildObjects()
{
	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 110.0f;
	CWallMesh *pWallCubeMesh = new CWallMesh(fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 30);
	pWallCubeMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth * 0.3f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pWallsObject = new CWallsObject();
	m_pWallsObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_pWallsObject->SetMesh(pWallCubeMesh);
	m_pWallsObject->SetColor(RGB(80, 80, 80));
	m_pWallsObject->m_pxmf4WallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[4] = XMFLOAT4(0.0f, 0.0f, +1.0f, fHalfDepth);
	m_pWallsObject->m_pxmf4WallPlanes[5] = XMFLOAT4(0.0f, 0.0f, -1.0f, fHalfDepth);

	// float fWidth = 10.0f, fHeight = 1.0f, fDepth = 4.0f;
	pRailCubeMesh = new CRailMesh(4.0f, 4.0f, 4.0f);

	for (int i = 0; i < nRail; ++i) {
		m_pRailObject.emplace_back(nullptr);
		m_pRailObject[i] = new CRailObject;
		m_pRailObject[i]->SetPosition(0.0f, 0.0f, 0.0f + RailGap * i);
		m_pRailObject[i]->SetMesh(pRailCubeMesh);
		m_pRailObject[i]->SetColor(RGB(255, 216, 0));
	}

}

void CRollerCoasterScene::ReleaseObjects()
{
	CScene::ReleaseObjects();

	if (m_pWallsObject) delete m_pWallsObject;
	if (m_pRailObject[0]) delete m_pRailObject[0];
}

void CRollerCoasterScene::CheckObjectByWallCollisions()
{
	for (int i = 0; i < m_nObjects; i++)
	{
		ContainmentType containType = m_pWallsObject->m_xmOOBB.Contains(m_ppObjects[i]->m_xmOOBB);
		switch (containType)
		{
		case DISJOINT:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
				if (intersectType == BACK)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
			}
			break;
		}
		case INTERSECTS:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
				if (intersectType == INTERSECTING)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
			}
			break;
		}
		case CONTAINS:
			break;
		}
	}
}

void CRollerCoasterScene::Animate(float fElapsedTime)
{
	m_pWallsObject->Animate(fElapsedTime);
	if (m_pWallsObject->m_xmOOBB.Contains(XMLoadFloat3(&m_pPlayer->m_xmf3Position)) == DISJOINT) m_pWallsObject->SetPosition(m_pPlayer->m_xmf3Position);

	// CheckObjectByWallCollisions();


	if (pRailCubeMesh && accumulateTime > 1.f) {
		m_pRailObject.emplace_back(nullptr);
		m_pRailObject[nRail] = new CRailObject;
		m_pRailObject[nRail]->SetPosition(0.0f, 0.0f, 0.0f + RailGap * nRail);
		m_pRailObject[nRail]->SetMesh(pRailCubeMesh);
		m_pRailObject[nRail]->SetColor(RGB(255, 216, 0));
		nRail++;
		accumulateTime = 0.0f;
	}

	for (const auto& r : m_pRailObject)
		r->Animate(fElapsedTime);

	CScene::Animate(fElapsedTime);
	
	accumulateTime += fElapsedTime;
}

void CRollerCoasterScene::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	m_pWallsObject->Render(hDCFrameBuffer, pCamera);
	for (int i = 0; i < m_pRailObject.size(); ++i)
		m_pRailObject[i]->Render(hDCFrameBuffer, pCamera);

	CScene::Render(hDCFrameBuffer, pCamera);
}

void CRollerCoasterScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:

			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
