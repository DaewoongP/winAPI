#pragma once
#include "Scene.h"
class CMyMenu : public CScene
{
private:
	enum SELECT_MENU {GAME_START, GAME_END, MENU_END};
public:
	CMyMenu();
	virtual ~CMyMenu();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
public:
	void Game_Start(HDC hDC);
	void Game_End(HDC hDC);
private:
	SELECT_MENU			m_eMenu;
};

