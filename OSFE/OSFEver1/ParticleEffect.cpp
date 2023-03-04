#include "stdafx.h"
#include "ParticleEffect.h"
#include "BitmapMgr.h"
#include "AbstractFactory.h"
#include "EffectMgr.h"

CParticleEffect::CParticleEffect()
	:m_dwTime(GetTickCount())
{
}


CParticleEffect::~CParticleEffect()
{
	Release();
}

void CParticleEffect::Initialize()
{
	BITMAP->Insert_Bmp(L"../Image/Particle/ExpDropLarge.bmp", L"ExpDropLarge");
	BITMAP->Insert_Bmp(L"../Image/Particle/SeraDropLarge.bmp", L"SeraDropLarge");
	
	m_tInfo.fCX = 38.4f;
	m_tInfo.fCY = 38.4f;
	m_iEffectCnt = 4;
}

void CParticleEffect::Update()
{
	if (m_iEffectCnt)
	{
		if (m_dwTime + 50 < GetTickCount())
		{
			EFFECT->Add_Effect(EFFECT_PARTICLE, CAbstractFactory<CSeraDropEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
			EFFECT->Add_Effect(EFFECT_PARTICLE, CAbstractFactory<CExpDropEffect>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
			--m_iEffectCnt;
			m_dwTime = GetTickCount();
		}
		
	}
	__super::Update_Rect();
}

void CParticleEffect::Late_Update()
{
}

void CParticleEffect::Render(HDC hDC)
{
}

void CParticleEffect::Release()
{
}
