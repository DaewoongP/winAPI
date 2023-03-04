#pragma once
#include "UI.h"
class CPlayerUI : public CUI
{
public:
	CPlayerUI();
	virtual ~CPlayerUI();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
public:
	void HP_UI(HDC hDC);
	void Money_UI(HDC hDC);
	void Level_UI(HDC hDC);
	void Inven_UI(HDC hDC);
};

