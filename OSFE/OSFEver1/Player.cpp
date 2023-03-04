#include "stdafx.h"
#include "Player.h"
#include "KeyMgr.h"
#include "AbstractFactory.h"
#include "InventoryUI.h"
#include "ShopUI.h"
#include "DamageUI.h"
#include "SceneMgr.h"
#include "BitmapMgr.h"
#include "EffectMgr.h"
#include "SoundMgr.h"

CPlayer::CPlayer()
	:m_iKeyInput(KEY_NULL), m_pAim(nullptr), m_bFinMotion(false), m_pDeck(nullptr), 
	m_pSlot(nullptr), m_bExhaust(false), m_dwExhaustTime(GetTickCount()), m_bSavedVi(false)
{
	ZeroMemory(&m_tFrame_Shield, sizeof(FRAME));
	ZeroMemory(&m_tFrame_Effect, sizeof(FRAME));
}


CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo = { (float)TILEINIT_X + TILECX * 0.5f,
				(float)TILEINIT_Y + TILECY * 0.5f,
				224.f, //224
				208.f }; //208
	m_iTileNum = 0;
	m_preTileNum = m_iTileNum;
	m_tCreature = { 9999, 6, 0, 0};

	m_tPreLocation = m_tInfo;
	m_dwPreDamage = 0;
	m_tAttackInfo = {
		m_tInfo.fX + TILECX * 4,
		m_tInfo.fY,
		15.f,
		15.f };
	// 64, 40�� �����ִ� �� �ϸ� ��������.
	
	m_fSpeed = 8.6f;
	m_eObjID = PLAYER;

	BITMAP->Insert_Bmp(L"../Image/Player/SaffronWitch.bmp", L"PLAYER");
	
	BITMAP->Insert_Bmp(L"../Image/Player/BasicCast.bmp", L"BasicCast");
	BITMAP->Insert_Bmp(L"../Image/Player/ShieldShotS.bmp", L"ShieldShotS");
	// spells
	BITMAP->Insert_Bmp(L"../Image/Spell/inputIconBorder.bmp", L"IconBorder");
	BITMAP->Insert_Bmp(L"../Image/Spell/BasicSpell.bmp", L"BasicSpell");
	BITMAP->Insert_Bmp(L"../Image/Spell/ExplosionMegumin.bmp", L"Explosion");
	BITMAP->Insert_Bmp(L"../Image/Spell/IceNeedleShot.bmp", L"IceNeedleShot");
	BITMAP->Insert_Bmp(L"../Image/Spell/IceNeedleShotRev.bmp", L"IceNeedleShotRev");
	BITMAP->Insert_Bmp(L"../Image/Spell/MinigunBullet.bmp", L"MinigunBullet");
	BITMAP->Insert_Bmp(L"../Image/Spell/PotionShotBlue.bmp", L"PotionShotBlue");
	BITMAP->Insert_Bmp(L"../Image/Spell/Turretgun.bmp", L"Turretgun");
	BITMAP->Insert_Bmp(L"../Image/Spell/Turretlaser.bmp", L"Turretlaser");
	//
	m_pFrameKey = L"PLAYER";
	m_eCurState = IDLE;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 15;
	m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
	m_tFrame.iMotion = 0;
	m_tFrame.iMotionEnd = 1;
	m_tFrame.iMotionCnt = m_tFrame.iMotion;
	m_tFrame.iImageEnd = 8;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount();

	m_tFrame_Shield = {0, 5, 0, 0, 1, 0, 2, 200, GetTickCount()};

	EFFECT->Add_Effect(AIM, CAbstractFactory<CAim>::Create_Effect(m_tInfo.fX, m_tInfo.fY));
	
	if (!m_pDeck)
	{
		m_pDeck = new CDeck;
		m_pDeck->Initialize();
		m_pSlot = m_pDeck->Get_Slot();
	}
	Check_Tile();
	m_iPreTileNum = m_iTileNum;
}

