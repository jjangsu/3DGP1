#include "stdafx.h"
#include "RunAwayScene.h"

CRunAwayScene::CRunAwayScene()
{
}

CRunAwayScene::~CRunAwayScene()
{
}

void CRunAwayScene::BuildObjects()
{
	float fHalfWidth = 100.0f, fHalfHeight = 0.0f, fHalfDepth = 100.0f;
	CTerrainMesh *pTerrainMesh = new CTerrainMesh(fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 100);
	pTerrainMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth * 0.3f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pTerrainObject = new CTerrainObject();
	m_pTerrainObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_pTerrainObject->SetMesh(pTerrainMesh);
	m_pTerrainObject->SetColor(RGB(0, 0, 0));

	CCubeMesh *pObjectCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);
	pObjectCubeMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_nObjects = 31;
	// m_ppObjects = new CGameObject*[m_nObjects];
	m_ppObjects.reserve(m_nObjects);

	for (int i = 0; i < m_nObjects; ++i)
		m_ppObjects.emplace_back(nullptr);

	for (int i = 0; i < m_nObjects; i++)
	{
		m_ppObjects[i] = new CFollowerObject(m_pPlayer);
		m_ppObjects[i]->SetMesh(pObjectCubeMesh);
		m_ppObjects[i]->SetColor(RGB(0, 0, 255));
		m_ppObjects[i]->SetMovingSpeed(2.0f);
	}
	m_ppObjects[0]->SetPosition(-31, 0.5f, 3);
	m_ppObjects[1]->SetPosition(20.6f, 0.5f, 38.6f);
	m_ppObjects[2]->SetPosition(-32.37f, 0.5f, -28.24f);
	m_ppObjects[3]->SetPosition(7.1, 0.5f, -27.8f);
	m_ppObjects[4]->SetPosition(-24.32, 0.5f, 15.09f);
	m_ppObjects[5]->SetPosition(23.66, 0.5f, 21.14f);
	m_ppObjects[6]->SetPosition(12.1, 0.5f, 2.6f);
	m_ppObjects[7]->SetPosition(15.63, 0.5f, 17.11f);
	m_ppObjects[8]->SetPosition(-0.7, 0.5f, 33.6f);
	m_ppObjects[9]->SetPosition(-11.7, 0.5f, 4.4f);
	m_ppObjects[10]->SetPosition(-32.4, 0.5f, 31.4f);
	m_ppObjects[11]->SetPosition(-20.31, 0.5f, -11.1f);
	m_ppObjects[12]->SetPosition(-13.59, 0.5f, 27.54f);
	m_ppObjects[13]->SetPosition(-20.76, 0.5f, -20.03f);
	m_ppObjects[14]->SetPosition(11, 0.5f, -6.9f);
	m_ppObjects[15]->SetPosition(25.6, 0.5f, 7.8f);
	m_ppObjects[16]->SetPosition(19.74, 0.5f, -30.1f);
	m_ppObjects[17]->SetPosition(21.1, 0.5f, -10.9f);
	m_ppObjects[18]->SetPosition(37.44, 0.5f, -18.267f);
	m_ppObjects[19]->SetPosition(28.73, 0.5f, 1.25f);
	m_ppObjects[20]->SetPosition(40.7, 0.5f, 7.1f);
	m_ppObjects[21]->SetPosition(-5.501, 0.5f, 16.603f);
	m_ppObjects[22]->SetPosition(30.9, 0.5f, 30.7f);
	m_ppObjects[23]->SetPosition(27.03, 0.5f, -27.01f);
	m_ppObjects[24]->SetPosition(-8.56, 0.5f, -33.5f);
	m_ppObjects[25]->SetPosition(14.1, 0.5f, 31.8f);
	m_ppObjects[26]->SetPosition(-15.79, 0.5f, -36.63f);
	m_ppObjects[27]->SetPosition(-7.35, 0.5f, -20.1f);
	m_ppObjects[28]->SetPosition(-35.6, 0.5f, -16);
	m_ppObjects[29]->SetPosition(-17.8, 0.5f, 37.6f);
	m_ppObjects[30]->SetPosition(-26.51, 0.5f, -39.65f);
}

void CRunAwayScene::ReleaseObjects()
{
	CScene::ReleaseObjects();

	if (m_pTerrainObject) delete m_pTerrainObject;
}

void CRunAwayScene::Animate(float fElapsedTime)
{
	CScene::Animate(fElapsedTime);

	CScene::CheckObjectByObjectCollisions();
}

void CRunAwayScene::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	m_pTerrainObject->Render(hDCFrameBuffer, pCamera);

	CScene::Render(hDCFrameBuffer, pCamera);
}