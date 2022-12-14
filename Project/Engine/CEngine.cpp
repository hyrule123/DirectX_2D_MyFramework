#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"


CEngine::CEngine()
	: m_hWnd(nullptr)
	, m_ClearColor(0.5f, 0.5f, 0.5f, 1.f)
{
}

CEngine::~CEngine()
{

}

int CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	// 메인 윈도우 핸들
	m_hWnd = _hWnd;
	m_vResolution = Vec2((float)_iWidth, (float)_iHeight);

	// 해상도에 맞는 작업영역 크기 조정
	RECT rt = { 0, 0, (int)_iWidth, (int)_iHeight};
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hWnd, true);


	// Device 초기화
	if (FAILED(CDevice::GetInst()->init(m_hWnd, _iWidth, _iHeight)))
	{
		MessageBox(nullptr, L"Device 초기화 실패", L"에러", MB_OK);
		return E_FAIL;
	}


	// Manager 초기화
	CPathMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CResMgr::GetInst()->init();
	CLevelMgr::GetInst()->init();		
	CRenderMgr::GetInst()->init();

	return S_OK;
}

void CEngine::progress()
{
	tick();

	render();
}

void CEngine::tick()
{
	// Manager Tick
	CTimeMgr::GetInst()->tick();
	CKeyMgr::GetInst()->tick();	

	CLevelMgr::GetInst()->tick();
}

void CEngine::render()
{
	CTimeMgr::GetInst()->render();

	// 출력 타겟 설정
	CDevice::GetInst()->SetRenderTarget();

	// 렌더링 시작
	CDevice::GetInst()->ClearTarget(m_ClearColor);

	CRenderMgr::GetInst()->render();

	// 렌더 종료
	CDevice::GetInst()->Present();
}
