#pragma once
#include "Pet.h"
class CDogPet : public CPet
{
public:
	CDogPet();
	virtual ~CDogPet();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Motion_Change() override;
	virtual void Collilsion_Event(CObj * _pObj) override;
	virtual void Count_Trigger(int _iTriggerCnt) override;
public:
	void Attack();
	void Angle();
private:
	INFO		m_tTargetInfo;
	float		m_fWidth;
	float		m_fHeight;
	float		m_fAngle;
	bool		m_bAttack;
};