int CPlayer::Update()
{
	m_tCreature.fMp += 0.05f;
	if (m_tCreature.fMp > m_tCreature.iMaxMp)
		m_tCreature.fMp = float(m_tCreature.iMaxMp);

	if (EFFECT->Get_Effect(AIM).empty())
		EFFECT->Add_Effect(AIM, CAbstractFactory<CAim>::Create_Effect(m_tInfo.fX, m_tInfo.fY));

	// Ż�� ����
	if (m_dwExhaustTime + 2500 < GetTickCount())
	{
		m_bExhaust = false;
		m_dwExhaustTime = GetTickCount();
	}

	Level_Up();

	Reset_DamageFont();

	Key_Input();

	Update_AttackInfo();

	m_pDeck->Update();
	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
	if (m_tFrame.iFrameCnt == m_tFrame.iFrameEnd)
	{
		m_eCurState = IDLE;
	}
	if (m_eCurState == ATTACK_END && m_tFrame.iFrameCnt == m_tFrame.iFrameEnd - 1)
	{
		Motion_Change();
		m_eCurState = IDLE;
	}
	else
	{
		Motion_Change();
	}

	if (m_eCurState == ATTACK || m_tFrame_Effect.iFrameCnt < m_tFrame_Effect.iFrameEnd)
		Other_Frame(m_tFrame_Effect);
	Other_Frame(m_tFrame_Shield);
	m_pDeck->Late_Update();
	__super::Move_Frame();
}

void CPlayer::Render(HDC hDC)
{
	// ü��
	CCreature::Render(hDC);
	
	HDC		hMemDC = BITMAP->Find_Img(m_pFrameKey);
	
	GdiTransparentBlt(hDC,		// ���� ���� ���� DC
		int(m_tRect.left), // ���� ���� �ؽ�ó ��ġ ��ǥ X
		int(m_tRect.top), // ���� ���� �ؽ�ó ��ġ ��ǥ Y
		int(m_tInfo.fCX), // �ؽ�ó ���� ������
		int(m_tInfo.fCY), // �ؽ�ó ���� ������
		hMemDC,	// ������ DC
		m_tFrame.iFrameCnt * 140,	// ��Ʈ�� ��� ��ǥ X
		m_tFrame.iMotionCnt * 130,	// ��Ʈ�� ��� ��ǥ Y
		140, // ������ ��Ʈ�� �ؽ�ó ���� ������
		130, // ������ ��Ʈ�� �ؽ�ó ���� ������
		RGB(1, 1, 1)); // ������ �ȼ� ����

	if (m_tFrame_Effect.iFrameCnt < m_tFrame_Effect.iFrameEnd)
	{
		HDC	hEffectDC = BITMAP->Find_Img(L"BasicCast");
		GdiTransparentBlt(hDC,		// ���� ���� ���� DC
			int(m_tInfo.fX + (m_tInfo.fCX * 0.15f)), // ���� ���� �ؽ�ó ��ġ ��ǥ X
			int(m_tInfo.fY - (m_tInfo.fCY * 0.3f)), // ���� ���� �ؽ�ó ��ġ ��ǥ Y
			29, // �ؽ�ó ���� ������
			29, // �ؽ�ó ���� ������
			hEffectDC,	// ������ DC
			m_tFrame_Effect.iFrameCnt * 18,	// ��Ʈ�� ��� ��ǥ X
			m_tFrame_Effect.iMotionCnt * 18,	// ��Ʈ�� ��� ��ǥ Y
			18, // ������ ��Ʈ�� �ؽ�ó ���� ������
			18, // ������ ��Ʈ�� �ؽ�ó ���� ������
			RGB(1, 1, 1)); // ������ �ȼ� ����
	}

	if (0 < m_tCreature.iShield)
	{
		HDC		hMemDC = BITMAP->Find_Img(L"ShieldShotS");
		GdiTransparentBlt(hDC,
			m_tRect.left,
			m_tRect.top,
			int(m_tInfo.fCX), // Ű�� ������
			int(m_tInfo.fCY),
			hMemDC,
			m_tFrame_Shield.iFrameCnt * 120,
			m_tFrame_Shield.iMotionCnt * 120,
			120,		// �ؽ�ó ���� ������
			120,
			RGB(1, 1, 1));
	}

	for (auto& iter : m_DamageList)
	{
		iter->Render_Damage(hDC);
	}

	m_pDeck->Render(hDC);
	Render_Spell_Slot(hDC);	
}

void CPlayer::Release()
{
	SCENE->Get_Scene()->Delete_Object(PET);
	Safe_Delete<CDeck*>(m_pDeck);
	EFFECT->Destroy_Instance();
	KEY->Destroy_Instance();

}

