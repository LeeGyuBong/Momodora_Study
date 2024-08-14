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

	D3DXFONT_DESCW	tFontInfo;
	D3DXFONT_DESCW	tBigFontInfo;
	D3DXFONT_DESCW	tMiddleFontInfo;
	
	D3DXFONT_DESCW	tFrontFontInfo;
	D3DXFONT_DESCW	tBackFontInfo;

	AddFontResourceEx(L"../Data/�Ҿ߹ٸ�9.ttf", FR_PRIVATE, 0);
	tFontInfo.Height = 23;	// ����
	tFontInfo.Width = 13;	// �ʺ�
	tFontInfo.Weight = FW_LIGHT; // �β�
	tFontInfo.CharSet = HANGEUL_CHARSET; // �ѱ�
	tFontInfo.Italic = FALSE; // ����
	lstrcpy(tFontInfo.FaceName, L"�Ҿ߹ٸ�9"); // �۾�ü

	tBigFontInfo.Height = 35;	// ����
	tBigFontInfo.Width = 20;	// �ʺ�
	tBigFontInfo.Weight = FW_HEAVY; // �β�
	tBigFontInfo.CharSet = HANGEUL_CHARSET; // �ѱ�
	tBigFontInfo.Italic = FALSE; // ����
	lstrcpy(tBigFontInfo.FaceName, L"�Ҿ߹ٸ�9"); // �۾�ü

	tMiddleFontInfo.Height = 30;	// ����
	tMiddleFontInfo.Width = 16;	// �ʺ�
	tMiddleFontInfo.Weight = FW_NORMAL; // �β�
	tMiddleFontInfo.CharSet = HANGEUL_CHARSET; // �ѱ�
	tMiddleFontInfo.Italic = FALSE; // ����
	lstrcpy(tMiddleFontInfo.FaceName, L"�Ҿ߹ٸ�9"); // �۾�ü

	tFrontFontInfo.Height = 40;	// ����
	tFrontFontInfo.Width = 20;	// �ʺ�
	tFrontFontInfo.Weight = FW_THIN; // �β�
	tFrontFontInfo.CharSet = HANGEUL_CHARSET; // �ѱ�
	tFrontFontInfo.Italic = FALSE; // ����
	lstrcpy(tFrontFontInfo.FaceName, L"�Ҿ߹ٸ�9"); // �۾�ü

	tBackFontInfo.Height = 40;	// ����
	tBackFontInfo.Width = 23;	// �ʺ�
	tBackFontInfo.Weight = FW_HEAVY; // �β�
	tBackFontInfo.CharSet = HANGEUL_CHARSET; // �ѱ�
	tBackFontInfo.Italic = FALSE; // ����
	lstrcpy(tBackFontInfo.FaceName, L"�Ҿ߹ٸ�9"); // �۾�ü

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"Font Create Failed!!");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tBigFontInfo, &m_pBigFont)))
	{
		ERR_MSG(L"Font Create Failed!!");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tMiddleFontInfo, &m_pMiddleFont)))
	{
		ERR_MSG(L"Font Create Failed!!");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFrontFontInfo, &m_pFrontFont)))
	{
		ERR_MSG(L"Font Create Failed!!");
		return E_FAIL;
	}
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tBackFontInfo, &m_pBackFont)))
	{
		ERR_MSG(L"Font Create Failed!!");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
	{
		ERR_MSG(L"CreateLine Failed!!!");
		return E_FAIL;
	}
	
	return S_OK;
}

void CDevice::Render_Begin()
{
	//D3DCLEAR TARGET - ���� ��� ǥ�գ� ������ �ĸ� �����̴�
	// D3DCLEAR ZBUFFER - ���� ����
	// D3DCLEAR STENCIL - ���ٽ� ����
	m_pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
		D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);

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
	if (m_pBackFont)
		m_pBackFont->Release();

	if (m_pFrontFont)
		m_pFrontFont->Release();

	if (m_pMiddleFont)
		m_pMiddleFont->Release();

	if (m_pBigFont)
		m_pBigFont->Release();

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
	d3dpp.BackBufferWidth = CLIENTWINCX;
	d3dpp.BackBufferHeight = CLIENTWINCY;
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
	d3dpp.Windowed = TRUE;
	// ���ٽ� ���۸� direct�� ���� ���� ����
	d3dpp.EnableAutoDepthStencil = TRUE;
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

//void CDevice::DrawRectangle(float left, float top, float right, float bottom, DWORD color)
//{
//	VERTEX vertex[4] = {
//		{ left, top, 0.f, 1.f, color }, // Left
//		{ right, top, 0.f, 1.f, color }, // Top
//		{ left, bottom, 0.f, 1.f, color }, // Bottom
//		{ right, bottom, 0.f, 1.f, color } // Right
//	};
//
//	m_pDevice->CreateVertexBuffer(4 * sizeof(VERTEX), 0, D3DFVF_CUSTOM, D3DPOOL_DEFAULT, &m_pVB, NULL);
//
//	VOID* pVoid;
//	m_pVB->Lock(0, sizeof(vertex), (void**)&pVoid, 0);
//	memcpy(pVoid, vertex, sizeof(vertex));
//	m_pVB->Unlock();
//
//	//m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//
//	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX));
//	m_pDevice->SetFVF(D3DFVF_CUSTOM);
//
//	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//	//m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//}

void CDevice::DoubleFontText(const wstring str, D3DXVECTOR3 Pos, DWORD backColor, DWORD frontColor)
{
	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, Pos.x, Pos.y, 0.f);

	m_pSprite->SetTransform(&matTrans);
	m_pBackFont->DrawTextW(CDevice::GetInstance()->GetSprite(),
		str.c_str(), lstrlen(str.c_str()), nullptr, 0, backColor);

	D3DXMatrixTranslation(&matTrans, Pos.x + 4.f, Pos.y, 0.f);
	m_pSprite->SetTransform(&matTrans);
	m_pBackFont->DrawTextW(CDevice::GetInstance()->GetSprite(),
		str.c_str(), lstrlen(str.c_str()), nullptr, 0, frontColor);
}