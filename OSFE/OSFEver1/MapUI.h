#pragma once
#include "UI.h"
class CMapUI : public CUI
{
public:
	CMapUI();
	virtual ~CMapUI();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
private:
	float			m_fSpeed;
};

