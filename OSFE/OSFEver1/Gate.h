#pragma once
#include "Enemie.h"
#include "GateBeamer.h"
#include "GateMissiler.h"
#include "GateSweeper.h"
#include "GateCannon.h"

class CGate : public CEnemie
{
private:
	enum PATTERN { BEAMER, MISSILER, SWEEPER, PATTERN_END };
public:
	CGate();
	virtual ~CGate();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Attack() override;
	virtual void Motion_Change() override;
	virtual void Collilsion_Event(CObj * _pObj) override;
	virtual void Count_Trigger(int _iTriggerCnt) override;
public:
	void Select_Pattern();
	void Set_Spawn1Cnt(int _Cnt) {
		if (m_iSpawn1Cnt)
			m_iSpawn1Cnt += _Cnt;
	}
	void Set_Spawn2Cnt(int _Cnt) {
		if (m_iSpawn2Cnt)
			m_iSpawn2Cnt += _Cnt;
	}
	void Set_Cannon1Cnt(int _Cnt) {
		if (m_iCannon1Cnt)
			m_iCannon1Cnt += _Cnt;
	}
	void Set_Cannon2Cnt(int _Cnt) {
		if (m_iCannon2Cnt)
			m_iCannon2Cnt += _Cnt;
	}
	int	Get_Spawn2Cnt() { 
		if (m_iSpawn2Cnt == 1)
			return true;
		else
			return false;
	}
	void Spawn_Cannon();
private:
	int			m_iSpawn1Cnt;
	int			m_iSpawn2Cnt;
	int			m_iCannon1Cnt;
	int			m_iCannon2Cnt;
	int			m_iSpawnTileNum;
	INFO		m_tSpawnInfo;
	DWORD		m_dwCannon;

	bool		m_bGateDead;
};

