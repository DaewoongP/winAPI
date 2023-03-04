#pragma once
#include "Enemie.h"
#include "Beam.h"
#include "MusicShot.h"
#include "MusicPattern.h"
#include "SpeakerSpawn.h"
#include "SpawnViolette.h"
class CViolette : public CEnemie
{
private:
	enum STATE { IDLE, MUSIC, ATTACK, HIT, DEAD, STATE_END };
	enum PATTERN { PATTERN_BEAM, PATTERN_SHOT, PATTERN_MUSIC, PATTERN_SPEAKER, PATTERN_END};
	enum MOVE { MOVE_LEFT, MOVE_UP, MOVE_RIGHT, MOVE_DOWN, MOVE_END };
public:
	CViolette();
	virtual ~CViolette();
public:
	virtual void Initialize();
	virtual int Update();
	virtual void Late_Update();
	virtual void Render(HDC hDC);
	virtual void Release();
	virtual void Attack();
	virtual void Motion_Change();
	virtual void Collilsion_Event(CObj* _pObj);
	virtual void Count_Trigger(int _iTriggerCnt);
public:
	void Select_Pattern();
	void Set_Pattern(bool _bStart) { bPattern = _bStart; }
	void Speaker_Spawn();
	void Select_Dead();
	void Move_Rand();

	void Move_Left();
	void Move_Up();
	void Move_Right();
	void Move_Down();

	void Save_Violette();
	bool Save_Event();
private:
	STATE		m_eCurState;
	STATE		m_ePreState;
	bool		bPattern;
	int			m_iPatternSpeed;

	DWORD		m_dwPatternTime;
	DWORD		m_dwMoveTime;
	int			m_iMoveNum;
	INFO		m_tPreLocation;
};

