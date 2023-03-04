#include "stdafx.h"
#include "SceneMgr.h"

CSceneMgr*	CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr()
	:m_eCurScene(SC_LOGO), m_ePreScene(SC_END), m_pScene(nullptr), 
	m_pPrePlayer(nullptr), m_pPrePet(nullptr)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENEID eID)
{
	m_eCurScene = eID;
	if (m_eCurScene != m_ePreScene)
	{		
		switch (eID)
		{
		case SC_LOGO:
			Safe_Delete(m_pScene);
			m_pScene = new CLogoScene;
			break;
		case SC_MENU:
			Safe_Delete(m_pScene);
			m_pScene = new CMyMenu;
			break;
		case SC_STAGE1:
			// 플레이어 생성
			Safe_Delete(m_pScene);
			m_pScene = new CStage1;
			break;
		case SC_STAGE2:
			if (!m_pPrePlayer)
				m_pPrePlayer = m_pScene->Get_Player();
			if (!m_pPrePet)
				m_pPrePet = m_pScene->Get_Pet();
			Safe_Delete(m_pScene);
			m_pScene = new CStage2;
			m_pScene->Add_Object(PLAYER, m_pPrePlayer);
			m_pScene->Add_Object(PET, m_pPrePet);
			break;
		case SC_STAGE3:
			if (!m_pPrePlayer)
				m_pPrePlayer = m_pScene->Get_Player();
			if (!m_pPrePet)
				m_pPrePet = m_pScene->Get_Pet();
			Safe_Delete(m_pScene);
			m_pScene = new CStage3;
			m_pScene->Add_Object(PLAYER, m_pPrePlayer);
			m_pScene->Add_Object(PET, m_pPrePet);
			break;
		case SC_STAGE4:
			if (!m_pPrePlayer)
				m_pPrePlayer = m_pScene->Get_Player();
			if (!m_pPrePet)
				m_pPrePet = m_pScene->Get_Pet();
			Safe_Delete(m_pScene);
			m_pScene = new CStage4;
			m_pScene->Add_Object(PLAYER, m_pPrePlayer);
			m_pScene->Add_Object(PET, m_pPrePet);
			break;
		case SC_SERIF:
			if (!m_pPrePlayer)
				m_pPrePlayer = m_pScene->Get_Player();
			if (!m_pPrePet)
				m_pPrePet = m_pScene->Get_Pet();
			Safe_Delete(m_pScene);
			m_pScene = new CStage_Serif;
			m_pScene->Add_Object(PLAYER, m_pPrePlayer);
			m_pScene->Add_Object(PET, m_pPrePet);
			break;
		case SC_GATE:
			if (!m_pPrePlayer)
				m_pPrePlayer = m_pScene->Get_Player();
			if (!m_pPrePet)
				m_pPrePet = m_pScene->Get_Pet();
			Safe_Delete(m_pScene);
			m_pScene = new CStage_Gate;
			m_pScene->Add_Object(PLAYER, m_pPrePlayer);
			m_pScene->Add_Object(PET, m_pPrePet);
			break;
		case SC_DEAD:
			Safe_Delete(m_pScene);
			m_pScene = new CPlayerDead;
			break;
		case SC_WIN:
			Safe_Delete(m_pScene);
			m_pScene = new CPlayerWin;
			break;
		default:
			break;
		}
		m_pScene->Initialize();
			
		m_ePreScene = m_eCurScene;
	}
}

void CSceneMgr::Update()
{
	m_pScene->Update();
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC hDC)
{
	m_pScene->Render(hDC);
}

void CSceneMgr::Release()
{
	m_pScene->Delete_Object(PLAYER);
	Safe_Delete(m_pScene);
}

