#pragma once
#include "UI.h"
#include "Reward.h"
class CRewardUI : public CUI
{
public:
	CRewardUI();
	virtual ~CRewardUI();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
public:
	bool Get_Selected() { return m_bSelected; }
private:
	CReward*			m_pReward;
	bool				m_bSelected;
};

