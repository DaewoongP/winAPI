#pragma once
#include "Effect.h"
class CSeraDropEffect : public CEffect
{
public:
	CSeraDropEffect();
	virtual ~CSeraDropEffect();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
public:
	void Set_EffectDelete() { m_iEffectCnt = 0; }
	void Jump();
	void Chase();
private:
	float		m_fSpeed;
	float		m_fTime;
	float		m_fAccel;
	float		m_fPower;
	float		m_fGravity;
	float		m_fPreY;
	float		m_fJump;
	float		m_fWidth;
	float		m_fHeight;
	DWORD		m_dwTime;
	INFO		PlayerInfo;
};

