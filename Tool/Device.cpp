#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	: m_pD3D(nullptr), m_pDevice(nullptr), m_pSprite(nullptr)
{
}


CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::InitDevice()
{
	// Direct 객체 생성
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
		return E_FAIL;


	// 장치 조사
	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		ERR_MSG(L"DeviceCaps Get Failed!!!");
		return E_FAIL;
	}

	// 버텍스 프로세싱 조사
	DWORD vp;
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Direct 객체 정보 입력
	D3DPRESENT_PARAMETERS d3d_Parameters;
	ZeroMemory(&d3d_Parameters, sizeof(D3DPRESENT_PARAMETERS));
	SetParameters(d3d_Parameters);

	// 인터페이스 생성
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3d_Parameters, &m_pDevice)))
	{
		ERR_MSG(L"CreateDevice Failed!!!");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		ERR_MSG(L"CreateSprite Failed!!!");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
	{
		ERR_MSG(L"CreateLine Failed!!!");
		return E_FAIL;
	}
	m_pLine->SetWidth(2);

	D3DXFONT_DESCW tFontInfo;
	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_LIGHT;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"맑은 고딕");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"Font Create Failed!!");
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::Render_Begin()
{
	//D3DCLEAR TARGET - 렌더 대상 표먼， 보통은 후면 버퍼이다
	// D3DCLEAR ZBUFFER - 갚이 버퍼
	// D3DCLEAR STENCIL - 스텐실 버퍼
	m_pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 200, 200, 200), 1.f, 0);

	m_pDevice->BeginScene();

	// D3DXSPRITE_ALPHABLEND: 알파 테스트가 유효한 상태에서 알파 블렌딩을 사용한다.
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDevice::Render_End()
{
	m_pSprite->End();

	m_pDevice->EndScene();

	m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

void CDevice::Release()
{
	if (m_pLine)
		m_pLine->Release();

	if (m_pFont)
		m_pFont->Release();
	
	if (m_pSprite)
		m_pSprite->Release();

	if (m_pDevice)
		m_pDevice->Release();

	if (m_pD3D)
		m_pD3D->Release();
}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS& d3dpp)
{
	// 후면 버퍼 크기
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	// 후면 버퍼 픽셀 포멧 // 32비트 픽셀 포멧
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	// 후면 버퍼 갯수
	d3dpp.BackBufferCount = 1;
	// 멀티샘플링 // 현재 사용 X
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	// 멀티샘플링 레벨
	d3dpp.MultiSampleQuality = 0;
	// 스왑체인 방식 설정
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// 장치에 연결할 윈도우 핸들
	d3dpp.hDeviceWindow = g_hWnd;
	// 창모드. //true : 창모드 false : 전체화면
	d3dpp.Windowed = true;
	// 스텐실 버퍼를 direct가 생성 관리 유무
	d3dpp.EnableAutoDepthStencil = true;
	// 스텐실 버퍼 포멧 // 24비트 깊이 버퍼와 스텐실 버퍼로 8비트 예약
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	// 부과적인 특성 // 0 : 없음
	d3dpp.Flags = 0;
	// 전체화면시 재생률 설정. // D3DPRESENT_RATE_DEFAULT : 디폴트 재생률 설정
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	// 모니터 주사율과 FPS 간격
	// D3DPRESENT_INTERVAL_IMMEDIATE : 현재 프로그램 FPS에 따라 즉시 시현
	// D3DPRESENT_INTERVAL_DEFAULT : 적절한 간격을 DirectX가 알아서 결정. 보통 재생률과 동일함.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}