void CPlayer::Collilsion_Event(CObj* _pObj)
{
	if (_pObj->Get_OBJID() == ENEMIE || _pObj->Get_OBJID() == ENEMIE_SPELL)
	{
		// �ǵ尡 0�̻��̸�
		if (0 < m_tCreature.iShield)
		{
			// �ǵ� ���
			m_tCreature.iShield -= dynamic_cast<CEnemie*>(_pObj)->Get_Attack();
			// �ǵ� ���� �� ����� hp�� ��� �ǵ� 0���� �ʱ�ȭ
			if (0 > m_tCreature.iShield)
			{
				m_tCreature.iHp += m_tCreature.iShield;
				m_tCreature.iShield = 0;
			}
		}
		// or hp��
		else
			m_tCreature.iHp -= dynamic_cast<CEnemie*>(_pObj)->Get_Attack();
		m_eCurState = HIT;
		m_DamageList.push_back(CAbstractFactory<CDamageUI>::Create_DamageUI(m_tInfo.fX - 50, m_tInfo.fY - 100 - m_iDamageY, static_cast<CEnemie*>(_pObj)->Get_Attack()));
		Set_DamageY(20);
		SOUND->PlaySound(L"enemy_hit_light.wav", SOUND_EFFECT12, 0.3f);
		EFFECT->Add_Effect(EFFECT_HIT, CAbstractFactory<CHitEffect>::Create_Effect(m_tInfo.fX,	m_tInfo.fY));
		SCENE->Get_Scene()->Set_Hit(15.f);
	}
}

void CPlayer::Count_Trigger(int _iTriggerCnt)
{
}

void CPlayer::Update_AttackInfo()
{
	m_tAttackInfo = {
		m_tInfo.fX + (TILECX + TILEITV) * 4,
		m_tInfo.fY,
		15.f,
		15.f };
	m_tAttackRect = {
		long(m_tAttackInfo.fX - m_tAttackInfo.fCX * 0.5f),
		long(m_tAttackInfo.fY - m_tAttackInfo.fCY * 0.5f),
		long(m_tAttackInfo.fX + m_tAttackInfo.fCX * 0.5f),
		long(m_tAttackInfo.fY + m_tAttackInfo.fCY * 0.5f) };
}

