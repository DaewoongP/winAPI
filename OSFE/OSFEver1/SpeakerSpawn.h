#pragma once
#include "Enemie.h"
#include "Speaker.h"
class CSpeakerSpawn : public CEnemie
{
public:
	CSpeakerSpawn();
	virtual ~CSpeakerSpawn();
public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;
	virtual void Attack() override;
	virtual void Motion_Change() {};
	virtual void Collilsion_Event(CObj * _pObj) {};
	virtual void Count_Trigger(int _iTriggerCnt);
private:
	INFO		m_SpeakerInfo;
};

