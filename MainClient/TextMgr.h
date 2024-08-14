#pragma once
class CTextMgr
{
	DECLARE_SINGLETON(CTextMgr)

private:
	CTextMgr();
	~CTextMgr();

public:
	HRESULT LoadText(const wstring& DataPath);
	void TalkText(const wstring& ObjKey, D3DXVECTOR3 Pos);

	void Release();

public:
	const bool& GetIsTalk() const { return m_bIsTalk; }
	const int& GetTalkIndex() const { return m_iTalkIndex; }

	void NextText() { ++m_iTalkIndex; }

	void StartTalk(int StartIndex, int EndIndex)
	{ 
		if (!m_bIsTalk)
		{
			m_bIsTalk = true;
			m_iTalkIndex = StartIndex;
			m_iEndIndex = EndIndex;
		}
	}
	void EndTalk() 
	{ 
		if (m_bIsTalk)
		{
			m_bIsTalk = false;
			m_iTalkIndex = 0;
			m_iEndIndex = 0;
			m_iPreIndex = 0;
			m_iTextLength = 0;
			m_fTextDrawTime = 0.f;
		}
	}

private:
	map<wstring, vector<wstring>> m_MapText;

	bool m_bIsTalk;

	int m_iTalkIndex;
	int m_iEndIndex;

	int m_iPreIndex;

	int m_iTextLength;
	float m_fTextDrawTime;
};

