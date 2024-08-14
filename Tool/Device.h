#pragma once
class CDevice
{
	DECLARE_SINGLETON(CDevice)

private:
	CDevice();
	~CDevice();

public:
	HRESULT InitDevice();
	void Render_Begin();
	void Render_End();
	void Release();

private:
	void SetParameters(D3DPRESENT_PARAMETERS & d3dpp);

public:
	LPDIRECT3DDEVICE9 GetDevice() { return m_pDevice; }
	LPD3DXSPRITE GetSprite() { return m_pSprite; }
	LPD3DXLINE GetLine() { return m_pLine; }
	LPD3DXFONT GetFont() { return m_pFont; }

private:
	// IDirect3D9: 여러 장치를 제어하기 위해 기능들이 구현되어있는 객체포인터(LPDIRECT3D9).
	LPDIRECT3D9 m_pD3D;
	// IDirect3DDevice9: 그래픽 장치를 통해 렌더링을 제어하는 객체포인터(LPDIRECT3DDEVICE9)
	LPDIRECT3DDEVICE9 m_pDevice;
	// LPD3DXSPRITE: DirectX에서 2D 이미지를 렌더링하고 제어하는 Com객체.
	LPD3DXSPRITE m_pSprite;
	// LPD3DXLINE : 라인 com 객체
	LPD3DXLINE m_pLine;
	// LPD3DXFONT : 폰트 com 객체
	LPD3DXFONT m_pFont;
};

