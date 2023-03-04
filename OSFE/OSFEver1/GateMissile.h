#pragma once
#include "Enemie.h"
class CGateMissile : public CEnemie
{
public:
	CGateMissile();
	virtual ~CGateMissile();
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
	void Rotate_Missile();
	void Target_Angle();
private:
	POINT		m_tPoint[3];
	float		m_fAngle;
	INFO		m_tTargetInfo;
	float		m_fWidth;
	float		m_fHeight;
};

