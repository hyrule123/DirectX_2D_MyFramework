#include "pch.h"
#include "CCamera.h"

//카메라의 위치를 얻어오기 위함
#include "CTransform.h"

//화면 해상도 얻어오기 위함
#include "CDevice.h"

//레이어를 받아오기 위함
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

//레이어 정보 받아와서 분류용
#include "CRenderComponent.h"
#include "CMaterial.h"

//카메라 등록
#include "CRenderMgr.h"

CCamera::CCamera():
	CComponent(eCOMPONENT_TYPE::CAMERA)
	, m_AspectRatio()
	, m_ProjectionType(ePROJ_TYPE::ORTHOGRAPHY)
	, m_CamIndex(-1)
	, m_LayerFlag(UINT32_MAX)
{
	SetProjType(m_ProjectionType);
}

CCamera::~CCamera()
{
}


void CCamera::SetProjType(ePROJ_TYPE _Type)
{
	m_ProjectionType = _Type;

	const Vec2& Resolution = CDevice::GetInst()->GetRenderResolution();
	m_AspectRatio = Resolution.x / Resolution.y;

	switch (m_ProjectionType)
	{
	case ePROJ_TYPE::ORTHOGRAPHY:
		//===========
		//투영 -> 사실 이건 한번만 구해줘도 됨
		//===========

		//1. 투영 행렬 생성
		m_matProj = XMMatrixOrthographicLH(Resolution.x, Resolution.y, 1.f, 10000.f);
		break;
	case ePROJ_TYPE::PERSPECTIVE:
		//1-1. 원근 투영행렬
		m_matProj = XMMatrixPerspectiveFovLH(0.5f * XM_PI, m_AspectRatio, 1.f, 10000.f);
		break;
	}

	//2. 업데이트 - 카메라 별로 렌더링이 진행되므로 굳이 업데이트 할 필요가 없음.
	//g_transform.MatProj = m_matProj;
}

void CCamera::SetCamIndex(eCAMERA_INDEX _Idx)
{
	assert(_Idx <= eCAMIDX_END);

	m_CamIndex = _Idx;
	CRenderMgr::GetInst()->RegisterCamera(this, _Idx);
}

void CCamera::init()
{
	//기본 설정으로 투영행렬을 생성
	SetProjType(m_ProjectionType);
}

