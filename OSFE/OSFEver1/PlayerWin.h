#pragma once
#include "Scene.h"
class CPlayerWin : public CScene
{
public:
	CPlayerWin();
	virtual ~CPlayerWin();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
private:
	HWND	m_hVideo;
};