void CPlayer::Motion_Change()
{		
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.iImageEnd = 8;
			m_tFrame.iMotion = 0;
			m_tFrame.iMotionEnd = 1;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case RMOVE:
			m_tFrame.iFrameStart = 2;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iImageEnd = 8;
			m_tFrame.iMotion = 3;
			m_tFrame.iMotionEnd = 3;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case LMOVE:
			m_tFrame.iFrameStart = 6;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iImageEnd = 8;
			m_tFrame.iMotion = 3;
			m_tFrame.iMotionEnd = 3;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 15;
			m_tFrame.iImageEnd = 8;
			m_tFrame.iMotion = 6;
			m_tFrame.iMotionEnd = 7;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			// ����Ʈ
			m_tFrame_Effect.iFrameStart = 0;
			m_tFrame_Effect.iFrameEnd = 2;
			m_tFrame_Effect.iImageEnd = 2;
			m_tFrame_Effect.iMotion = 0;
			m_tFrame_Effect.iMotionEnd = 1;
			m_tFrame_Effect.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame_Effect.iMotionCnt = m_tFrame.iMotion;
			m_tFrame_Effect.dwSpeed = 50;
			m_tFrame_Effect.dwTime = GetTickCount();
			break;
		// ����Ű ���� ���
		case ATTACK_END:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iImageEnd = 8;
			m_tFrame.iMotion = 2;
			m_tFrame.iMotionEnd = 3;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			break;

		case HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iImageEnd = 8;
			m_tFrame.iMotion = 5;
			m_tFrame.iMotionEnd = 5;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 50;
			m_tFrame.dwTime = GetTickCount();
			break;
		case EXHAUST:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iImageEnd = 7;
			m_tFrame.iMotion = 4;
			m_tFrame.iMotionEnd = 4;
			m_tFrame.iFrameCnt = m_tFrame.iFrameStart;
			m_tFrame.iMotionCnt = m_tFrame.iMotion;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CPlayer::Key_Input()
{
	if (m_bExhaust)
		return;
	// ����Ű ����
	Key_Left();
	Key_Up();
	Key_Right();
	Key_Down();
	
	if (KEY->Key_Pressing('E'))
	{
		m_eCurState = ATTACK;
		if (m_dwTime + 100 < GetTickCount())
		{
			SCENE->Get_Scene()->Add_Object(SPELL, CAbstractFactory<CBracer>::Create(m_iTileNum));
			m_dwTime = GetTickCount();
		}
	}
	if (KEY->Key_Up('E'))
	{
		m_eCurState = ATTACK_END;
	}
	if (KEY->Key_Down('R'))
	{
		m_pDeck->Reload_Deck();
	}
	if (KEY->Key_Down('Q'))
	{
		Use_SlotQ();
	}
	if (KEY->Key_Down('W'))
	{
		Use_SlotW();
	}
	if (KEY->Key_Up('Q') || KEY->Key_Up('W'))
		m_eCurState = ATTACK_END;

	if (KEY->Key_Down('I'))
	{
		Open_Inventory();
	}
}


void CPlayer::Key_Left()
{
	if (m_iKeyInput == KEY_LEFT)
	{
		m_tInfo.fX -= m_fSpeed;
		// �Ʊ� Ÿ�� ������ϸ� �̵�����
		if (m_tInfo.fX <= (float)(TILEINIT_X + TILECX * 0.5f) - m_fSpeed)
		{
			m_tInfo = m_tPreLocation;
			m_iKeyInput = KEY_NULL;
			m_eCurState = IDLE;
		}
		else
		{
			// �ڿ������� �̵���Ű��.
			if (m_tInfo.fX < m_tPreLocation.fX - (TILECX + TILEITV))
			{
				// ��ġ ��Ȯ�Ѱ��� ����
				m_tInfo.fX = m_tPreLocation.fX - (TILECX + TILEITV);
				m_tPreLocation = m_tInfo;
				m_iPreTileNum = m_iTileNum;
				m_iKeyInput = KEY_NULL;
				m_eCurState = IDLE;
				m_iTileNum -= 1;
				
			}
			else
			{
				// ��Ǻ���
				m_eCurState = LMOVE;
			}
		}
	}
	else if (m_iKeyInput == KEY_NULL)
	{
		// Ű�Է� Ȱ��ȭ, ������ġ ����
		if (KEY->Key_Down(VK_LEFT))
		{
			m_iKeyInput = KEY_LEFT;
			m_tPreLocation = m_tInfo;
		}
	}
}

void CPlayer::Key_Up()
{
	if (m_iKeyInput == KEY_UP)
	{
		m_tInfo.fY -= m_fSpeed;
		// �Ʊ� Ÿ�� ������ϸ� �̵�����
		if (m_tInfo.fY <= (float)(TILEINIT_Y + TILECY * 0.5f) - m_fSpeed)
		{
			m_tInfo = m_tPreLocation;
			m_iKeyInput = KEY_NULL;
			m_eCurState = IDLE;
		}
		else
		{
			// �ڿ������� �̵���Ű��.
			if (m_tInfo.fY < m_tPreLocation.fY - (TILECY + TILEITV))
			{
				m_tInfo.fY = m_tPreLocation.fY - (TILECY + TILEITV);
				m_tPreLocation = m_tInfo;
				m_iPreTileNum = m_iTileNum;
				m_iKeyInput = KEY_NULL;
				m_eCurState = IDLE;
				m_iTileNum -= 8;
				
			}
			else
			{
				// ��Ǻ���
				m_eCurState = IDLE;
			}
		}
	}
	else if (m_iKeyInput == KEY_NULL)
	{
		// Ű�Է� Ȱ��ȭ, ������ġ ����
		if (KEY->Key_Down(VK_UP))
		{
			m_iKeyInput = KEY_UP;
			m_tPreLocation = m_tInfo;
		}
	}
}

void CPlayer::Key_Right()
{
	if (m_iKeyInput == KEY_RIGHT)
	{
		m_tInfo.fX += m_fSpeed;
		// �Ʊ� Ÿ�� ������ϸ� �̵�����
		if (m_tInfo.fX >= (float)(TILEINIT_X + TILECX * 0.5f) + (float)(TILECX + TILEITV) * 3.f + m_fSpeed)
		{
			//m_tInfo.fX -= TILECX + TILEITV;
			m_tInfo = m_tPreLocation;
			m_iKeyInput = KEY_NULL;
			m_eCurState = IDLE;
		}
		else
		{
			// �ڿ������� �̵���Ű��.
			if (m_tInfo.fX > m_tPreLocation.fX + TILECX + TILEITV)
			{
				m_tInfo.fX = m_tPreLocation.fX + TILECX + TILEITV;
				m_tPreLocation = m_tInfo;
				m_iPreTileNum = m_iTileNum;
				m_iKeyInput = KEY_NULL;
				m_eCurState = IDLE;
				m_iTileNum += 1;
			}
			else
			{
				// ��Ǻ���
				m_eCurState = RMOVE;
			}
		}
	}
	else if (m_iKeyInput == KEY_NULL)
	{
		// Ű�Է� Ȱ��ȭ, ������ġ ����
		if (KEY->Key_Down(VK_RIGHT))
		{
			m_iKeyInput = KEY_RIGHT;
			m_tPreLocation = m_tInfo;
		}
	}
}

void CPlayer::Key_Down()
{
	if (m_iKeyInput == KEY_DOWN)
	{
		m_tInfo.fY += m_fSpeed;
		// �Ʊ� Ÿ�� ������ϸ� �̵�����
		if (m_tInfo.fY >= (float)(TILEINIT_Y + TILECY * 0.5f) + (float)(TILECY + TILEITV) * 3.f + m_fSpeed)
		{
			m_tInfo = m_tPreLocation;
			m_iKeyInput = KEY_NULL;
			m_eCurState = IDLE;
		}
		else
		{
			// �ڿ������� �̵���Ű��.
			if (m_tInfo.fY > m_tPreLocation.fY + TILECY + TILEITV)
			{
				m_tInfo.fY = m_tPreLocation.fY + TILECY + TILEITV;
				m_tPreLocation = m_tInfo;
				m_iPreTileNum = m_iTileNum;
				m_iKeyInput = KEY_NULL;
				m_eCurState = IDLE;
				m_iTileNum += 8;
			}
			else
			{
				// ��Ǻ���
				m_eCurState = IDLE;
			}
		}
	}
	else if (m_iKeyInput == KEY_NULL)
	{
		// Ű�Է� Ȱ��ȭ, ������ġ ����
		if (KEY->Key_Down(VK_DOWN))
		{
			m_iKeyInput = KEY_DOWN;
			m_tPreLocation = m_tInfo;
		}
	}
}

void CPlayer::Other_Frame(FRAME& _frame)
{
	if (_frame.dwTime + _frame.dwSpeed < GetTickCount())
	{
		++_frame.iFrameCnt;
		// �̹��� ���� ������� �������� ������� �׳� ȸ��.
		if (_frame.iFrameEnd < _frame.iImageEnd)
		{
			if (_frame.iFrameCnt >= _frame.iFrameEnd)
			{
				_frame.iFrameCnt = _frame.iFrameStart;
				_frame.iMotionCnt = _frame.iMotion;
				_frame.iFrameEnd += _frame.iImageEnd * (_frame.iMotionEnd - _frame.iMotion);
			}
		}
		else
		{	// ���ΰ� ���庸�� Ŀ���� �ʱ�ȭ
			if (_frame.iMotionCnt > _frame.iMotionEnd)
			{
				_frame.iMotionCnt = _frame.iMotion;
			}
			else
			{
				// �̹��� ���� ���� �����ϸ� �ʱ�ȭ
				if (_frame.iFrameCnt > _frame.iImageEnd)
				{
					_frame.iFrameCnt = _frame.iFrameStart;
					++_frame.iMotionCnt;
					_frame.iFrameEnd -= _frame.iImageEnd;
				}
			}
		}

		_frame.dwTime = GetTickCount();
	}
}

void CPlayer::Use_SlotQ()
{
	// ��������� �������ϴ°� ��������.
	if (m_pSlot->Get_Slot().front() != nullptr)
	{
		int SpellMp = m_pSlot->Get_Slot().front()->Get_SpellInfo().iMana;
		if (SpellMp > m_tCreature.fMp)
			return;
		else
			m_tCreature.fMp -= SpellMp;
		SCENE->Get_Scene()->Add_Object(SPELL, m_pSlot->Get_Slot().front());
		m_pSlot->Get_Slot().front()->Set_PlayerPos(m_iTileNum);
		m_pSlot->Get_Slot().front()->Player_Effect();
		SOUND->PlaySound(m_pSlot->Get_Slot().front()->Get_SoundInfo().szName,
			m_pSlot->Get_Slot().front()->Get_SoundInfo().eID,
			m_pSlot->Get_Slot().front()->Get_SoundInfo().fVolume);
		if (m_pSlot->Get_Slot().front()->Get_SpellID() == SPELL_VIOLETTE)
			m_pDeck->Delete_Spell(SPELL_VIOLETTE);
		m_pSlot->Set_SlotQ(nullptr);
		m_eCurState = ATTACK;
		if (!m_pDeck->Get_Deck().empty())
		{
			m_pDeck->Set_DeckY(419.f);
			m_pDeck->Push_SlotQ();
		}
	}
}

void CPlayer::Use_SlotW()
{
	if (m_pSlot->Get_Slot().back() != nullptr)
	{
		int SpellMp = m_pSlot->Get_Slot().back()->Get_SpellInfo().iMana;
		if (SpellMp > m_tCreature.fMp)
			return;
		else
			m_tCreature.fMp -= SpellMp;

		SCENE->Get_Scene()->Add_Object(SPELL, m_pSlot->Get_Slot().back());
		m_pSlot->Get_Slot().back()->Set_PlayerPos(m_iTileNum);
		m_pSlot->Get_Slot().back()->Player_Effect();
		SOUND->PlaySound(m_pSlot->Get_Slot().back()->Get_SoundInfo().szName,
			m_pSlot->Get_Slot().back()->Get_SoundInfo().eID,
			m_pSlot->Get_Slot().back()->Get_SoundInfo().fVolume);
		if (m_pSlot->Get_Slot().back()->Get_SpellID() == SPELL_VIOLETTE)
			m_pDeck->Delete_Spell(SPELL_VIOLETTE);
		m_pSlot->Set_SlotW(nullptr);
		m_eCurState = ATTACK;
		if (!m_pDeck->Get_Deck().empty())
		{
			m_pDeck->Set_DeckY(419.f);
			m_pDeck->Push_SlotW();
		}
	}
}

void CPlayer::Open_Inventory()
{
	CScene* CurScene = SCENE->Get_Scene();

	if (!CurScene->Get_ObjList(SHOP).empty())
	{
		if (!dynamic_cast<CShopKeeper*>(CurScene->Get_ShopKeeper())->IsShopOpen())
		{
			dynamic_cast<CShopKeeper*>(CurScene->Get_ShopKeeper())->Set_ShopOpen();

			CurScene->AddUI(UI_INVENTORY, CAbstractFactory<CInventoryUI>::Create_UI());
			CurScene->AddUI(UI_SHOP, CAbstractFactory<CShopUI>::Create_UI());
		}
		else
		{
			dynamic_cast<CShopKeeper*>(CurScene->Get_ShopKeeper())->Set_ShopClose();
			CurScene->DeleteUI(UI_INVENTORY);
			CurScene->DeleteUI(UI_SHOP);
		}
	}
	else
	{
		if (CurScene->Get_ObjList(ENEMIE).empty() && CurScene->Get_UIList(UI_INVENTORY).empty())
		{
			CurScene->AddUI(UI_INVENTORY, CAbstractFactory<CInventoryUI>::Create_UI());
			// �ؿ� �Ȱɸ��� ����
			return;
		}
		if (!CurScene->Get_UIList(UI_INVENTORY).empty())
		{
			CurScene->DeleteUI(UI_INVENTORY);
			return;
		}
	}
}

void CPlayer::Render_Spell_Slot(HDC hDC)
{
	HDC		hMemDC = BITMAP->Find_Img(L"IconBorder");

	if (m_pSlot->Get_Slot().front() != nullptr)
		m_pSlot->Get_Slot().front()->Render_SpellIcon(hDC, int(m_tInfo.fX - 32), int(m_tInfo.fY - 100), 25, 25);
	else
	{
		GdiTransparentBlt(hDC,
			int(m_tInfo.fX - 32),
			int(m_tInfo.fY - 100),
			25,
			25,
			hMemDC,
			0,
			0,
			256,		// �ؽ�ó ���� ������
			256,
			RGB(1, 1, 1));
	}
	if (m_pSlot->Get_Slot().back() != nullptr)
		m_pSlot->Get_Slot().back()->Render_SpellIcon(hDC, int(m_tInfo.fX + 8), int(m_tInfo.fY - 100), 25, 25);
	else
	{
		GdiTransparentBlt(hDC,
			int(m_tInfo.fX + 8),
			int(m_tInfo.fY - 100),
			25,
			25,
			hMemDC,
			0,
			0,
			256,		// �ؽ�ó ���� ������
			256,
			RGB(1, 1, 1));
	}
}

void CPlayer::Render_Spell_Use(HDC hDC, CSpell* _Spell)
{
	_Spell->Render_SpellIcon(hDC, int(m_tInfo.fX - 50), int(m_tInfo.fY - 125), 25, 25);
}

void CPlayer::Level_Up()
{
	if (m_tCreature.fExp > m_tCreature.fMaxExp)
	{
		// ������
		m_tCreature.fExp -= m_tCreature.fMaxExp;
		++m_tCreature.iLevel;
		// ��Ƽ��Ʈ â �߰�
	}
}

