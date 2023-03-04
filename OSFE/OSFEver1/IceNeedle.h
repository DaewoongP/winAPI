#pragma once
#include "ShotSpell.h"
class CIceNeedle : public CShotSpell
{
public:
	CIceNeedle();
	virtual ~CIceNeedle();
public:
	virtual void Initialize();
	virtual int Update();
	virtual void Late_Update();
	virtual void Render(HDC hDC);
	virtual void Release();
	virtual void Collilsion_Event(CObj* _pObj);
	virtual void Count_Trigger(int _iTriggerCnt);
	virtual void Player_Effect();

private:
	int		m_preTileNum;
};

