#pragma once
#include "Obj.h"
#include "Tile.h"
#include "Enemie.h"
class CCollilsionMgr
{
public:
	CCollilsionMgr();
	~CCollilsionMgr();

public:
	static void Collision_Tile(list<CObj*> _Dest, list<CObj*> _Src);
	static bool Check_Tile(CObj * pDest, CObj * pSrc);
	static void Collision_TileEvent(list<CObj*> _Dest, list<CObj*> _Src);

	static void Collision_Rect(list<CObj*> _Dest, list<CObj*> _Src);
	static void Collision_RectEx(list<CObj*> _Dest, list<CObj*> _Src);
	static void Collision_Sphere(list<CObj*> _Dest, list<CObj*> _Src);

	static void Collision_Gate(CObj* _Gate, list<CObj*> _Spell);

	static bool Check_Sphere(CObj* pDest, CObj* pSrc);
	static bool Check_Rect(CObj* pDest, CObj* pSrc, float* pX, float* pY);
};

