#pragma once
#include "Obj.h"
#include "Effect.h"
#include "UI.h"
#include "DamageUI.h"

template<typename T>
class CAbstractFactory
{
public:
	CAbstractFactory(){}
	~CAbstractFactory(){}

public:
	static CObj*	Create()
	{
		CObj*	pObj = new T;
		pObj->Initialize();
		return pObj;
	}

	static CObj*	Create(float _fX, float _fY)
	{
		CObj*	pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		return pObj;
	}
	static CObj*	Create(int _TileNum)
	{
		CObj*	pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_TileNum);
		return pObj;
	}

	static CObj*	Create(const T& rhs, int _TargetTile)
	{
		CObj*	pObj = new T(rhs);
		pObj->Initialize();
		pObj->Set_Target(_TargetTile);
		return pObj;
	}

	static CEffect*	Create_Effect()
	{
		CEffect*	pEffect = new T;
		pEffect->Initialize();
		return pEffect;
	}

	static CEffect*	Create_Effect(const T& rhs)
	{
		CEffect*	pEffect = new T(rhs);
		pEffect->Initialize();
		return pEffect;
	}

	static CEffect*	Create_Effect(float _fX, float _fY)
	{
		CEffect*	pEffect = new T;
		pEffect->Initialize();
		pEffect->Set_Pos(_fX, _fY);
		return pEffect;
	}

	static CUI* Create_UI()
	{
		CUI* pUI = new T;
		pUI->Initialize();
		return pUI;
	}

	static CDamageUI* Create_DamageUI(float _fX, float _fY, int _dmg)
	{
		CDamageUI* pUI = new T;
		pUI->Initialize();
		pUI->Set_Damage(_fX, _fY, _dmg);
		return pUI;
	}

	static CObj*	Create_Spell(const T& rhs, int _TileNum)
	{
		CObj*	pObj = new T(rhs);
		pObj->Initialize();
		pObj->Set_Pos(_TileNum);
		return pObj;
	}

};