#pragma once
#include "Enemie.h"
class CGateBullet : public CEnemie
{
public:
	CGateBullet();
	virtual ~CGateBullet();
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
	void Set_Angle();
private:
	float			m_fWidth;
	float			m_fHeight;
};

