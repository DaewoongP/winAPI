#pragma once
#include "Scene.h"
#include "Define.h"
#include "LogoScene.h"
#include "MyMenu.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "Stage_Serif.h"
#include "Stage_Gate.h"

#include "PlayerDead.h"
#include "PlayerWin.h"
#include "Obj.h"

class CSceneMgr
{
public:
	CSceneMgr();
	~CSceneMgr();
public:
	void   Scene_Change(SCENEID eID);
	void   Update();
	void   Late_Update();
	void   Render(HDC hDC);
	void   Release();

	CScene*	Get_Scene() { return m_pScene; }
	SCENEID Get_SceneID() { return m_eCurScene; }
private:
	static CSceneMgr*       m_pInstance;
	CScene*					m_pScene;
	SCENEID					m_eCurScene;
	SCENEID					m_ePreScene;
	CObj*					m_pPrePlayer;
	CObj*					m_pPrePet;

public:
	static	CSceneMgr*		Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSceneMgr;
		return m_pInstance;
	}

	static void Destroy_Instance(void)
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

};

#define SCENE		CSceneMgr::Get_Instance()