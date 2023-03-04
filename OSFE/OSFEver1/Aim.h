#pragma once
#include "Define.h"
#include "Effect.h"
class CAim : public CEffect
{
public:
	CAim();
	~CAim();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	void Aim_Pos();
};

