#pragma once
#include "BuffSpell.h"

class CHealOrb : public CBuffSpell
{
public:
	CHealOrb();
	virtual ~CHealOrb();
public:
	virtual void	Initialize();
	virtual int		Update();
	virtual void	Late_Update();
	virtual void	Render(HDC hDC);
	virtual void	Release();
	virtual void	Collilsion_Event(CObj * _pObj) override;
	virtual void	Count_Trigger(int _iTriggerCnt) override;
	virtual void	Player_Effect() override;
public:
	void Heal_Position();
private:
	bool			m_bJump;
};

