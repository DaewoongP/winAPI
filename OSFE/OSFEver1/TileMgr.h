#pragma once
#include "Tile.h"
class CTileMgr
{
private:
	CTileMgr();
	~CTileMgr();
public:
	void		Initialize(void);
	void		Update(void);
	void		Late_Update(void);
	void		Render(HDC hDC);
	void		Release(void);

	INFO		Get_TileInfo(int _TileNum) 
	{
		if (0 <= _TileNum && 32 > _TileNum)
			return dynamic_cast<CTile*>(m_vecTile[_TileNum])->Get_Info();
		else
			return {};
	}
	CObj*		Get_Tile(int _TileNum)
	{
		if (0 <= _TileNum && 32 > _TileNum)
			return m_vecTile[_TileNum];
		return nullptr;
	}
	vector<CObj*>* Get_vecTile()
	{
		return &m_vecTile;
	}
private:
	static CTileMgr*		m_pInstance;
	vector<CObj*>			m_vecTile;

public:
	static CTileMgr*	Get_Instance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTileMgr;
		}
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

#define TILE	CTileMgr::Get_Instance()