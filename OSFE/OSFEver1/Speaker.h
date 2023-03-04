#pragma once
#include "Enemie.h"
class CSpeaker : public CEnemie
{
private:
	enum SPEAKERATTACK {SOUNDCIRCLE, SOUNDCONES, SOUNDCROSS, SOUNDEND};
public:
	CSpeaker();
	virtual ~CSpeaker();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Attack() override;
	virtual void Motion_Change() override;
	virtual void Collilsion_Event(CObj * _pObj) override;
	virtual void Count_Trigger(int _iTriggerCnt);
public:
	void Attack_Frame();

	void Circle_Attack();
	void Cone_Attack();
	void Cross_Attack();

	void Circle_Idle();
	void Cone_Idle();
	void Cross_Idle();
private:
	SPEAKERATTACK		m_eCurAttack;
	SPEAKERATTACK		m_ePreAttack;
	FRAME				m_tFrame_Attack;
	TCHAR*				m_pAttackKey;
	int					iRender;
	bool				m_bAttackEnd;

	int					iCnt;
	int					iReleaseCnt;
};

