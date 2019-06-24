#include "stdafx.h"
#include "TerrainPlayer.h"
#include "HeightMapTerrain.h"
#include "Shader.h"



CTerrainPlayer::CTerrainPlayer(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList,
	ID3D12RootSignature *pd3dGraphicsRootSignature, void *pContext, int nMeshes) : CPlayer(nMeshes)
{
	m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA, 0.0f);
	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)pContext;
	/*플레이어의 위치를 지형의 가운데(y-축 좌표는 지형의 높이보다 1500 높게)로 설정한다.
	플레이어 위치 벡터의 y좌표가 지형의 높이보다 크고 중력이 작용하도록 플레이어를 설정하였으므로 플레이어는 점차적으로 하강하게 된다.*/

	float fHeight = pTerrain->GetHeight(pTerrain->GetWidth()*0.5f, pTerrain->GetLength()*0.5f);
	SetPosition(XMFLOAT3(pTerrain->GetWidth()*0.5f, fHeight + 1500.0f, pTerrain->GetLength()*0.5f));

	//플레이어의 위치가 변경될 때 지형의 정보에 따라 플레이어의 위치를 변경할 수 있도록 설정한다. 
	SetPlayerUpdatedContext(pTerrain);

	//카메라의 위치가 변경될 때 지형의 정보에 따라 카메라의 위치를 변경할 수 있도록 설정한다. 
	SetCameraUpdatedContext(pTerrain);

	CCubeMeshDiffused *pCubeMesh = new CCubeMeshDiffused(pd3dDevice, pd3dCommandList, 4.0f, 12.0f, 4.0f);
	SetMesh(0, pCubeMesh);

	//플레이어를 렌더링할 셰이더를 생성한다. 
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
		//1인칭 카메라일 때 플레이어에 y-축 방향으로 중력이 작용한다. 
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
		//스페이스 쉽 카메라일 때 플레이어에 중력이 작용하지 않는다. 
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
		//3인칭 카메라일 때 플레이어에 y-축 방향으로 중력이 작용한다. 
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

	/*지형에서 플레이어의 현재 위치 (x, z)의 지형 높이(y)를 구한다. 
	그리고 플레이어 메쉬의 높이가 12이고 플레이어의 중심이 직육면체의 가운데이므로 
	y 값에 메쉬의 높이의 절반을 더하면 플레이어의 위치가 된다.*/ 
	float fHeight = pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z) + 6.0f; 

	/*플레이어의 위치 벡터의 y-값이 음수이면(예를 들어, 중력이 적용되는 경우) 플레이어의 위치 벡터의 y-값이 점점 작아지게 된다. 
	이때 플레이어의 현재 위치 벡터의 y 값이 지형의 높이(실제로 지형의 높이 + 6)보다 작으면 플레이어 의 일부가 지형 아래에 있게 된다. 
	이러한 경우를 방지하려면 플레이어의 속도 벡터의 y 값을 0으로 만들고 
	플레이어 의 위치 벡터의 y-값을 지형의 높이(실제로 지형의 높이 + 6)로 설정한다.
	그러면 플레이어는 항상 지형 위에 있게 된 다.*/ 
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
	/*높이 맵에서 카메라의 현재 위치 (x, z)에 대한 지형의 높이(y 값)를 구한다. 
	이 값이 카메라의 위치 벡터의 y-값 보 다 크면 카메라가 지형의 아래에 있게 된다.
	이렇게 되면 다음 그림의 왼쪽과 같이 지형이 그려지지 않는 경우가 발생 한다
	(카메라가 지형 안에 있으므로 삼각형의 와인딩 순서가 바뀐다). 
	이러한 경우가 발생하지 않도록 카메라의 위치 벡 터의 y-값의 최소값은 (지형의 높이 + 5)로 설정한다.
	카메라의 위치 벡터의 y-값의 최소값은 지형의 모든 위치에서 카메라가 지형 아래에 위치하지 않도록 설정해야 한다.*/

	CHeightMapTerrain *pTerrain = (CHeightMapTerrain *)m_pCameraUpdatedContext; 
	float fHeight = pTerrain->GetHeight(xmf3CameraPosition.x, xmf3CameraPosition.z) + 5.0f; 

	if (xmf3CameraPosition.y <= fHeight) {
		xmf3CameraPosition.y = fHeight; 
		m_pCamera->SetPosition(xmf3CameraPosition); 
		if (m_pCamera->GetMode() == THIRD_PERSON_CAMERA) { 
			//3인칭 카메라의 경우 카메라 위치(y-좌표)가 변경되었으므로 카메라가 플레이어를 바라보도록 한다. 
			CThirdPersonCamera *p3rdPersonCamera = (CThirdPersonCamera *)m_pCamera; 
			p3rdPersonCamera->SetLookAt(GetPosition()); 
		}
	} 
}
