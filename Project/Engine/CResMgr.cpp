#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"

CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
}

void CResMgr::init()
{
	CreateResClassTypeIndex();

	CreateDefaultMesh();
	CreateDefaultGraphicsShader();
	CreateDefaultMaterial();

	LoadDefaultTexture();
}

void CResMgr::CreateResClassTypeIndex()
{
	m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMesh)), RES_TYPE::MESH));
	//m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMesh)), RES_TYPE::MESHDATA));
	m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CMaterial)), RES_TYPE::MATERIAL));
	m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CTexture)), RES_TYPE::TEXTURE));
	//m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CSound)), RES_TYPE::SOUND));
	//m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CPrefab)), RES_TYPE::PREFAB));
	m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CGraphicsShader)), RES_TYPE::GRAPHICS_SHADER));
	//m_mapResClassTypeIndex.insert(make_pair(std::type_index(typeid(CComputeShader)), RES_TYPE::COMPUTE_SHADER));
}

void CResMgr::CreateDefaultMesh()
{	
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;


	Ptr<CMesh> pMesh = nullptr;
	// =============
	// RectMesh ����
	// =============	
	// 0 --- 1 
	// |  \  |
	// 3 --- 2
	v.vPos = Vec3(-0.5f, 0.5f, 0.5f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"RectMesh", pMesh);
}

void CResMgr::CreateDefaultGraphicsShader()
{
	Ptr<CGraphicsShader> pShader = nullptr;

	// ===========
	// Test Shader
	// ===========
	pShader = new CGraphicsShader;
	pShader->SetKey(L"TestShader");
	pShader->CreateVertexShader(L"shader\\test.fx", "VS_Test");
	pShader->CreatePixelShader(L"shader\\test.fx", "PS_Test");

	AddRes(L"TestShader", pShader);
}

void CResMgr::CreateDefaultMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// Test Material
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TestShader"));
	AddRes(L"TestMtrl", pMtrl);
}

void CResMgr::LoadDefaultTexture()
{
	Ptr<CTexture> pTexture = Load<CTexture>(L"PlayerTex", L"texture\\Fighter.bmp");	

}