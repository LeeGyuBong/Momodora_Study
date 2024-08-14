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

	//void DrawRectangle(float left, float top, float right, float bottom, DWORD color);
	void DoubleFontText(const wstring str, D3DXVECTOR3 Pos, DWORD backColor, DWORD frontColor);

private:
	void SetParameters(D3DPRESENT_PARAMETERS & d3dpp);

public:
	LPDIRECT3DDEVICE9 GetDevice() { return m_pDevice; }
	LPD3DXSPRITE GetSprite() { return m_pSprite; }
	LPD3DXLINE GetLine() { return m_pLine; }

	LPD3DXFONT GetFont() { return m_pFont; }
	LPD3DXFONT GetBigFont() { return m_pBigFont; }
	LPD3DXFONT GetMiddleFont() { return m_pMiddleFont; }

	LPD3DXFONT GetFrontFont() { return m_pFrontFont; }
	LPD3DXFONT GetBackFont() { return m_pBackFont; }
private:
	// IDirect3D9: ���� ��ġ�� �����ϱ� ���� ��ɵ��� �����Ǿ��ִ� ��ü������(LPDIRECT3D9).
	LPDIRECT3D9 m_pD3D;
	// IDirect3DDevice9: �׷��� ��ġ�� ���� �������� �����ϴ� ��ü������(LPDIRECT3DDEVICE9)
	LPDIRECT3DDEVICE9 m_pDevice;
	// LPD3DXSPRITE: DirectX���� 2D �̹����� �������ϰ� �����ϴ� Com��ü.
	LPD3DXSPRITE m_pSprite;

	LPD3DXLINE m_pLine;

	LPD3DXFONT m_pFont;
	LPD3DXFONT m_pMiddleFont;
	LPD3DXFONT m_pBigFont;

	LPD3DXFONT m_pFrontFont;
	LPD3DXFONT m_pBackFont;

	//LPDIRECT3DVERTEXBUFFER9 m_pVB;
};

