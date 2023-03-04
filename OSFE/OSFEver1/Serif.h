#pragma once
#include "Enemie.h"
#include "Tile_Blast.h"
#include "LightSwordShot.h"
#include "LightBallRain.h"
#include "WingSword.h"
#include "LightSwordRain.h"
class CSerif : public CEnemie
{
private:
	enum STATE { IDLE, ATTACK_BLAST, ATTACK_SWORD, ATTACK_BALL, ATTACK_WING, ATTACK_RAIN, DEAD, YAMI, STATE_END };
	enum PATTERN { PATTERN_BLAST, PATTERN_SWORD, PATTERN_BALL, PATTERN_WING, PATTERN_RAIN, PATTERN_END };
	enum MOVE { MOVE_LEFT, MOVE_UP, MOVE_RIGHT, MOVE_DOWN, MOVE_END };
public:
	CSerif();
	virtual ~CSerif();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Attack() override;
	virtual void Motion_Change() override;
	virtual void Collilsion_Event(CObj * _pObj) override;
	virtual void Count_Trigger(int _iTriggerCnt) override;

public:
	void Select_Pattern();
	void Pattern_Blast();
	void Pattern_Sword();
	void Pattern_Ball();
	void Pattern_Wing();
	void Pattern_Rain();

	void Set_Pattern(bool _bStart) { bPattern = _bStart; }
	void Revive();
	void Move_Rand();

	void Move_Left();
	void Move_Up();
	void Move_Right();
	void Move_Down();
private:
	STATE		m_eCurState;
	STATE		m_ePreState;
	bool		bPattern;
	int			m_iPatternSpeed;

	DWORD		m_dwPatternTime;

	int			m_iBlastCnt;
	int			m_iSwordCnt;
	int			m_iBallCnt;
	int			m_iWingCnt;
	int			m_iRainCnt;
	DWORD		m_dwMoveTime;
	int			m_iMoveNum;
	INFO		m_tPreLocation;

	CObj*		m_pLightSwordRain;
	DWORD		m_dwRainTime;
};

