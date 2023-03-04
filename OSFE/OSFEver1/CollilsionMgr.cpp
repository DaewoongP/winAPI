#include "stdafx.h"
#include "CollilsionMgr.h"


CCollilsionMgr::CCollilsionMgr()
{
}


CCollilsionMgr::~CCollilsionMgr()
{
}

void CCollilsionMgr::Collision_Tile(list<CObj*> _Dest, list<CObj*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (Check_Tile(Dest, Src))
			{
				if (static_cast<CSpell*>(Src)->Get_SpellInfo().iAtk > 0)
				{
					Dest->Collilsion_Event(Src);
					Src->Collilsion_Event(Dest);
				}
			}
		}
	}
}

bool CCollilsionMgr::Check_Tile(CObj * pDest, CObj * pSrc)
{
	return pDest->Get_TileNum() == pSrc->Get_TileNum();
}

void CCollilsionMgr::Collision_TileEvent(list<CObj*> _Dest, list<CObj*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (!dynamic_cast<CEnemie*>(Src)->Get_AttackTileList().empty())
			{
				for (auto& Tile : dynamic_cast<CEnemie*>(Src)->Get_AttackTileList())
				{
					if (Dest->Check_Tile() == Tile->Check_Tile())
					{
						Dest->Collilsion_Event(Src);
						Src->Collilsion_Event(Dest);
					}
						
				}
			}
		}
	}
}




void CCollilsionMgr::Collision_Rect(list<CObj*> _Dest, list<CObj*> _Src)
{
	RECT		rc{};

	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &(Dest->Get_Rect()), &(Src->Get_Rect())))
			{
				Dest->Collilsion_Event(Src);
				Src->Collilsion_Event(Dest);
			}
		}
	}
}

void CCollilsionMgr::Collision_RectEx(list<CObj*> _Dest, list<CObj*> _Src)
{
	float	fX = 0.f, fY = 0.f;


	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{

			if (Check_Rect(Dest, Src, &fX, &fY))
			{
				// 상하 충돌
				if (fX > fY)
				{
					// 상 충돌
					if (Dest->Get_Info().fY < Src->Get_Info().fY)
					{
						Dest->Set_Y(-fY);
					}
					else // 하 충돌
					{
						Dest->Set_Y(fY);
					}

				}
				else // 좌우 충돌
				{
					// 좌 충돌
					if (Dest->Get_Info().fX < Src->Get_Info().fX)
					{
						Dest->Set_X(-fX);
					}
					else // 우 충돌
					{
						Dest->Set_X(fX);
					}
				}

			}
		}
	}
}

void CCollilsionMgr::Collision_Sphere(list<CObj*> _Dest, list<CObj*> _Src)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dest, Src))
			{
				Dest->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

void CCollilsionMgr::Collision_Gate(CObj* _Gate, list<CObj*> _Spell)
{
	for (auto& Spell : _Spell)
	{
		if (Spell->Get_TileNum() % 8 == 6 || Spell->Get_TileNum() % 8 == 7)
		{
			if (static_cast<CSpell*>(Spell)->Get_SpellInfo().iAtk > 0)
			{
				_Gate->Collilsion_Event(Spell);
				Spell->Collilsion_Event(_Gate);
			}
		}
	}
}

bool CCollilsionMgr::Check_Sphere(CObj * pDest, CObj * pSrc)
{
	float		fWidth = fabs(pDest->Get_Info().fX - pSrc->Get_Info().fX);
	float		fHeight = fabs(pDest->Get_Info().fY - pSrc->Get_Info().fY);

	float		fDistance = sqrt(fWidth * fWidth + fHeight * fHeight);

	float		fRadius = (pDest->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;

	return fRadius >= fDistance;
}

bool CCollilsionMgr::Check_Rect(CObj * pDest, CObj * pSrc, float * pX, float * pY)
{
	float	fX = fabs(pDest->Get_Info().fX - pSrc->Get_Info().fX);
	float	fY = fabs(pDest->Get_Info().fY - pSrc->Get_Info().fY);

	float	fRadiusX = (pDest->Get_Info().fCX + pSrc->Get_Info().fCX) * 0.5f;
	float	fRadiusY = (pDest->Get_Info().fCY + pSrc->Get_Info().fCY) * 0.5f;

	if ((fRadiusX > fX) && (fRadiusY > fY))
	{
		*pX = fRadiusX - fX;
		*pY = fRadiusY - fY;

		return true;
	}

	return false;
}

