#include "stdafx.h"
#include "MainGame.h"
#include "BitmapMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"


CMainGame::CMainGame()
	:m_dwTime(GetTickCount()), m_iFPS(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);
	BITMAP->Insert_Bmp(L"../Image/Back.bmp", L"Back");
	SOUND->Initialize();
	SCENE->Scene_Change(SC_LOGO);
	// ÄÜ¼ÖÃ¢
#ifdef _DEBUG

	/*if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}*/

#endif // _DEBUG
}

void CMainGame::Update()
{
	DWORD dwExitCode = 0;
	SCENE->Update();
}
	
void CMainGame::Late_Update()
{
	SCENE->Late_Update();
}

void CMainGame::Render()
{
	if (SC_WIN == SCENE->Get_SceneID())
		return;

	++m_iFPS;
	if (m_dwTime + 1000 < GetTickCount())
	{
		swprintf_s(m_szFPS, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}

	HDC	hBackDC = BITMAP->Find_Img(L"Back");
	SCENE->Render(hBackDC);
	BitBlt(m_hDC, 0, 0, WINCX, WINCY, hBackDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	//FreeConsole();
	SCENE->Destroy_Instance();
	SOUND->StopAll();
	SOUND->Destroy_Instance();
	BITMAP->Destroy_Instance();

	ReleaseDC(g_hWnd, m_hDC);
}
