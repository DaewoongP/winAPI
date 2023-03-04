#pragma once
#include "Define.h"
class CKeyMgr
{
private:
	CKeyMgr();
	~CKeyMgr();
public:
	bool Key_Down(int _iKey);
	bool Key_Up(int _iKey);
	bool Key_Pressing(int _iKey);

private:
	static CKeyMgr*		m_pInstance;
	bool				m_bKeyState[VK_MAX];

public:
	static CKeyMgr* Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CKeyMgr;
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

#define		KEY		CKeyMgr::Get_Instance()