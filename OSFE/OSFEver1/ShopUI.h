#pragma once
#include "UI.h"
#include "Shop.h"
class CShopUI : public CUI
{
public:
	CShopUI();
	virtual ~CShopUI();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
public:
	void SpellCard_BackGround(HDC hDC);
private:
	CShop*		m_pShop;
};

