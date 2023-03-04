#pragma once
#include "Effect.h"
class CExplodeEffect : public CEffect
{
public:
	CExplodeEffect();
	virtual ~CExplodeEffect();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

