#include "stdafx.h"
#include "Mesh.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CPolygon::CPolygon(int nVertices)
{
	if (nVertices > 0)
	{
		m_nVertices = nVertices;
		m_pVertices = new CVertex[nVertices];
	}
}

CPolygon::~CPolygon()
{
	if (m_pVertices) delete[] m_pVertices;
}

void CPolygon::SetVertex(int nIndex, CVertex& vertex)
{
	if ((0 <= nIndex) && (nIndex < m_nVertices) && m_pVertices)
	{
		m_pVertices[nIndex] = vertex;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CMesh::CMesh(int nPolygons)
{
	if (nPolygons > 0)
	{
		m_nPolygons = nPolygons;
		m_ppPolygons = new CPolygon*[nPolygons];
		m_nReferences = 0;
	}
}

CMesh::~CMesh(void)
{
	if (m_ppPolygons)
	{
		for (int i = 0; i < m_nPolygons; i++) if (m_ppPolygons[i]) delete m_ppPolygons[i];
		delete[] m_ppPolygons;
	}
}

void CMesh::SetPolygon(int nIndex, CPolygon *pPolygon)
{
	if ((0 <= nIndex) && (nIndex < m_nPolygons) && m_ppPolygons)
	{
		m_ppPolygons[nIndex] = pPolygon;
	}
}

void CMesh::Render(HDC hDCFrameBuffer, XMFLOAT4X4& xmf4x4World, CCamera *pCamera)
{
	XMFLOAT4X4 xmf4x4Transform = Matrix4x4::Multiply(xmf4x4World, pCamera->m_xmf4x4ViewProject);
	for (int j = 0; j < m_nPolygons; j++)
	{
		int nVertices = m_ppPolygons[j]->m_nVertices;
		CVertex *pVertices = m_ppPolygons[j]->m_pVertices;
		XMFLOAT3 xmf3Previous(-1.0f, 0.0f, 0.0f);
		for (int i = 0; i <= nVertices; i++)
		{
			CVertex vertex = pVertices[i % nVertices];
			//World/View/Projection Transformation(Perspective Divide)
			XMFLOAT3 xmf3Current = Vector3::TransformCoord(vertex.m_xmf3Position, xmf4x4Transform);
			if ((xmf3Current.z >= 0.0f) && (xmf3Current.z <= 1.0f))
			{
				//Screen Transformation
				xmf3Current.x = +xmf3Current.x * (pCamera->m_Viewport.m_nWidth * 0.5f) + pCamera->m_Viewport.m_xStart + (pCamera->m_Viewport.m_nWidth * 0.5f);
				xmf3Current.y = -xmf3Current.y * (pCamera->m_Viewport.m_nHeight * 0.5f) + pCamera->m_Viewport.m_yStart + (pCamera->m_Viewport.m_nHeight * 0.5f);

				if (xmf3Previous.x >= 0.0f)
				{
					::MoveToEx(hDCFrameBuffer, (long)xmf3Previous.x, (long)xmf3Previous.y, NULL);
					::LineTo(hDCFrameBuffer, (long)xmf3Current.x, (long)xmf3Current.y);
				}
				xmf3Previous = xmf3Current;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CCubeMesh::CCubeMesh(float fWidth, float fHeight, float fDepth) : CMesh(6)
{
	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;

	CPolygon *pFrontFace = new CPolygon(4);
	pFrontFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	SetPolygon(0, pFrontFace);

	CPolygon *pTopFace = new CPolygon(4);
	pTopFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	pTopFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pTopFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pTopFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	SetPolygon(1, pTopFace);

	CPolygon *pBackFace = new CPolygon(4);
	pBackFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	SetPolygon(2, pBackFace);

	CPolygon *pBottomFace = new CPolygon(4);
	pBottomFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	pBottomFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pBottomFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBottomFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	SetPolygon(3, pBottomFace);

	CPolygon *pLeftFace = new CPolygon(4);
	pLeftFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	pLeftFace->SetVertex(1, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pLeftFace->SetVertex(2, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	pLeftFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	SetPolygon(4, pLeftFace);

	CPolygon *pRightFace = new CPolygon(4);
	pRightFace->SetVertex(0, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pRightFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pRightFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pRightFace->SetVertex(3, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	SetPolygon(5, pRightFace);
}

CCubeMesh::~CCubeMesh(void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CWallMesh::CWallMesh(float fWidth, float fHeight, float fDepth, int nSubRects) : CMesh((4 * nSubRects * nSubRects) + 2)
{
	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;
	float fCellWidth = fWidth * (1.0f / nSubRects);
	float fCellHeight = fHeight * (1.0f / nSubRects);
	float fCellDepth = fDepth * (1.0f / nSubRects);

	int k = 0;
	CPolygon *pLeftFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pLeftFace = new CPolygon(4);
			pLeftFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight + (i*fCellHeight), -fHalfDepth + (j*fCellDepth)));
			pLeftFace->SetVertex(1, CVertex(-fHalfWidth, -fHalfHeight + ((i + 1)*fCellHeight), -fHalfDepth + (j*fCellDepth)));
			pLeftFace->SetVertex(2, CVertex(-fHalfWidth, -fHalfHeight + ((i + 1)*fCellHeight), -fHalfDepth + ((j + 1)*fCellDepth)));
			pLeftFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight + (i*fCellHeight), -fHalfDepth + ((j + 1)*fCellDepth)));
			SetPolygon(k++, pLeftFace);
		}
	}

	CPolygon *pRightFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pRightFace = new CPolygon(4);
			pRightFace->SetVertex(0, CVertex(+fHalfWidth, -fHalfHeight + (i*fCellHeight), -fHalfDepth + (j*fCellDepth)));
			pRightFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight + ((i + 1)*fCellHeight), -fHalfDepth + (j*fCellDepth)));
			pRightFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight + ((i + 1)*fCellHeight), -fHalfDepth + ((j + 1)*fCellDepth)));
			pRightFace->SetVertex(3, CVertex(+fHalfWidth, -fHalfHeight + (i*fCellHeight), -fHalfDepth + ((j + 1)*fCellDepth)));
			SetPolygon(k++, pRightFace);
		}
	}

	CPolygon *pTopFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pTopFace = new CPolygon(4);
			pTopFace->SetVertex(0, CVertex(-fHalfWidth + (i*fCellWidth), +fHalfHeight, -fHalfDepth + (j*fCellDepth)));
			pTopFace->SetVertex(1, CVertex(-fHalfWidth + ((i + 1)*fCellWidth), +fHalfHeight, -fHalfDepth + (j*fCellDepth)));
			pTopFace->SetVertex(2, CVertex(-fHalfWidth + ((i + 1)*fCellWidth), +fHalfHeight, -fHalfDepth + ((j + 1)*fCellDepth)));
			pTopFace->SetVertex(3, CVertex(-fHalfWidth + (i*fCellWidth), +fHalfHeight, -fHalfDepth + ((j + 1)*fCellDepth)));
			SetPolygon(k++, pTopFace);
		}
	}

	CPolygon *pBottomFace;
	for (int i = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			pBottomFace = new CPolygon(4);
			pBottomFace->SetVertex(0, CVertex(-fHalfWidth + (i*fCellWidth), -fHalfHeight, -fHalfDepth + (j*fCellDepth)));
			pBottomFace->SetVertex(1, CVertex(-fHalfWidth + ((i + 1)*fCellWidth), -fHalfHeight, -fHalfDepth + (j*fCellDepth)));
			pBottomFace->SetVertex(2, CVertex(-fHalfWidth + ((i + 1)*fCellWidth), -fHalfHeight, -fHalfDepth + ((j + 1)*fCellDepth)));
			pBottomFace->SetVertex(3, CVertex(-fHalfWidth + (i*fCellWidth), -fHalfHeight, -fHalfDepth + ((j + 1)*fCellDepth)));
			SetPolygon(k++, pBottomFace);
		}
	}

	CPolygon *pFrontFace = new CPolygon(4);
	pFrontFace->SetVertex(0, CVertex(-fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(1, CVertex(+fHalfWidth, +fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(2, CVertex(+fHalfWidth, -fHalfHeight, -fHalfDepth));
	pFrontFace->SetVertex(3, CVertex(-fHalfWidth, -fHalfHeight, -fHalfDepth));
	SetPolygon(k++, pFrontFace);

	CPolygon *pBackFace = new CPolygon(4);
	pBackFace->SetVertex(0, CVertex(-fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(1, CVertex(+fHalfWidth, -fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(2, CVertex(+fHalfWidth, +fHalfHeight, +fHalfDepth));
	pBackFace->SetVertex(3, CVertex(-fHalfWidth, +fHalfHeight, +fHalfDepth));
	SetPolygon(k++, pBackFace);
}

CWallMesh::~CWallMesh(void)
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//

CRailMesh::CRailMesh(float fWidth, float fHeight, float fDepth) : CMesh(18)
{
	int i = 0;
	
	float fLenWidth = fWidth * 1.0f;
	float fLenHeight = fHeight * 0.1f;
	float fLenDepth = fDepth * 0.2f;

	//---------------------------------------------------
	// ���
	//---------------------------------------------------
	CPolygon *pFrontFace = new CPolygon(4);
	pFrontFace->SetVertex(0, CVertex(-fLenWidth, +fLenHeight, -fLenDepth));
	pFrontFace->SetVertex(1, CVertex(+fLenWidth, +fLenHeight, -fLenDepth));
	pFrontFace->SetVertex(2, CVertex(+fLenWidth, -fLenHeight, -fLenDepth));
	pFrontFace->SetVertex(3, CVertex(-fLenWidth, -fLenHeight, -fLenDepth));
	SetPolygon(i++, pFrontFace);

	CPolygon *pTopFace = new CPolygon(4);
	pTopFace->SetVertex(0, CVertex(-fLenWidth, +fLenHeight, +fLenDepth));
	pTopFace->SetVertex(1, CVertex(+fLenWidth, +fLenHeight, +fLenDepth));
	pTopFace->SetVertex(2, CVertex(+fLenWidth, +fLenHeight, -fLenDepth));
	pTopFace->SetVertex(3, CVertex(-fLenWidth, +fLenHeight, -fLenDepth));
	SetPolygon(i++, pTopFace);

	CPolygon *pBackFace = new CPolygon(4);
	pBackFace->SetVertex(0, CVertex(-fLenWidth, -fLenHeight, +fLenDepth));
	pBackFace->SetVertex(1, CVertex(+fLenWidth, -fLenHeight, +fLenDepth));
	pBackFace->SetVertex(2, CVertex(+fLenWidth, +fLenHeight, +fLenDepth));
	pBackFace->SetVertex(3, CVertex(-fLenWidth, +fLenHeight, +fLenDepth));
	SetPolygon(i++, pBackFace);

	CPolygon *pBottomFace = new CPolygon(4);
	pBottomFace->SetVertex(0, CVertex(-fLenWidth, -fLenHeight, -fLenDepth));
	pBottomFace->SetVertex(1, CVertex(+fLenWidth, -fLenHeight, -fLenDepth));
	pBottomFace->SetVertex(2, CVertex(+fLenWidth, -fLenHeight, +fLenDepth));
	pBottomFace->SetVertex(3, CVertex(-fLenWidth, -fLenHeight, +fLenDepth));
	SetPolygon(i++, pBottomFace);

	CPolygon *pLeftFace = new CPolygon(4);
	pLeftFace->SetVertex(0, CVertex(-fLenWidth, +fLenHeight, +fLenDepth));
	pLeftFace->SetVertex(1, CVertex(-fLenWidth, +fLenHeight, -fLenDepth));
	pLeftFace->SetVertex(2, CVertex(-fLenWidth, -fLenHeight, -fLenDepth));
	pLeftFace->SetVertex(3, CVertex(-fLenWidth, -fLenHeight, +fLenDepth));
	SetPolygon(i++, pLeftFace);

	CPolygon *pRightFace = new CPolygon(4);
	pRightFace->SetVertex(0, CVertex(+fLenWidth, +fLenHeight, -fLenDepth));
	pRightFace->SetVertex(1, CVertex(+fLenWidth, +fLenHeight, +fLenDepth));
	pRightFace->SetVertex(2, CVertex(+fLenWidth, -fLenHeight, +fLenDepth));
	pRightFace->SetVertex(3, CVertex(+fLenWidth, -fLenHeight, -fLenDepth));
	SetPolygon(i++, pRightFace);

	//---------------------------------------------------
	// Left Side
	//---------------------------------------------------

	float fSideWidth = fWidth * 0.1f;
	float fSideHeight = fHeight * 0.1f;
	float fSideDepth = fDepth * 0.5f;

	float fSideWidthInterval = fWidth + fSideWidth / 2.f;

	pFrontFace = new CPolygon(4);
	pFrontFace->SetVertex(0, CVertex(-fSideWidthInterval - fSideWidth, +fSideHeight, -fSideDepth));
	pFrontFace->SetVertex(1, CVertex(-fSideWidthInterval + fSideWidth, +fSideHeight, -fSideDepth));
	pFrontFace->SetVertex(2, CVertex(-fSideWidthInterval + fSideWidth, -fSideHeight, -fSideDepth));
	pFrontFace->SetVertex(3, CVertex(-fSideWidthInterval - fSideWidth, -fSideHeight, -fSideDepth));
	SetPolygon(i++, pFrontFace);

	pTopFace = new CPolygon(4);
	pTopFace->SetVertex(0, CVertex(-fSideWidthInterval - fSideWidth, +fSideHeight, +fSideDepth));
	pTopFace->SetVertex(1, CVertex(-fSideWidthInterval + fSideWidth, +fSideHeight, +fSideDepth));
	pTopFace->SetVertex(2, CVertex(-fSideWidthInterval + fSideWidth, +fSideHeight, -fSideDepth));
	pTopFace->SetVertex(3, CVertex(-fSideWidthInterval - fSideWidth, +fSideHeight, -fSideDepth));
	SetPolygon(i++, pTopFace);

	pBackFace = new CPolygon(4);
	pBackFace->SetVertex(0, CVertex(-fSideWidthInterval - fSideWidth, -fSideHeight, +fSideDepth));
	pBackFace->SetVertex(1, CVertex(-fSideWidthInterval + fSideWidth, -fSideHeight, +fSideDepth));
	pBackFace->SetVertex(2, CVertex(-fSideWidthInterval + fSideWidth, +fSideHeight, +fSideDepth));
	pBackFace->SetVertex(3, CVertex(-fSideWidthInterval - fSideWidth, +fSideHeight, +fSideDepth));
	SetPolygon(i++, pBackFace);

	pBottomFace = new CPolygon(4);
	pBottomFace->SetVertex(0, CVertex(-fSideWidthInterval - fSideWidth, -fSideHeight, -fSideDepth));
	pBottomFace->SetVertex(1, CVertex(-fSideWidthInterval + fSideWidth, -fSideHeight, -fSideDepth));
	pBottomFace->SetVertex(2, CVertex(-fSideWidthInterval + fSideWidth, -fSideHeight, +fSideDepth));
	pBottomFace->SetVertex(3, CVertex(-fSideWidthInterval - fSideWidth, -fSideHeight, +fSideDepth));
	SetPolygon(i++, pBottomFace);

	pLeftFace = new CPolygon(4);
	pLeftFace->SetVertex(0, CVertex(-fSideWidthInterval - fSideWidth, +fSideHeight, +fSideDepth));
	pLeftFace->SetVertex(1, CVertex(-fSideWidthInterval - fSideWidth, +fSideHeight, -fSideDepth));
	pLeftFace->SetVertex(2, CVertex(-fSideWidthInterval - fSideWidth, -fSideHeight, -fSideDepth));
	pLeftFace->SetVertex(3, CVertex(-fSideWidthInterval - fSideWidth, -fSideHeight, +fSideDepth));
	SetPolygon(i++, pLeftFace);

	pRightFace = new CPolygon(4);
	pRightFace->SetVertex(0, CVertex(-fSideWidthInterval + fSideWidth, +fSideHeight, -fSideDepth));
	pRightFace->SetVertex(1, CVertex(-fSideWidthInterval + fSideWidth, +fSideHeight, +fSideDepth));
	pRightFace->SetVertex(2, CVertex(-fSideWidthInterval + fSideWidth, -fSideHeight, +fSideDepth));
	pRightFace->SetVertex(3, CVertex(-fSideWidthInterval + fSideWidth, -fSideHeight, -fSideDepth));
	SetPolygon(i++, pRightFace);

	//---------------------------------------------------
	// Right Side
	//---------------------------------------------------
	pFrontFace = new CPolygon(4);
	pFrontFace->SetVertex(0, CVertex(fSideWidthInterval - fSideWidth, +fSideHeight, -fSideDepth));
	pFrontFace->SetVertex(1, CVertex(fSideWidthInterval + fSideWidth, +fSideHeight, -fSideDepth));
	pFrontFace->SetVertex(2, CVertex(fSideWidthInterval + fSideWidth, -fSideHeight, -fSideDepth));
	pFrontFace->SetVertex(3, CVertex(fSideWidthInterval - fSideWidth, -fSideHeight, -fSideDepth));
	SetPolygon(i++, pFrontFace);

	pTopFace = new CPolygon(4);
	pTopFace->SetVertex(0, CVertex(fSideWidthInterval - fSideWidth, +fSideHeight, +fSideDepth));
	pTopFace->SetVertex(1, CVertex(fSideWidthInterval + fSideWidth, +fSideHeight, +fSideDepth));
	pTopFace->SetVertex(2, CVertex(fSideWidthInterval + fSideWidth, +fSideHeight, -fSideDepth));
	pTopFace->SetVertex(3, CVertex(fSideWidthInterval - fSideWidth, +fSideHeight, -fSideDepth));
	SetPolygon(i++, pTopFace);

	pBackFace = new CPolygon(4);
	pBackFace->SetVertex(0, CVertex(fSideWidthInterval - fSideWidth, -fSideHeight, +fSideDepth));
	pBackFace->SetVertex(1, CVertex(fSideWidthInterval + fSideWidth, -fSideHeight, +fSideDepth));
	pBackFace->SetVertex(2, CVertex(fSideWidthInterval + fSideWidth, +fSideHeight, +fSideDepth));
	pBackFace->SetVertex(3, CVertex(fSideWidthInterval - fSideWidth, +fSideHeight, +fSideDepth));
	SetPolygon(i++, pBackFace);

	pBottomFace = new CPolygon(4);
	pBottomFace->SetVertex(0, CVertex(fSideWidthInterval - fSideWidth, -fSideHeight, -fSideDepth));
	pBottomFace->SetVertex(1, CVertex(fSideWidthInterval + fSideWidth, -fSideHeight, -fSideDepth));
	pBottomFace->SetVertex(2, CVertex(fSideWidthInterval + fSideWidth, -fSideHeight, +fSideDepth));
	pBottomFace->SetVertex(3, CVertex(fSideWidthInterval - fSideWidth, -fSideHeight, +fSideDepth));
	SetPolygon(i++, pBottomFace);

	pLeftFace = new CPolygon(4);
	pLeftFace->SetVertex(0, CVertex(fSideWidthInterval - fSideWidth, +fSideHeight, +fSideDepth));
	pLeftFace->SetVertex(1, CVertex(fSideWidthInterval - fSideWidth, +fSideHeight, -fSideDepth));
	pLeftFace->SetVertex(2, CVertex(fSideWidthInterval - fSideWidth, -fSideHeight, -fSideDepth));
	pLeftFace->SetVertex(3, CVertex(fSideWidthInterval - fSideWidth, -fSideHeight, +fSideDepth));
	SetPolygon(i++, pLeftFace);

	pRightFace = new CPolygon(4);
	pRightFace->SetVertex(0, CVertex(fSideWidthInterval + fSideWidth, +fSideHeight, -fSideDepth));
	pRightFace->SetVertex(1, CVertex(fSideWidthInterval + fSideWidth, +fSideHeight, +fSideDepth));
	pRightFace->SetVertex(2, CVertex(fSideWidthInterval + fSideWidth, -fSideHeight, +fSideDepth));
	pRightFace->SetVertex(3, CVertex(fSideWidthInterval + fSideWidth, -fSideHeight, -fSideDepth));
	SetPolygon(i++, pRightFace);

}
CRailMesh::~CRailMesh()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAirplaneMesh::CAirplaneMesh(float fWidth, float fHeight, float fDepth) : CMesh(24)
{
	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;

	float x1 = fx * 0.2f, y1 = fy * 0.2f, x2 = fx * 0.1f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1)*x2 + (fy - y3);
	int i = 0;

	//Upper Plane
	CPolygon *pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	//Lower Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	//Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(2, CVertex(+x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x2, +y2, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(1, CVertex(+x2, +y2, +fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Right Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(+fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(+fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, -fz));
	pFace->SetVertex(2, CVertex(+x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(+x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(0.0f, 0.0f, +fz));
	SetPolygon(i++, pFace);

	//Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(0.0f, +(fy + y3), -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, +(fy + y3), +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-x2, +y2, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-x2, +y2, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x2, +y2, +fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, -fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	//Back/Left Plane
	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(0.0f, 0.0f, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(0.0f, 0.0f, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-x1, -y1, -fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-x1, -y1, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, +fz));
	SetPolygon(i++, pFace);

	pFace = new CPolygon(3);
	pFace->SetVertex(0, CVertex(-x1, -y1, -fz));
	pFace->SetVertex(1, CVertex(-fx, -y3, +fz));
	pFace->SetVertex(2, CVertex(-fx, -y3, -fz));
	SetPolygon(i++, pFace);
}

CAirplaneMesh::~CAirplaneMesh()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CTerrainMesh::CTerrainMesh(float fWidth, float fHeight, float fDepth, int nSubRects) : CMesh((nSubRects * nSubRects))
{
	float fHalfWidth = fWidth * 0.5f;
	float fHalfHeight = fHeight * 0.5f;
	float fHalfDepth = fDepth * 0.5f;
	float fCellWidth = fWidth * (1.0f / nSubRects);
	float fCellHeight = fHeight * (1.0f / nSubRects);
	float fCellDepth = fDepth * (1.0f / nSubRects);

	for (int i = 0, k = 0; i < nSubRects; i++)
	{
		for (int j = 0; j < nSubRects; j++)
		{
			CPolygon *pPolygon = new CPolygon(4);
			pPolygon->SetVertex(0, CVertex(-fHalfWidth + (i*fCellWidth), 0.0f, -fHalfDepth + (j*fCellDepth)));
			pPolygon->SetVertex(1, CVertex(-fHalfWidth + ((i + 1)*fCellWidth), 0.0f, -fHalfDepth + (j*fCellDepth)));
			pPolygon->SetVertex(2, CVertex(-fHalfWidth + ((i + 1)*fCellWidth), 0.0f, -fHalfDepth + ((j + 1)*fCellDepth)));
			pPolygon->SetVertex(3, CVertex(-fHalfWidth + (i*fCellWidth), 0.0f, -fHalfDepth + ((j + 1)*fCellDepth)));
			SetPolygon(k++, pPolygon);
		}
	}
}

CTerrainMesh::~CTerrainMesh(void)
{
}
