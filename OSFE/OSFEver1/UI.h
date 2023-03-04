#pragma once
#include "Define.h"
#include "Player.h"

class CUI
{
// ��, ����, ����, ����, �κ��丮
// ����, ��, ���Ժ��� ó��
public:
	CUI();
	virtual ~CUI();
public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;
public:
	void Update_Rect();
protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	FRAME		m_tFrame;
	TCHAR*		m_pFrameKey;
	UIID		m_eID;
};

