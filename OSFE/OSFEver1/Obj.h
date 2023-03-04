#pragma once
#include "Define.h"
class CObj
{
public:
	CObj();
	virtual ~CObj();
public:
	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;
	virtual void Collilsion_Event(CObj* _pObj) PURE;
	virtual void Count_Trigger(int _iTriggerCnt) PURE;
public:
	void Set_Dead() { m_bDead = true; }
	void Set_Pos(int _num);

	void Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}

	void Set_Target(int _num) { m_iTargetTile = _num; }

	INFO	Get_Info() { return m_tInfo; }
	RECT	Get_Rect() { return m_tRect; }
	OBJID	Get_OBJID() { return m_eObjID; }
	int		Get_TileNum() { return m_iTileNum; }

	void	Set_X(float _fX) { m_tInfo.fX += _fX; }
	void	Set_Y(float _fY) { m_tInfo.fY += _fY; }

	void Update_Rect();
	void Move_Frame();
	int Check_Tile();
	
protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	FRAME		m_tFrame;
	OBJID		m_eObjID;
	TCHAR*		m_pFrameKey;
	DWORD		m_dwTime;
	bool		m_bDead;
	float		m_fSpeed;
	int			m_iTileNum;
	int			m_iTargetTile;
	int			m_iRenderCnt;
	bool		m_bRenderSkip;
};

