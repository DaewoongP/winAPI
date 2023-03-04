#pragma once
#include "Enemie.h"
#include "Shop.h"

class CShopKeeper : public CCreature
{
private:
	enum STATE { IDLE, SHOP_OPEN, SHOP_CLOSE, STATE_END };
public:
	CShopKeeper();
	virtual ~CShopKeeper();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Collilsion_Event(CObj* _pObj) override;
	virtual void Count_Trigger(int _iTriggerCnt) override;
public:
	void Motion_Change();
	void Set_ShopOpen() { m_eCurState = SHOP_OPEN; }
	void Set_ShopClose() { m_eCurState = SHOP_CLOSE; }

	bool IsShopOpen() {return m_eCurState == SHOP_OPEN;}

	void ShopKeeper_Position();

private:
	STATE				m_ePreState;
	STATE				m_eCurState;
};

