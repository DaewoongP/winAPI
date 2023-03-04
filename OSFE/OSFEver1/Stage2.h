#pragma once
#include "Scene.h"
class CStage2 : public CScene
{
public:
	CStage2();
	virtual ~CStage2();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
private:
	bool			m_bRewardSelected;
};

