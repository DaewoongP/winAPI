#pragma once
#include "Mouse.h"
class CMouseMgr
{
private:
	CMouseMgr();
	~CMouseMgr();
public:
	bool Click_Event(float _left, float _top, float _right, float _bottom);
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

private:
	static CMouseMgr*	m_pInstance;
	CMouse*				m_pMouse;

public:
	static CMouseMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CMouseMgr;
		}
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

#define MOUSE		CMouseMgr::Get_Instance()