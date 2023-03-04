#include "stdafx.h"
#include "MouseMgr.h"
#include "KeyMgr.h"

CMouseMgr* CMouseMgr::m_pInstance = nullptr;

CMouseMgr::CMouseMgr()
	:m_pMouse(nullptr)
{
}


CMouseMgr::~CMouseMgr()
{
	Release();
}

bool CMouseMgr::Click_Event(float _left, float _top, float _right, float _bottom)
{ // 키다운은 밖에 구현해야 함.
	if (m_pMouse)
	{
		INFO MouseInfo = m_pMouse->Get_Info();

		return	_left <= MouseInfo.fX && _right >= MouseInfo.fX &&
			_top <= MouseInfo.fY && _bottom >= MouseInfo.fY;
	}
	else
		return false;
}

void CMouseMgr::Initialize()
{
	if (!m_pMouse)
	{
		m_pMouse = new CMouse;
		m_pMouse->Initialize();
	}
}

void CMouseMgr::Update()
{
	m_pMouse->Update();
}

void CMouseMgr::Late_Update()
{
	m_pMouse->Late_Update();
}

void CMouseMgr::Render(HDC hDC)
{
	m_pMouse->Render(hDC);
}

void CMouseMgr::Release()
{
	Safe_Delete<CMouse*>(m_pMouse);
}
