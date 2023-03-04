#include "stdafx.h"
#include "BitmapMgr.h"

CBitmapMgr*		CBitmapMgr::m_pInstance = nullptr;

CBitmapMgr::CBitmapMgr()
{
}


CBitmapMgr::~CBitmapMgr()
{
	Release();
}

void CBitmapMgr::Insert_Bmp(const TCHAR * pFilePath, const TCHAR * pImgKey)
{
	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImgKey));
	
	if (iter == m_mapBit.end())
	{
		CMyBitmap*	pBmp = new CMyBitmap;
		pBmp->Load_Bmp(pFilePath);

		m_mapBit.emplace(pImgKey, pBmp);
	}
}

HDC CBitmapMgr::Find_Img(const TCHAR * pImgKey)
{

	auto	iter = find_if(m_mapBit.begin(), m_mapBit.end(), CTag_Finder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;


	return iter->second->Get_MemDC();
}

void CBitmapMgr::Release(void)
{
	for_each(m_mapBit.begin(), m_mapBit.end(), CDeleteMap());
	m_mapBit.clear();
}
