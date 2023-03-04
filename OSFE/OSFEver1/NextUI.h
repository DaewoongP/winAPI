#pragma once
#include "UI.h"
class CNextUI : public CUI
{
public:
	CNextUI();
	virtual ~CNextUI();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
private:
	float			m_fSpeed;
};

