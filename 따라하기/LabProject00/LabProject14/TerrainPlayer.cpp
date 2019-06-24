#include "stdafx.h"
#include "TerrainPlayer.h"
#include "HeightMapTerrain.h"
#include "Shader.h"



CTerrainPlayer::CTerrainPlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,
	ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext, int nMeshes) : CPlayer(nMeshes)
{
	m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA, 0.0f);
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;
	/*�÷��̾��� ��ġ�� ������ ���(y-�� ��ǥ�� ������ ���̺��� 1500 ����)�� �����Ѵ�.
	�÷��̾� ��ġ ������ y��ǥ�� ������ ���̺��� ũ�� �߷��� �ۿ��ϵ��� �÷��̾ �����Ͽ����Ƿ� �÷��̾�� ���������� �ϰ��ϰ� �ȴ�.*/

	float fHeight = pTerrain->GetHeight(pTerrain->GetWidth()*0.5f, pTerrain->GetLength()*0.5f);
	SetPosition(XMFLOAT3(pTerrain->GetWidth()*0.5f, fHeight + 1500.0f, pTerrain->GetLength()*0.5f));

	//�÷��̾��� ��ġ�� ����� �� ������ ������ ���� �÷��̾��� ��ġ�� ������ �� �ֵ��� �����Ѵ�. 
	SetPlayerUpdatedContext(pTerrain);

	//ī�޶��� ��ġ�� ����� �� ������ ������ ���� ī�޶��� ��ġ�� ������ �� �ֵ��� �����Ѵ�. 
	SetCameraUpdatedContext(pTerrain);

	CCubeMeshDiffused *pCubeMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList, 4.0f, 12.0f, 4.0f);
	SetMesh(0, pCubeMesh);

	//�÷��̾ �������� ���̴��� �����Ѵ�. 
	CPlayerShader* pShader = new CPlayerShader();
	pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	SetShader(pShader);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

CTerrainPlayer::~CTerrainPlayer()
{

}


CCamera *CTerrainPlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed) {
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;
	if (nCurrentCameraMode == nNewCameraMode)
		return(m_pCamera);

	switch (nNewCameraMode) {
	case FIRST_PERSON_CAMERA:
		SetFriction(250.0f);
		//1��Ī ī�޶��� �� �÷��̾ y-�� �������� �߷��� �ۿ��Ѵ�. 
		SetGravity(XMFLOAT3(0.0f, -250.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 50000.0f, ASPECT_RATIO, 60.0f);
		break;
	case SPACESHIP_CAMERA:
		SetFriction(125.0f);
		//�����̽� �� ī�޶��� �� �÷��̾ �߷��� �ۿ����� �ʴ´�. 
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(SPACESHIP_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 0.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 50000.0f, ASPECT_RATIO, 60.0f);
		break;
	case THIRD_PERSON_CAMERA:
		SetFriction(250.0f);
		//3��Ī ī�޶��� �� �÷��̾ y-�� �������� �߷��� �ۿ��Ѵ�. 
		SetGravity(XMFLOAT3(0.0f, -250.0f, 0.0f));
		SetMaxVelocityXZ(300.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, -50.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 50000.0f, ASPECT_RATIO, 60.0f);
		break;
	default:
		break;
	} 

	Update(fTimeElapsed);

	return(m_pCamera);
}


void CTerrainPlayer::OnPlayerUpdateCallback(float fTimeElapsed) 
{
	XMFLOAT3 xmf3PlayerPosition = GetPosition(); 
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pPlayerUpdatedContext; 

	/*�������� �÷��̾��� ���� ��ġ (x, z)�� ���� ����(y)�� ���Ѵ�. 
	�׸��� �÷��̾� �޽��� ���̰� 12�̰� �÷��̾��� �߽��� ������ü�� ����̹Ƿ� 
	y ���� �޽��� ������ ������ ���ϸ� �÷��̾��� ��ġ�� �ȴ�.*/ 
	float fHeight = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z) + 6.0f; 

	/*�÷��̾��� ��ġ ������ y-���� �����̸�(���� ���, �߷��� ����Ǵ� ���) �÷��̾��� ��ġ ������ y-���� ���� �۾����� �ȴ�. 
	�̶� �÷��̾��� ���� ��ġ ������ y ���� ������ ����(������ ������ ���� + 6)���� ������ �÷��̾� �� �Ϻΰ� ���� �Ʒ��� �ְ� �ȴ�. 
	�̷��� ��츦 �����Ϸ��� �÷��̾��� �ӵ� ������ y ���� 0���� ����� 
	�÷��̾� �� ��ġ ������ y-���� ������ ����(������ ������ ���� + 6)�� �����Ѵ�.
	�׷��� �÷��̾�� �׻� ���� ���� �ְ� �� ��.*/ 
	if (xmf3PlayerPosition.y < fHeight) { 
		XMFLOAT3 xmf3PlayerVelocity = GetVelocity(); 
		xmf3PlayerVelocity.y = 0.0f; 
		SetVelocity(xmf3PlayerVelocity); 

		xmf3PlayerPosition.y = fHeight; 
		SetPosition(xmf3PlayerPosition); 
	} 
}


void CTerrainPlayer::OnCameraUpdateCallback(float fTimeElapsed) 
{
	XMFLOAT3 xmf3CameraPosition = m_pCamera->GetPosition();
	/*���� �ʿ��� ī�޶��� ���� ��ġ (x, z)�� ���� ������ ����(y ��)�� ���Ѵ�. 
	�� ���� ī�޶��� ��ġ ������ y-�� �� �� ũ�� ī�޶� ������ �Ʒ��� �ְ� �ȴ�.
	�̷��� �Ǹ� ���� �׸��� ���ʰ� ���� ������ �׷����� �ʴ� ��찡 �߻� �Ѵ�
	(ī�޶� ���� �ȿ� �����Ƿ� �ﰢ���� ���ε� ������ �ٲ��). 
	�̷��� ��찡 �߻����� �ʵ��� ī�޶��� ��ġ �� ���� y-���� �ּҰ��� (������ ���� + 5)�� �����Ѵ�.
	ī�޶��� ��ġ ������ y-���� �ּҰ��� ������ ��� ��ġ���� ī�޶� ���� �Ʒ��� ��ġ���� �ʵ��� �����ؾ� �Ѵ�.*/

	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pCameraUpdatedContext; 
	float fHeight = pTerrain->GetHeight(xmf3CameraPosition.x, xmf3CameraPosition.z) + 5.0f; 

	if (xmf3CameraPosition.y <= fHeight) {
		xmf3CameraPosition.y = fHeight; 
		m_pCamera->SetPosition(xmf3CameraPosition); 
		if (m_pCamera->GetMode() == THIRD_PERSON_CAMERA) { 
			//3��Ī ī�޶��� ��� ī�޶� ��ġ(y-��ǥ)�� ����Ǿ����Ƿ� ī�޶� �÷��̾ �ٶ󺸵��� �Ѵ�. 
			CThirdPersonCamera *p3rdPersonCamera = (CThirdPersonCamera *)m_pCamera; 
			p3rdPersonCamera->SetLookAt(GetPosition()); 
		}
	} 
}
