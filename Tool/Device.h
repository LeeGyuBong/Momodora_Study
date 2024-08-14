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
	// IDirect3D9: ���� ��ġ�� �����ϱ� ���� ��ɵ��� �����Ǿ��ִ� ��ü������(LPDIRECT3D9).
	LPDIRECT3D9 m_pD3D;
	// IDirect3DDevice9: �׷��� ��ġ�� ���� �������� �����ϴ� ��ü������(LPDIRECT3DDEVICE9)
	LPDIRECT3DDEVICE9 m_pDevice;
	// LPD3DXSPRITE: DirectX���� 2D �̹����� �������ϰ� �����ϴ� Com��ü.
	LPD3DXSPRITE m_pSprite;
	// LPD3DXLINE : ���� com ��ü
	LPD3DXLINE m_pLine;
	// LPD3DXFONT : ��Ʈ com ��ü
	LPD3DXFONT m_pFont;
};

