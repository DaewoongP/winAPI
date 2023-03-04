#pragma once
#include "Obj.h"

class CTile : public CObj
{
public:
	CTile();
	virtual ~CTile();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collilsion_Event(CObj* _pObj) override;
	virtual void Count_Trigger(int _iTriggerCnt) override;
public:
	void Set_TileState(TILESTATE _eState) { m_eState = _eState; }
	void Set_TileAttack(int _Attack) { m_iAttack = _Attack; }
	INFO Get_TileInfo() { return m_tInfo; }
	
	TILESTATE Get_TileState() { return m_eState; }
	int		Get_TileAttack() { return m_iAttack; }

private:
	TILESTATE		m_eState;
	int				m_iAttack;
};

