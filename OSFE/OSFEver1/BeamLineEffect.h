#pragma once
#include "Effect.h"
class CBeamLineEffect : public CEffect
{
private:
	struct PENINFO
	{
		HPEN	CurPen;
		HBRUSH	CurBrush;
		HPEN	OldPen;
		HBRUSH	OldBrush;

		POINT		Point_Start;
		POINTPTR	Point_End;
	};
public:
	CBeamLineEffect();
	virtual ~CBeamLineEffect();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
public:
	void Set_LineStart(float _fX, float _fY);
	void Set_LineEnd(POINTPTR _Pointptr);
private:
	PENINFO		m_tPenInfo;
};

