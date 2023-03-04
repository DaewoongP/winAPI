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
	m_hVideo = MCIWndCreate(g_hWnd,		// �θ� ������ �ڵ�
		nullptr,	// mci�����츦 ����ϴ� �ν��Ͻ� �ڵ�
		WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR,  // ������ ��� ���� �ɼ�(WS_CHILD : �ڽ� â WS_VISIBLE: �� ��� ��� MCIWNDF_NOPLAYBAR : �÷��� �ٸ� ����� �� ��)
		L"../Video/Scene_bad.wmv");

	// ������ ������ ũ�⸦ â �������� ���缭 ���
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
