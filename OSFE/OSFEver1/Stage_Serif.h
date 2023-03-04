#pragma once
#include "Scene.h"
class CStage_Serif : public CScene
{
public:
	CStage_Serif();
	virtual ~CStage_Serif();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
};

