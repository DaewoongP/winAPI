#include "stdafx.h"
#include "EffectMgr.h"

CEffectMgr* CEffectMgr::m_pInstance = nullptr;

CEffectMgr::CEffectMgr()
{
}


CEffectMgr::~CEffectMgr()
{
	Release();
}

void CEffectMgr::Add_Effect(EFFECTID eID, CEffect* pEffect)
{
	if (EFFECT_END <= eID || nullptr == pEffect)
		return;

	m_EffectList[eID].push_back(pEffect);
}

void CEffectMgr::Delete_Effect(EFFECTID eID)
{
	for (auto& iter : m_EffectList[eID])
		Safe_Delete(iter);

	m_EffectList[eID].clear();
}

void CEffectMgr::Update(void)
{
	for (size_t i = 0; i < EFFECT_END; ++i)
	{
		for (auto iter = m_EffectList[i].begin();
		iter != m_EffectList[i].end();)
		{
			(*iter)->Update();

			if (0 >= (*iter)->Get_Count())
			{
				Safe_Delete<CEffect*>(*iter);
				iter = m_EffectList[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CEffectMgr::Late_Update(void)
{
	for (size_t i = 0; i < EFFECT_END; ++i)
	{
		for (auto& iter : m_EffectList[i])
		{
			iter->Late_Update();
		}
	}
}

void CEffectMgr::Render(HDC hDC)
{
	for (size_t i = AIM; i < EFFECT_END; ++i)
	{
		for (auto& iter : m_EffectList[i])
		{
			iter->Render(hDC);
		}
	}
}

void CEffectMgr::Release(void)
{
	for (size_t i = 0; i < EFFECT_END; ++i)
	{
		for_each(m_EffectList[i].begin(), m_EffectList[i].end(), Safe_Delete<CEffect*>);
		m_EffectList[i].clear();
	}
}
