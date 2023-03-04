#pragma once
#include "Scene.h"
class CPlayerDead : public CScene
{
public:
	CPlayerDead();
	virtual ~CPlayerDead();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
public:
	void Player_Dead(HDC hDC);
private:
	int			DeadCnt;
};

