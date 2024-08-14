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
	// Direct ��ü ����
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
		return E_FAIL;


	// ��ġ ����
	D3DCAPS9 DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		ERR_MSG(L"DeviceCaps Get Failed!!!");
		return E_FAIL;
	}

	// ���ؽ� ���μ��� ����
	DWORD vp;
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Direct ��ü ���� �Է�
	D3DPRESENT_PARAMETERS d3d_Parameters;
	ZeroMemory(&d3d_Parameters, sizeof(D3DPRESENT_PARAMETERS));
	SetParameters(d3d_Parameters);

	// �������̽� ����
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
	lstrcpy(tFontInfo.FaceName, L"���� ���");

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"Font Create Failed!!");
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::Render_Begin()
{
	//D3DCLEAR TARGET - ���� ��� ǥ�գ� ������ �ĸ� �����̴�
	// D3DCLEAR ZBUFFER - ���� ����
	// D3DCLEAR STENCIL - ���ٽ� ����
	m_pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(255, 200, 200, 200), 1.f, 0);

	m_pDevice->BeginScene();

	// D3DXSPRITE_ALPHABLEND: ���� �׽�Ʈ�� ��ȿ�� ���¿��� ���� ������ ����Ѵ�.
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
	// �ĸ� ���� ũ��
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	// �ĸ� ���� �ȼ� ���� // 32��Ʈ �ȼ� ����
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	// �ĸ� ���� ����
	d3dpp.BackBufferCount = 1;
	// ��Ƽ���ø� // ���� ��� X
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	// ��Ƽ���ø� ����
	d3dpp.MultiSampleQuality = 0;
	// ����ü�� ��� ����
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// ��ġ�� ������ ������ �ڵ�
	d3dpp.hDeviceWindow = g_hWnd;
	// â���. //true : â��� false : ��üȭ��
	d3dpp.Windowed = true;
	// ���ٽ� ���۸� direct�� ���� ���� ����
	d3dpp.EnableAutoDepthStencil = true;
	// ���ٽ� ���� ���� // 24��Ʈ ���� ���ۿ� ���ٽ� ���۷� 8��Ʈ ����
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	// �ΰ����� Ư�� // 0 : ����
	d3dpp.Flags = 0;
	// ��üȭ��� ����� ����. // D3DPRESENT_RATE_DEFAULT : ����Ʈ ����� ����
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	// ����� �ֻ����� FPS ����
	// D3DPRESENT_INTERVAL_IMMEDIATE : ���� ���α׷� FPS�� ���� ��� ����
	// D3DPRESENT_INTERVAL_DEFAULT : ������ ������ DirectX�� �˾Ƽ� ����. ���� ������� ������.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}