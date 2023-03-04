#pragma once
#include "Enemie.h"
#include "GateBeam.h"
#include "BeamLineEffect.h"

class CGateBeamer : public CEnemie
{
public:
	CGateBeamer();
	virtual ~CGateBeamer();
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
private:
	int			m_iRenderOffsetY;
};

