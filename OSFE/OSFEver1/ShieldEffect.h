#pragma once
#include "Effect.h"
class CShieldEffect : public CEffect
{
public:
	CShieldEffect();
	virtual ~CShieldEffect();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