void CCamera::finaltick()
{
	Vec3 vCamPos = Transform()->GetWorldPos();

	//뷰행렬 = 카메라 앞으로 월드행렬의 물체들을 끌어오는 작업.
	//도로 끌어오는 작업이므로 월드행렬에 배치했던 순서의 역순으로 작업을 해주면 된다.
	//이동과 회전을 원래대로 되돌리는 행렬은 특정 행렬을 다시 원래 상태로 돌리는 행렬이라고 볼 수 있다.
	//(로컬 위치) * (이동행렬) * (회전행렬) = (월드 위치)
	//(월드 위치) * (이동과 회전을 되돌리는 행렬) = (로컬 위치)
	//이건 역행렬의 정의와 일치한다.
	// I * X = X
	// X * X^(-1) = I
	//그러므로 카메라의 위치와 회전만큼 뺴 주는 행렬은 카메라의 위치행렬과 회전행렬의 역행렬을 곱해주면 된다는 뜻이다.
	//(R * T)^(-1) = T^(-1) * R^(-1)
	//역행렬을 곱하면 순서가 반대로 됨. 주의할 것
	//또한 역행렬은 행렬마다 만들어지는 방식이 다르다. 어떤 행렬은 역행렬이 존재하지 않을 수도 있다.
	//역행렬의 존재 여부는 판별식(행렬곱 Det)으로 가능하다.
	//월드변환행렬의 경우는 역행렬이 다 존재하므로 사용할 필요는 없음.


	//1. 위치의 역행렬 -> 그냥 음수만큼 이동해주면됨
	//x축으로 a, y축으로 b, z축으로 c만큼 이동했다고 하면
	//x축으로 -a, y축으로 -b, z축으로 -c만큼 이동해주면 됨.
	/*
	1 0 0 0 
	0 1 0 0 
	0 0 1 0
	-a -b -c 1
	*/

	//자신도 화면에 나오게 하기 위해서 z축으로 약간 거리를 더 이동
	vCamPos.z -= 10.f;
	m_matView = Matrix::CreateTranslation(-vCamPos);


	//2. 회전
	//회전은 이동과는 역행렬의 모습은 다르지만 쉽게 구할수 있다.
	//이는 직교행렬의 성질 덕분이다.
	//직교행렬은 각 행(또는 열)끼리 수직인 행렬을 말한다.
	//이런 직교행렬은 전치행렬이 자신의 역행렬이 되는 특징을 가지고 있다.
	//회전행렬은 직교행렬이고, 역행렬은 돌렸던 걸 다시 원상복귀시키는 행렬이므로 
	//회전행렬을 직교하면 현재 회전각의 반대 방향으로 돌릴 수 있게 된다.
	//const Vec3& vecRot = Transform()->GetRelativeRot();
	//const XMVECTOR& vecQut = XMQuaternionRotationRollPitchYawFromVector(vecRot);
	//Matrix tempmat = Matrix::CreateFromQuaternion(vecQut);
	//m_matView *= tempmat.Transpose();

	const Matrix& matRot = Transform()->GetWorldRotMat();
	m_matView *= matRot.Transpose();

	//3. transform 상수버퍼 구조체에 업데이트 -> 안함. 나중에 render때 일괄적으로 view 행렬과 proj 행렬을 곱할 예정.
	//g_transform.matViewProj = m_matView;


	////===========
	////투영 -> 사실 이건 한번만 구해줘도 됨 -> init()으로 이동함
	////===========
	//
	////1. 투영 행렬 생성
	//const Vec2& Resolution = CDevice::GetInst()->GetRenderResolution();
	////m_matProj = XMMatrixOrthographicLH(Resolution.x, Resolution.y, 1.f, 10000.f);

	////1-1. 원근 투영행렬(체험용)
	//m_AspectRatio = Resolution.x / Resolution.y;
	//m_matProj = XMMatrixPerspectiveFovLH(0.5f * XM_PI, m_AspectRatio, 1.f, 10000.f);
	//
	////2. 업데이트
	//g_transform.MatProj = m_matProj;
	
}


void CCamera::SortObject()
{
	for (UINT32 i = 0; i < MAX_LAYER; ++i)
	{
		UINT32 mask = (UINT32)1 << i;

		if (false == (mask & m_LayerFlag))
			continue;

		//카메라가 출력하고자 하는 레이어의 오브젝트 리스트를 받아와서
		const list<CGameObject*>& objList = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(i)->GetObjList();

		//순회 돌아주면서
		for (auto& iter : objList)
		{
			//출력 담당 컴포넌트를 받아온다.
			CRenderComponent* Com = iter->GetRenderComponent();

			//컴포넌트가 없거나, 컴포넌트 내부의 출력용 클래스가 등록되어있지 않을 경우 continue
			if (
				nullptr == Com
				||
				false == Com->GetRenderReady()
				)
				continue;
			
			//쉐이더 도메인을 받아와서
			eSHADER_DOMAIN dom = Com->GetMaterial()->GetShader()->GetShaderDomain();

			//만약 쉐이더 도메인이 등록되어있지 않을 경우 assert 처리
			assert((int)dom < (int)eSHADER_DOMAIN_END);

			//그렇지 않을 경우 push back
			m_arrvecShaderDomain[(int)dom].push_back(&(*iter));
		}
	}
}

void CCamera::render()
{
	//이제 카메라별로 렌더링이 진행되므로, 카메라가 가지고 있는 View 행렬과 Proj 행렬을 미리 곱해 놓는다.
	g_transform.matViewProj = m_matView * m_matProj;


	for (int i = 0; i < eSHADER_DOMAIN_END; ++i)
	{
		size_t size = m_arrvecShaderDomain[i].size();
		for (size_t j = 0; j < size; ++j)
		{
			m_arrvecShaderDomain[i][j]->render();
		}

		//렌더링 순회 끝나고 나면 비워주기.
		m_arrvecShaderDomain[i].clear();
	}
}


