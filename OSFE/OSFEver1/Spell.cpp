#include "stdafx.h"
#include "Spell.h"
#include "BitmapMgr.h"
#include "MouseMgr.h"

CSpell::CSpell()
	:m_iPlayerTile(0), m_eSpellID(SPELL_END), m_pSpellName(nullptr)
{
	m_eObjID = SPELL;
	BITMAP->Insert_Bmp(L"../Image/UI/card_epic_blank.bmp", L"card_epic_blank");
	BITMAP->Insert_Bmp(L"../Image/UI/CursorArt.bmp", L"CursorArt");
	ZeroMemory(&m_tSpellInfo, sizeof(SPELLINFO));
	ZeroMemory(&m_tSoundInfo, sizeof(SOUNDINFO));
}


CSpell::~CSpell()
{
}

void CSpell::Render_SpellIcon(HDC hDC, int _left, int _top, int _CX, int _CY)
{
	HDC		hMemDC = BITMAP->Find_Img(m_pSpellName);
	GdiTransparentBlt(hDC,
		_left,
		_top,
		_CX, // Ű�� ������
		_CY,
		hMemDC,
		0,
		0,
		256,		// �ؽ�ó ���� ������
		256,
		RGB(1, 1, 1));
}

void CSpell::Render_SpellCard(HDC hDC, int _left, int _top)
{
	HDC		hCardDC = BITMAP->Find_Img(L"card_epic_blank");
	GdiTransparentBlt(hDC,
		_left,
		_top,
		120, // Ű�� ������
		180,
		hCardDC,
		0,
		0,
		550,		// �ؽ�ó ���� ������
		790,
		RGB(255, 255, 255));

	HDC		hMemDC = BITMAP->Find_Img(m_pSpellName);
	GdiTransparentBlt(hDC,
		_left + 35,
		_top + 35,
		50, // Ű�� ������
		50,
		hMemDC,
		0,
		0,
		256,		// �ؽ�ó ���� ������
		256,
		RGB(1, 1, 1));

	TCHAR buf[32] = {};
	//_tcscat_s(buf, L"\t");
	HFONT hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	SetTextAlign(hDC, TA_CENTER);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	_itow_s(m_tSpellInfo.iMana, buf, 10);
	TextOut(hDC,
		_left + 17,
		_top + 7,
		buf, lstrlen(buf));
	(HFONT)DeleteObject(oldFont);

	oldFont = (HFONT)SelectObject(hDC, hFont);
	_itow_s(m_tSpellInfo.iAtk, buf, 10);
	TextOut(hDC,
		_left + 102,
		_top + 152,
		buf, lstrlen(buf));
	(HFONT)DeleteObject(oldFont);

	hFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("�����ý��丮"));
	oldFont = (HFONT)SelectObject(hDC, hFont);

	SetTextColor(hDC, RGB(200, 200, 200));
	TextOut(hDC,
		_left + 77,
		_top + 12,
		m_pSpellName, lstrlen(m_pSpellName));
	(HFONT)DeleteObject(oldFont);

	SetTextColor(hDC, RGB(255, 255, 255));
	TCHAR* DetailBuf = m_tSpellInfo.szDetail;
	TCHAR  szBuf[2] = {};
	TCHAR  CharBuf[32] = {};
	for (int i = 0; i < lstrlen(m_tSpellInfo.szDetail) / 11; ++i)
	{
		if (11 <= lstrlen(&DetailBuf[11 * i]))
		{
			for (int j = 0; j < 11; ++j)
			{
				szBuf[0] = DetailBuf[j];
				_tcscat_s(CharBuf, &szBuf[0]);
			}
			oldFont = (HFONT)SelectObject(hDC, hFont);
			TextOut(hDC,
				_left + 60,
				_top + 100 + 20 * i, // 20�� ����
				CharBuf, lstrlen(CharBuf));
			(HFONT)DeleteObject(oldFont);

			// ���ڿ� 12�������� �ڸ�
			DetailBuf = &m_tSpellInfo.szDetail[11 * (i + 1)];
		}
	}

	oldFont = (HFONT)SelectObject(hDC, hFont);
	TextOut(hDC,
		_left + 60,
		_top + 100 + 20,
		DetailBuf, lstrlen(DetailBuf));
	(HFONT)DeleteObject(oldFont);

	oldFont = (HFONT)SelectObject(hDC, hFont);
	_itow_s(m_tSpellInfo.iMoney, buf, 10);
	TextOut(hDC,
		_left + 22,
		_top + 155,
		buf, lstrlen(buf));
	(HFONT)DeleteObject(oldFont);

	HDC		hMoneyDC = BITMAP->Find_Img(L"money-icon");
	GdiTransparentBlt(hDC,
		_left + 32,
		_top + 155,
		15,
		15,
		hMoneyDC,
		0,
		0,
		512,		// �ؽ�ó ���� ������
		512,
		RGB(1, 1, 1));

	// ���콺 ����
	if (MOUSE->Click_Event((float)_left, (float)_top, (float)_left + 120, (float)_top + 180))
	{
		HDC		hMoneyDC = BITMAP->Find_Img(L"CursorArt");
		GdiTransparentBlt(hDC,
			_left - 10,
			_top - 20,
			140,
			220,
			hMoneyDC,
			0,
			0,
			200,		// �ؽ�ó ���� ������
			200,
			RGB(255, 255, 255));
	}
}