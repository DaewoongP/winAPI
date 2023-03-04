#pragma once
#include "MyBitmap.h"
class CBitmapMgr
{
private:
	CBitmapMgr();
	~CBitmapMgr();

public:
	void		Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey);
	HDC			Find_Img(const TCHAR* pImgKey);
	void		Release(void);

private:
	static CBitmapMgr*	m_pInstance;
	map<const TCHAR*, CMyBitmap*>		m_mapBit;

public:
	static CBitmapMgr*	Get_Instance(void)
	{
		if (!m_pInstance)
			m_pInstance = new CBitmapMgr;

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
};

#define BITMAP		CBitmapMgr::Get_Instance()