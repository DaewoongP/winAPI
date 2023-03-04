#pragma once
#include "Effect.h"
class CHitEffect : public CEffect
{
public:
	CHitEffect();
	virtual ~CHitEffect();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

