#pragma once
#include "Effect.h"
class CRhythmEffect : public CEffect
{
public:
	CRhythmEffect();
	virtual ~CRhythmEffect();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

