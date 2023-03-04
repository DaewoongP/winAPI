#pragma once
#include "UI.h"
class CBackGroundUI : public CUI
{
public:
	CBackGroundUI();
	virtual ~CBackGroundUI();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
public:
	void Deck_UI(HDC hDC);
	void Slot_UI(HDC hDC);
	void MP_UI(HDC hDC);

private:
	INFO		m_tMana;
	RECT		m_tManaRect;
};

