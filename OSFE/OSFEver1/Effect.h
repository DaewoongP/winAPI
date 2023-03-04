#pragma once
#include "Define.h"
class CEffect
{
public:
	CEffect();
	virtual ~CEffect();
public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;
public:
	void Update_Rect();
	void Move_Frame();
	void Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}
	int Get_Count() { return m_iEffectCnt; }
protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	FRAME		m_tFrame;
	TCHAR*		m_pFrameKey;

	int			m_iEffectCnt;
};

