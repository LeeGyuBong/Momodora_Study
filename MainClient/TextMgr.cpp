#include "stdafx.h"
#include "TextMgr.h"

IMPLEMENT_SINGLETON(CTextMgr)

CTextMgr::CTextMgr()
	: m_bIsTalk(false), m_iTalkIndex(0), m_iTextLength(0), m_iPreIndex(0),
	m_fTextDrawTime(0.f)
{
}


CTextMgr::~CTextMgr()
{
	Release();
}

HRESULT CTextMgr::LoadText(const wstring& DataPath)
{
	wifstream fin;

	fin.open(DataPath);

	fin.imbue(std::locale("kor"));

	if (fin.fail())
	{
		ERR_MSG(L"TextPath Load Failed!!");
		return E_FAIL;
	}

	TCHAR szObjKey[MAX_STR] = L"";
	TCHAR szText[MAX_STR] = L"";

	while (true)
	{
		if (fin.eof())
			break;

		fin.getline(szObjKey, MAX_STR, '|');
		fin.getline(szText, MAX_STR);

		m_MapText[szObjKey].push_back(szText);
	}
	fin.close();

	return S_OK;
}

void CTextMgr::TalkText(const wstring& ObjKey, D3DXVECTOR3 Pos)
{
	if (!m_bIsTalk)
		return;

	if (m_iTalkIndex > m_iEndIndex)
	{
		m_bIsTalk = false;
		return;
	}

	auto iter_find = m_MapText.find(ObjKey);

	if (iter_find == m_MapText.end())
		return;

	if (iter_find->second.size() < size_t(m_iTalkIndex))
	{
		m_bIsTalk = false;
		return;
	}

	TCHAR CurText[MAX_STR] = L"";
	lstrcpy(CurText, iter_find->second[m_iTalkIndex].c_str());

	// 다음 Text로 넘어갈때 출력 문자 길이 0 초기화
	if (m_iPreIndex != m_iTalkIndex)
		m_iTextLength = 0;

	
	if(m_iTextLength != lstrlen(CurText))
		m_fTextDrawTime += CTimeMgr::GetInstance()->GetTime();
	// 일정시간마다 출력할 글자 갯수를 늘리면서 출력
	if (m_fTextDrawTime > 0.08f)
	{
		if (m_iTextLength == lstrlen(CurText))
			m_iTextLength = lstrlen(CurText);
		
		CSoundMgr::GetInstance()->PlaySound(L"sndDialogVoiceGirl3.wav", CSoundMgr::EFFECT);

		++m_iTextLength;
		m_fTextDrawTime = 0.f;
	}

	D3DXMATRIX matWorld, matTrans;
	D3DXMatrixIdentity(&matWorld);

	int LimitTextCX = 200;
	RECT MaxRect = {};
	// 한 텍스트 문장의 크기를 정함
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), CurText, -1, &MaxRect, DT_CALCRECT, D3DCOLOR_ARGB(255, 255, 255, 255));

	int LessRight = MaxRect.right - LimitTextCX;
	if (LessRight > 0)
	{
		MaxRect.bottom = MaxRect.bottom + (MaxRect.bottom * ((LessRight / LimitTextCX) + 2));
		MaxRect.right -= LessRight;
	}

	// 텍스트 박스 렌더링
	int Box_X = LimitTextCX / 32 + 1;
	int Box_Y = MaxRect.bottom / 32 + 1;

	if (Box_Y == 1)
		Box_Y += 1;
	
	int Index = 0;
	for (int y = 0; y < Box_Y; ++y)
	{
		for (int x = 0; x < Box_X; ++x)
		{
			if (y == 0 && x == 0) // 좌상단
				Index = 0;
			else if (y == 0 && x == (Box_X - 1)) // 우상단
				Index = 3;
			else if (y == (Box_Y - 1) && x == 0) // 좌하단
				Index = 1;
			else if (x == (Box_X - 1) && y == (Box_Y - 1)) // 우하단
				Index = 2;
			else if (x != 0 && x != (Box_X - 1) && y == 0) // 위
				Index = 6;
			else if (x == 0 && y != 0 && y != (Box_Y - 1)) // 좌
				Index = 5;
			else if (x == (Box_X - 1) && y != 0 && y != (Box_Y - 1)) // 우
				Index = 4;
			else if (x != 0 && x != (Box_X - 1) && y == (Box_Y - 1)) // 하단
				Index = 7;
			else
				Index = 8;

			const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"UI", L"Talk", Index);
			NULL_CHECK(pTexInfo);
			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

			D3DXVECTOR3 vPos = { Pos.x + (pTexInfo->tImgInfo.Width * x), Pos.y + (pTexInfo->tImgInfo.Width * y) + 5.f, 0.f };
			matWorld = CMath::CalculateMatrix(vPos);

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			/*if (x == (Box_X - 1) && y == (Box_Y - 1))
			{
				const TEXINFO* pTexInfo2 = CTextureMgr::GetInstance()->GetTexture(L"UI", L"Talk", 9);
				NULL_CHECK(pTexInfo);
				fCenterX = pTexInfo2->tImgInfo.Width * 0.5f;
				fCenterY = pTexInfo2->tImgInfo.Height * 0.5f;

				D3DXVECTOR3 vPos = { Pos.x + (pTexInfo->tImgInfo.Width * x), Pos.y + (pTexInfo->tImgInfo.Width * y) + 5.f, 0.f };
		
				matWorld = CMath::CalculateMatrix(vPos);

				CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo2->pTexture, nullptr,
					&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}*/
		}
	}

	// 텍스트 렌더링
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matTrans, Pos.x, Pos.y, Pos.z);
	matWorld = matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
		CurText, m_iTextLength, &MaxRect, DT_WORDBREAK, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_iPreIndex = m_iTalkIndex;
}

void CTextMgr::Release()
{
	for_each(m_MapText.begin(), m_MapText.end(),
		[](auto& MyPair)
	{
		MyPair.second.clear();
		MyPair.second.shrink_to_fit();
	});
	m_MapText.clear();
}
