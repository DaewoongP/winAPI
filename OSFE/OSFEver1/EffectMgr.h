#pragma once
#include "Effect.h"
class CEffectMgr
{
private:
	CEffectMgr();
	~CEffectMgr();
public:
	void	Add_Effect(EFFECTID eID, CEffect* pEffect);
	void	Delete_Effect(EFFECTID eID);
	void	Update(void);
	void	Late_Update(void);
	void	Render(HDC hDC);
	void	Release(void);
public:
	list<CEffect*> Get_Effect(EFFECTID _ID) { return m_EffectList[_ID]; }

private:
	static CEffectMgr*	m_pInstance;
	list<CEffect*>		m_EffectList[EFFECT_END];

public:
	static CEffectMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CEffectMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};

#define		EFFECT		CEffectMgr::Get_Instance()