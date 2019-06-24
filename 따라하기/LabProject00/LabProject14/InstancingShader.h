#pragma once
#include "Shader.h"

//인스턴스 정보(게임 객체의 월드 변환 행렬과 객체의 색상)를 위한 구조체이다. 
struct VS_VB_INSTANCE 
{
	XMFLOAT4X4 m_xmf4x4Transform; 
	XMFLOAT4 m_xmcColor;
};

class CInstancingShader : public CObjectsShader
{
protected:
	//인스턴스 정점 버퍼와 정점 버퍼 뷰이다. 
	ID3D12Resource *m_pd3dcbGameObjects = NULL;
	VS_VB_INSTANCE *m_pcbMappedGameObjects = NULL;
	//D3D12_VERTEX_BUFFER_VIEW m_d3dInstancingBufferView;

public:
	CInstancingShader();
	virtual ~CInstancingShader();
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob **ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob **ppd3dShaderBlob);

	virtual void CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	virtual void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, void *pContext);

	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera);
};


