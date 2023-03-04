#pragma once
#include "Scene.h"
class CStage_Gate : public CScene
{
public:
	CStage_Gate();
	virtual ~CStage_Gate();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

