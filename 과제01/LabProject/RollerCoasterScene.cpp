#include "stdafx.h"
#include "RollerCoasterScene.h"
#include <iostream>
#include <iterator>

CRollerCoasterScene::CRollerCoasterScene()
{
}

CRollerCoasterScene::~CRollerCoasterScene()
{
}

void CRollerCoasterScene::BuildObjects()
{
	pRailCubeMesh = new CRailMesh(4.0f, 4.0f, 4.0f);

	for (int i = 0; i < nRail; ++i) {
		m_pRailObject.emplace_back(nullptr);
		m_pRailObject[i] = new CRailObject;
		m_pRailObject[i]->SetPosition(0.0f, 0.0f, 0.0f + RailInterval * i);
		m_pRailObject[i]->SetMesh(pRailCubeMesh);
		m_pRailObject[i]->SetColor(RGB(255, 216, 0));
	}

	rotationMatrix = Matrix4x4::Identity();

	pBulletMesh = new CCubeMesh(0.5f, 0.5f, 0.5f);

}

void CRollerCoasterScene::ReleaseObjects()
{
	CScene::ReleaseObjects();

	// if (m_pWallsObject) delete m_pWallsObject;
	if (m_pRailObject[0]) delete m_pRailObject[0];
}


void CRollerCoasterScene::Animate(float fElapsedTime)
{
	accumulateTime += fElapsedTime;

	if (pRailCubeMesh && accumulateTime >= timeToMakeRail) {
		m_pRailObject.emplace_back(nullptr);
		m_pRailObject[nRail] = new CRailObject;

		// �ٷ� ���� �׸� ������ ����� �����ͼ� ���� �׸� ���� ��Ŀ� ���� 
		m_pRailObject[nRail]->m_xmf4x4World = m_pRailObject[nRail - 1]->m_xmf4x4World;
		// Ű �Է��� �޾����� ȸ�� (��� ��� ȸ���� ��)
		m_pRailObject[nRail]->Rotate(rotationAngle.x, rotationAngle.y, rotationAngle.z);
		// �ʱ�ȭ ���ֱ� 
		rotationAngle = XMFLOAT3(0.f, 0.f, 0.f);

		m_pRailObject[nRail]->Move(m_pRailObject[nRail]->GetLook(), RailInterval);

		m_pRailObject[nRail]->SetMesh(pRailCubeMesh);
		m_pRailObject[nRail]->SetColor(RGB(255, 216, 0));
		if (nRail < nMaxRail)
			nRail++;
		accumulateTime -= timeToMakeRail;
	}

	// for (const auto& r : m_pRailObject)
	// 	r->Animate(fElapsedTime);

	if (m_pRailObject.size() > nMaxRail) {
		delete m_pRailObject[0];
		m_pRailObject.pop_front();
	}

	float Speed = RailInterval * (1 / timeToMakeRail); // 1�ʿ� 10���� ���� 

	// �ٷ� �� ������ look, up, right�� �� player����
	m_pPlayer->m_xmf3Look = m_pRailObject[nRail - OnPlayerFrontRailInterval]->GetLook();
	m_pPlayer->m_xmf3Up = m_pRailObject[nRail - OnPlayerFrontRailInterval]->GetUp();
	m_pPlayer->m_xmf3Right = m_pRailObject[nRail - OnPlayerFrontRailInterval]->GetRight();

	m_pPlayer->Move(DIR_FORWARD, Speed * fElapsedTime);


	for (const auto& bullet : m_dBulletObject) {
		bullet->Move(bullet->m_xmf3MovingDirection, bullet->m_fBulletSpeed * bullet->m_fElapsedTimes);
		if (m_dBulletObject.size() > 0 && 
			m_dBulletObject[0]->GetPosition().z > m_pPlayer->m_xmf3Position.z + fBulletMaxdistance) {
			delete m_dBulletObject[0];
			m_dBulletObject.pop_front();
			BulletNum--;
		}
	}

	CScene::Animate(fElapsedTime);
}

void CRollerCoasterScene::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	for (int i = 0; i < m_pRailObject.size(); ++i)
		m_pRailObject[i]->Render(hDCFrameBuffer, pCamera);

	for (auto& bullet : m_dBulletObject)
		bullet->Render(hDCFrameBuffer, pCamera);

	CScene::Render(hDCFrameBuffer, pCamera);
}

void CRollerCoasterScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	static UCHAR pKeyBuffer[256];
	float angle = 2.5f;

	if (GetKeyboardState(pKeyBuffer)) {
		if (pKeyBuffer[VK_UP] & 0xF0) rotationAngle = Vector3::Add(rotationAngle, { angle, 0.f, 0.f });
		if (pKeyBuffer[VK_DOWN] & 0xF0) rotationAngle = Vector3::Add(rotationAngle, { -angle, 0.f, 0.f });
		if (pKeyBuffer[VK_LEFT] & 0xF0) rotationAngle = Vector3::Add(rotationAngle, { 0.f, -angle, 0.f });
		if (pKeyBuffer[VK_RIGHT] & 0xF0) rotationAngle = Vector3::Add(rotationAngle, { 0.f, angle, 0.f });
		// if (pKeyBuffer[VK_F1] & 0xF0) rotationAngle = Vector3::Add(rotationAngle, { 0.f, 0.f, angle });
		// if (pKeyBuffer[VK_F2] & 0xF0) rotationAngle = Vector3::Add(rotationAngle, { 0.f, 0.f, -angle });
		if (pKeyBuffer[VK_SPACE] & 0xF0) {
			m_dBulletObject.emplace_back(nullptr);
			m_dBulletObject[BulletNum] = new CBulletObject();
			m_dBulletObject[BulletNum]->SetMesh(pBulletMesh);
			m_dBulletObject[BulletNum]->SetPosition(m_pPlayer->m_xmf3Position.x, 
													m_pPlayer->m_xmf3Position.y, 
													m_pPlayer->m_xmf3Position.z);
			m_dBulletObject[BulletNum]->SetColor(RGB(255, 0, 0));
			m_dBulletObject[BulletNum]->SetMovingDirection(m_pPlayer->m_xmf3Look);
			BulletNum += 1;
		}
	}	
}

// �÷��̾� ȸ�� ���� ��� �Ѿ˵� �׹��� ������ ��ġ�� 
// m_dBulletObject[BulletNum]->m_xmf3BulletVectors = m_pPlayer->m_xmf3Look;
// m_dBulletObject[BulletNum]->SetMovingDirection(m_pPlayer->m_xmf3MovingDirection);
// m_dBulletObject[BulletNum]->m_xmf3BulletVectors = XMFLOAT3(m_pPlayer->m_xmf3Look);

//m_dBulletObject[BulletNum]->m_xmf4x4World = m_pPlayer->m_xmf4x4World