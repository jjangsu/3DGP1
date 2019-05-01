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
	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 110.0f;
	CWallMesh *pWallCubeMesh = new CWallMesh(fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 30);
	pWallCubeMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth * 0.3f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));


	// float fWidth = 10.0f, fHeight = 1.0f, fDepth = 4.0f;
	pRailCubeMesh = new CRailMesh(4.0f, 4.0f, 4.0f);

	for (int i = 0; i < nRail; ++i) {
		m_pRailObject.emplace_back(nullptr);
		m_pRailObject[i] = new CRailObject;
		m_pRailObject[i]->SetPosition(0.0f, 0.0f, 0.0f + RailSpeed * i);
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

	XMFLOAT3 newPosition(0, 0, 4.f);
	newPosition = Vector3::Add(m_pRailObject[nRail - 1]->GetPosition(), newPosition);

	XMFLOAT4X4 mRotation = Matrix4x4::Identity();
	// mRotation = Matrix4x4::Multiply(rotationAngle);

	if (pRailCubeMesh && accumulateTime >= timeToMakeRail) {
		m_pRailObject.emplace_back(nullptr);
		m_pRailObject[nRail] = new CRailObject;

		// �ٷ� ���� �׸� ������ ����� �����ͼ� ���� �׸� ���� ��Ŀ� ���� 
		m_pRailObject[nRail]->m_xmf4x4World = m_pRailObject[nRail - 1]->m_xmf4x4World;
		// Ű �Է��� �޾����� ȸ�� (��� ��� ȸ���� ��)
		m_pRailObject[nRail]->Rotate(rotationAngle.x, rotationAngle.y, rotationAngle.z);
		// �ʱ�ȭ ���ֱ� 
		rotationAngle = XMFLOAT3(0.f, 0.f, 0.f);
		//rotationMatrix = Matrix4x4::Identity();
		// m_pRailObject[nRail]->Rotate(rotationAngle.x, rotationAngle.y, rotationAngle.z);

		m_pRailObject[nRail]->Move(m_pRailObject[nRail]->GetLook(), RailSpeed);

		m_pRailObject[nRail]->SetMesh(pRailCubeMesh);
		m_pRailObject[nRail]->SetColor(RGB(255, 216, 0));
		if (nRail < 30)
			nRail++;
		accumulateTime -= timeToMakeRail;
	}

	for (const auto& r : m_pRailObject)
		r->Animate(fElapsedTime);

	if (m_pRailObject.size() > 30)
		m_pRailObject.pop_front();


	float Speed = RailSpeed * (1 / timeToMakeRail); // 1�ʿ� 5���� ���� 

	// ���� ������ �ϸ� �� �ڿ������� �ȴٰ� �մϴ�.

	// 2. �ٷ� �� ������ look, up, right�� �� player����
	m_pPlayer->m_xmf3Look = m_pRailObject[nRail - 4]->GetLook();
	m_pPlayer->m_xmf3Up = m_pRailObject[nRail - 4]->GetUp();
	m_pPlayer->m_xmf3Right = m_pRailObject[nRail - 4]->GetRight();


	m_pPlayer->Move(DIR_FORWARD, Speed * fElapsedTime);


	for (const auto& bullet : m_dBulletObject) {
		bullet->Animate(fElapsedTime);
		bullet->Move(bullet->GetLook(), bullet->m_fBulletSpeed * bullet->m_fElapsedTimes);
		if (m_dBulletObject.size() > 0 && m_dBulletObject[0]->GetPosition().z > m_pPlayer->m_xmf3Position.z + 100.0f) {
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
	float angle = 5.f;

	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			rotationAngle = Vector3::Add(rotationAngle, { 0.f, -angle, 0.f });
			//rotationMatrix = Matrix4x4::Multiply(rotationMatrix, Matrix4x4::RotationYawPitchRoll(0.f, -10.f, 0.f));
			break;
		case VK_RIGHT:
			rotationAngle = Vector3::Add(rotationAngle, { 0.f, angle, 0.f });
			//rotationMatrix = Matrix4x4::Multiply(rotationMatrix, Matrix4x4::RotationYawPitchRoll(0.f, 10.f, 0.f));
			break;
		case VK_UP:
			rotationAngle = Vector3::Add(rotationAngle, { angle, 0.f, 0.f });
			//rotationMatrix = Matrix4x4::Multiply(rotationMatrix, Matrix4x4::RotationYawPitchRoll(10.f, 0.f, 0.f));
			break;
		case VK_DOWN:
			rotationAngle = Vector3::Add(rotationAngle, { -angle, 0.f, 0.f });
			break;
		case VK_SPACE:
			m_dBulletObject.emplace_back(nullptr);
			m_dBulletObject[BulletNum] = new CBulletObject();
			m_dBulletObject[BulletNum]->SetMesh(pBulletMesh);
			m_dBulletObject[BulletNum]->SetPosition(m_pPlayer->m_xmf3Position.x, m_pPlayer->m_xmf3Position.y, m_pPlayer->m_xmf3Position.z);
			m_dBulletObject[BulletNum]->SetColor(RGB(255, 0, 0));
			// m_dBulletObject[BulletNum]->SetMovingDirection(m_pPlayer->m_xmf3MovingDirection);
			// m_dBulletObject[BulletNum]->m_xmf3BulletVectors = XMFLOAT3(m_pPlayer->m_xmf3Look);
			BulletNum += 1;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
