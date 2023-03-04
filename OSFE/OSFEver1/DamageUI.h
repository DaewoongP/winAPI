#pragma once
#include "UI.h"
class CDamageUI : public CUI
{
public:
	CDamageUI();
	virtual ~CDamageUI();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
public:
	void Set_Damage(float _fX, float _fY, int _dmg)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
		m_iDmg = _dmg;
	}
public:
	void Render_Damage(HDC hDC);
private:
	DWORD			m_dwTime;
	int				m_iDmg;
};

