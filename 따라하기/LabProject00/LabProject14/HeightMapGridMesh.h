#pragma once
#include "Mesh.h"


class CHeightMapGridMesh : public CMesh
{
protected: 
	//������ ũ��(����: x-����, ����: z-����)�̴�. 
	int m_nWidth; int m_nLength; 
	/*������ ������(����: x-����, ����: z-����, ����: y-����) �����̴�. 
	���� ���� �޽��� �� ������ x-��ǥ, y-��ǥ, z-��ǥ�� ������ ������ x-��ǥ, y-��ǥ, z-��ǥ�� ���� ���� ���´�. 
	��, ���� ������ x-�� ������ ������ 1�� �ƴ� �� ������ ������ x-��ǥ�� �ȴ�. 
	�̷��� �ϸ� ���� ����(���� ����)�� ����ϴ��� ū ũ���� ����(����)�� ������ �� �ִ�.*/ 
	XMFLOAT3 m_xmf3Scale;

public:
	CHeightMapGridMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, 
		int xStart, int zStart, int nWidth, int nLength, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f),
		XMFLOAT4 xmf4Color = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), void *pContext = NULL); 
	virtual ~CHeightMapGridMesh();

	XMFLOAT3 GetScale() { return(m_xmf3Scale); } 
	int GetWidth() { return(m_nWidth); } 
	int GetLength() { return(m_nLength); }

	//������ ��ǥ�� (x, z)�� �� ����(����)�� ���̸� ��ȯ�ϴ� �Լ��̴�. 
	virtual float OnGetHeight(int x, int z, void *pContext); 
	//������ ��ǥ�� (x, z)�� �� ����(����)�� ������ ��ȯ�ϴ� �Լ��̴�.
	virtual XMFLOAT4 OnGetColor(int x, int z, void *pContext); 
};

