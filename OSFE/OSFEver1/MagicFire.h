#pragma once
#include "Effect.h"
class CMagicFire : public CEffect
{
public:
	CMagicFire();
	virtual ~CMagicFire();
public:
	virtual void Initialize();
	virtual void Update();
	virtual void Late_Update();
	virtual void Render(HDC hDC);
	virtual void Release();
};

