#pragma once
#include "Define.h"
class CMouse
{
public:
	CMouse();
	~CMouse();
public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();

	INFO Get_Info() { return m_tInfo; }
private:
	INFO		m_tInfo;
	RECT		m_tRect;
};

