#pragma once
#include "Effect.h"
#include "SeraDropEffect.h"
#include "ExpDropEffect.h"
class CParticleEffect : public CEffect
{
public:
	CParticleEffect();
	virtual ~CParticleEffect();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
private:
	DWORD		m_dwTime;
};

	