#pragma once
#define		PI			3.141592
#define		PURE		= 0
// 윈도우창 X좌표
#define		WINCX		800
#define		WINCY		600
#define		OBJ_NOEVENT 0
#define		OBJ_DEAD	1
#define		PLAYER_DEAD	2
#define		VK_MAX		0xFF
// 비율 8(40):5(25) 타일사이즈
#define			TILECX		64
#define			TILECY		40
// 타일 개수
#define			TILEX		8
#define			TILEY		4
// 타일 인터벌
#define			TILEITV		20
// 타일 초기위치
#define			TILEINIT_X	100
#define			TILEINIT_Y	250

extern	HWND	g_hWnd;

template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

typedef struct tagInfo
{
	float fX;
	float fY;
	float fCX;
	float fCY;

}INFO;

typedef struct tagCreature
{
	int	iHp;
	int iMaxHp;
	float fMp;
	int iMaxMp;
	int iShield;

	int iLevel;
	int iMoney;
	float fExp;
	float fMaxExp;

	tagCreature() {}
	// MaxHp, MaxMp ,money, exp 설정
	tagCreature(int _iMaxHp, int _iMaxMp, int _iMoney, float _fExp)
		:iHp(_iMaxHp), iMaxHp(_iMaxHp), fMp(0.f), iMaxMp(_iMaxMp), iShield(0),
		iMoney(_iMoney), fExp(_fExp), fMaxExp(10.f), iLevel(1){}

}CREATURE;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iFrameCnt;
	int		iMotion;
	int		iMotionEnd;
	int		iMotionCnt;

	int		iImageEnd;

	DWORD	dwSpeed;
	DWORD	dwTime;

}FRAME;

typedef struct tagSpellInfo
{
	// 공격력
	int		iAtk;
	// 사용횟수
	int		iCnt;
	// 소모 마나
	int		iMana;
	// 스펠 가격
	int		iMoney;

	// 실드량
	int		iShield;

	TCHAR*	szDetail;

}SPELLINFO;

class CTag_Finder
{
public:
	CTag_Finder() {}
	CTag_Finder(const TCHAR* _key):m_pKey(_key) {}
	~CTag_Finder() {}

public:
	template<typename T>
	bool operator () (T& pair)
	{
		return !lstrcmp(m_pKey, pair.first);
	}


private:
	const TCHAR*		m_pKey;
};

class CDeleteMap
{
public:
	template<typename T>
	void operator () (T& _MyPair)
	{
		if (_MyPair.second)
		{
			delete _MyPair.second;
			_MyPair.second = nullptr;
		}
	}
};

struct POINTPTR
{
	float*	fpX;
	float*	fpY;
};

enum TILESTATE
{ 
	TILE_IDLE, TILE_ATTACKED, 
	TILE_CRACKED, TILE_BROKEN, 
	TILE_FIRE, TILE_SHIELD, 
	TILE_MUSIC, TILE_HEAL,
	TILE_END
};
enum SCENEID { 
	SC_LOGO, SC_MENU, 
	SC_STAGE1, SC_STAGE2, SC_STAGE3, SC_STAGE4, 
	SC_SERIF, SC_GATE, SC_DEAD, SC_WIN,
	SC_END };
enum OBJID { PLAYER, PET, GATE, TURRET, ENEMIE_SPELL, ENEMIE, SHOP, ITEM, SPELL, TILE, OBJ_END };
enum UIID {UI_BACKGROUND, UI_DAMAGE, UI_REWARD, UI_NEXT, UI_INVENTORY, UI_SHOP, UI_MAP, UI_PLAYER, UI_END};
enum EFFECTID { EFFECT_TILE, AIM, EFFECT_LINE, EFFECT_SPELL, EFFECT_HIT, EFFECT_PARTICLE, EFFECT_END};
enum SPELLID
{
	SPELL_ICENEEDLE,
	SPELL_MINIGUN,
	SPELL_EXPLOSION,
	SPELL_FIRESTORM,
	SPELL_SHIELDCATCH,
	SPELL_THUNDER,
	SPELL_RAGNAROK,
	SPELL_TURRETGUN,
	SPELL_TURRETLASER,
	SPELL_VIOLETTE,
	SPELL_SUNSHINE,
	SPELL_YAMI,
	SPELL_END
};

enum CHANNELID { SOUND_BGM, 
	SOUND_EFFECT00, SOUND_EFFECT01, SOUND_EFFECT02,
	SOUND_EFFECT03, SOUND_EFFECT04, SOUND_EFFECT05,
	SOUND_EFFECT06, SOUND_EFFECT07, SOUND_EFFECT08,
	SOUND_EFFECT09, SOUND_EFFECT10, SOUND_EFFECT11,
	SOUND_EFFECT12, SOUND_EFFECT13, SOUND_EFFECT14,
	SOUND_EFFECT15, SOUND_EFFECT16, SOUND_EFFECT17,
	SOUND_EFFECT18, SOUND_EFFECT19, SOUND_EFFECT20,
	SOUND_EFFECT21, SOUND_EFFECT22, SOUND_EFFECT23,
	SOUND_EFFECT24, SOUND_EFFECT25, SOUND_EFFECT26,
	SOUND_EFFECT27, SOUND_EFFECT28, SOUND_EFFECT29,
	SOUND_EFFECT30, SOUND_EFFECT31,
	MAXCHANNEL };



typedef struct tagSoundInfo
{
	TCHAR*	szName;
	CHANNELID eID;
	float	fVolume;

}SOUNDINFO;