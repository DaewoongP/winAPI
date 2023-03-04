#pragma once
#include "Scene.h"

class CStage3 : public CScene
{
public:
	CStage3();
	virtual ~CStage3();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

