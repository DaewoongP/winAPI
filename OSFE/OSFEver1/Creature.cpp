#include "stdafx.h"
#include "Creature.h"
#include "SceneMgr.h"
#include "DamageUI.h"
#include "AbstractFactory.h"

CCreature::CCreature()
	:m_iPreTileNum(m_iTileNum) ,m_ePreState(STATE_END), m_eCurState(IDLE),
	m_iDamageY(0), m_dwDamageTime(GetTickCount()), m_bDamaged(false)
{
	ZeroMemory(&m_tCreature, sizeof(CREATURE));
}


CCreature::~CCreature()
{
	for_each(m_DamageList.begin(), m_DamageList.end(), Safe_Delete<CDamageUI*>);
	m_DamageList.clear();
}

void CCreature::Render(HDC hDC)
{
	// -------------------------- ü��
	TCHAR buf[32] = {};
	_itow_s(m_tCreature.iHp, buf, 10); // hp�� int -> wstring
	HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("quadaptor")); // ��Ʈ����
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont); // DC����
	if (0 < m_tCreature.iShield)
		SetTextAlign(hDC, TA_RIGHT); // �ؽ�Ʈ �߾�����
	else
		SetTextAlign(hDC, TA_CENTER); // �ؽ�Ʈ �߾�����
	SetTextColor(hDC, RGB(255, 255, 255)); // �ؽ�Ʈ �÷� ����
	SetBkMode(hDC, TRANSPARENT); // �Ű����� DC�� ���ڹ���� �����ϰ� �Ѵ�. 
	TextOut(hDC, int(m_tInfo.fX), int(m_tRect.bottom - m_tInfo.fCY * 0.45f), buf, lstrlen(buf));
	(HFONT)DeleteObject(oldFont); // ���� �ִ���Ʈ ����
	// -------------------------- �ǵ�
	if (0 < m_tCreature.iShield)
	{
		TCHAR cShield[32] = {};
		TCHAR buf[32] = {};
		// int to wstring
		_tcscat_s(buf, L" [");
		_itow_s(m_tCreature.iShield, cShield, 10); // hp�� int -> wstring
		_tcscat_s(buf, cShield);
		_tcscat_s(buf, L"]");
		HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			VARIABLE_PITCH | FF_ROMAN, TEXT("quadaptor")); // ��Ʈ����
		HFONT oldFont = (HFONT)SelectObject(hDC, hFont); // DC����
		SetTextAlign(hDC, TA_LEFT); // �ؽ�Ʈ �߾�����
		SetTextColor(hDC, RGB(108, 135, 235)); // �ؽ�Ʈ �÷� ����
		SetBkMode(hDC, TRANSPARENT); // �Ű����� DC�� ���ڹ���� �����ϰ� �Ѵ�. 
		TextOut(hDC, int(m_tInfo.fX), int(m_tRect.bottom - m_tInfo.fCY * 0.45f), buf, lstrlen(buf));
		(HFONT)DeleteObject(oldFont); // ���� �ִ���Ʈ ����
	}
}

void CCreature::Set_Damage(int _Damage)
{
	m_tCreature.iHp -= _Damage;
	m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX - 50, m_tInfo.fY - 100 - m_iDamageY, _Damage));
}

void CCreature::Reset_DamageFont()
{
	if (m_DamageList.empty())
		m_dwDamageTime = GetTickCount();
	if (m_dwDamageTime + 500 < GetTickCount())
	{
		m_iDamageY = 0;
		for_each(m_DamageList.begin(), m_DamageList.end(), Safe_Delete<CDamageUI*>);
		m_DamageList.clear();
		m_dwDamageTime = GetTickCount();
	}
}
