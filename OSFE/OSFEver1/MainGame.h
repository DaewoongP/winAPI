#pragma once
#include "Define.h"
class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

private:
	HDC			m_hDC;
	DWORD		m_dwTime;

	TCHAR			m_szFPS[32];
	int				m_iFPS;
};

