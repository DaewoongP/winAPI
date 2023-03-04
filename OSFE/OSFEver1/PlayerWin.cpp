#include "stdafx.h"
#include "PlayerWin.h"
#include "SceneMgr.h"
#include "BitmapMgr.h"

CPlayerWin::CPlayerWin()
{
}


CPlayerWin::~CPlayerWin()
{
	Release();
}

void CPlayerWin::Initialize()
{
	m_hVideo = MCIWndCreate(g_hWnd,		// 부모 윈도우 핸들
		nullptr,	// mci윈도우를 사용하는 인스턴스 핸들
		WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR,  // 윈도우 모양 설정 옵션(WS_CHILD : 자식 창 WS_VISIBLE: 그 즉시 재생 MCIWNDF_NOPLAYBAR : 플레이 바를 띄우지 말 것)
		L"../Video/Scene_bad.wmv");

	// 동영상 파일의 크기를 창 기준으로 맞춰서 출력
	MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, FALSE);

	MCIWndPlay(m_hVideo);
}

int CPlayerWin::Update()
{
	return 0;
}

void CPlayerWin::Late_Update()
{
	if ((MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo))
		|| KEY->Key_Down(VK_RETURN))
	{
		SCENE->Scene_Change(SC_MENU);
		return;
	}
}

void CPlayerWin::Render(HDC hDC)
{
	__super::Render(hDC);
}

void CPlayerWin::Release()
{
	KEY->Destroy_Instance();
	MCIWndClose(m_hVideo);
}
