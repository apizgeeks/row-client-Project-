// CharStatus.cpp: implementation of the CCharStatus class.
//
//////////////////////////////////////////////////////////////////////

#include <Skill/SkillMgr.h>
#include <Skill/Spell/SpellKind.h>

#include <Creature/Character/CharacterClass.h>

#include <Item/ItemMgr.h>
#include <DB/DBdefine.h>

#include "DataTable.h"
#include "SoundMgr.h"

#include "RYLStringTable.h"
#include "RYLRaceBase.h"

#include "RYLCreatureManager.h"
#include "RYLCharacterDataManager.h"
#include "RYLObjectControl.h"
#include "RYLMessageBox.h"

#include "RYLGameData.h"
#include "RYLNetWorkData.h"
#include "RYLClientMain.h"
#include "RYLSceneManager.h"
#include "RYLGameScene.h"

#include "RYLHumanInventoryFrameDlg.h"
#include "RYLHumanInventoryPanel.h"
#include "RYLStatusAndQuestFrameDlg.h"
#include "RYLQuestDetailDataDlg.h"
#include "RYLExecutionQuestPanel.h"
#include "RYLSkillPanel.h"
#include "RYLAbilityPanel.h"
#include "RYLSocialActionPanel.h"
#include "RYLChattingDlg.h"


#include "CharStatus.h"
#include "GMMemory.h"

CDataTable g_dtDateTable;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharStatus::CCharStatus()
{
	int i, j, k;

	for (k = 0; k < INVENTORY_TAB; ++k)
	{
		for (j = 0; j < INVENTORY_HEIGHT; ++j)
		{
			for (i = 0; i < INVENTORY_WIDTH; ++i)
			{
				m_aryInven[k][j][i] = ITEM_EMPTY;
			}
		}
	}

	for (j = 0; j < 4; ++j)
	{
		for (i = 0; i < 8; ++i)
		{
			m_aryExchange[j][i] = ITEM_EMPTY;
			m_aryExchange2[j][i] = ITEM_EMPTY;
		}
	}

	for (k = 0; k < 4; ++k)
	{
		for (j = 0; j < 12; ++j)
		{
			for (i = 0; i < 8; ++i)
			{
				m_aryDeposit[k][j][i] = ITEM_EMPTY;
			}
		}
	}

	for (j = 0; j < 8; ++j)
	{
		for (i = 0; i < 10; ++i)
		{
			m_aryStreetTrade[j][i] = ITEM_EMPTY;
		}
	}

	m_dwExchangeGold = 0;
	m_dwExchange2Gold = 0;

	m_dwDepositGold = 0;
	m_cSavePass = 0;
	m_dwMaxTab = 0;
	m_dwDepositFlag = 0;
	m_cUsedDeposit = 0;

	m_dwMyTotalRanking = 0;
	m_dwMyClassRanking = 0;

	for (i = 0; i < Item::EquipmentPos::MAX_EQUPMENT_POS; ++i)
	{
		m_lstEquipment[i] = NULL;
	}

	for (i = 0; i < SKILL::MAX_SLOT_NUM; ++i)
	{
		m_lstSkillSlot[i] = NULL;
	}

	for (i = 0; i < QUICK::MAX_QUICK_NUM; ++i)
	{
		m_lstQuickSlot[i] = NULL;
		m_bQuickSlotLength[i] = FALSE;
	}

	for (i = 0; i < Skill::SpellID::MAX_SPELL_TYPE; ++i)
	{
		m_aryEnchantLevel[i] = 0;
		m_aryEnchantTime[i] = 0;
	}

	m_lpQuickSelected = NULL;

	m_PartyInfo.m_cMemberNum = 0;
	m_PartyInfo.m_dwPartyID = 0;	
	for (i = 0; i < PARTY::MAX_MEM; ++i)
	{
		m_PartyInfo.MemberCID[i] = 0;        
	}

	m_wUseChantSkillID	= 0 ;
	m_bSummonGuard	= FALSE;

	m_lpUpgrade[0] 	= NULL;
	m_lpUpgrade[1] 	= NULL;
	m_bUpgrading	= FALSE;

	m_bUseStealth	= TRUE;
	m_Info.Class 	= 0;
	m_bRangedWeapon	= FALSE;
	m_cWeaponPos 	= 0;
	m_cRidePos		= 0;

	m_lpSiegeArmsMaterial		= NULL;
	m_lpSiegeArmsSkill	  		= NULL;
	m_lpTmpQuickSkill	  		= NULL;

	m_lpCompositionOrigin		= NULL;
	m_lpCompositionOffering		= NULL;
	m_lpCompositionResult		= NULL;
	m_iCompositionResult		= -1;
	m_lpCompositionTempResult	= NULL;

	m_lpCompensation			= NULL;
	m_lpCompensationResult		= NULL;

	m_dwTempInventoryTime		= 0;

	m_MaxHP		= 0;
	m_MaxMP		= 0;

	m_lAddMaxHP	= 0;
	m_lAddMaxMP	= 0;

	m_iAbilityPoint = 0;
	m_iUseAbilityPoint = 0;

	m_iLuck = 0;
	m_fLuckResistRate = 0.0f;

	memset(m_AbilityValue, 0, sizeof(m_AbilityValue));

	InitTestItemInfo();
}

CCharStatus::~CCharStatus()
{
	list<CItemInstance *>::iterator it;
	CItemInstance *lpItem;
	for (it = m_lstInventory.begin(); it != m_lstInventory.end();)
	{
		lpItem = (*it);
		it = m_lstInventory.erase(it);
		delete lpItem;
	}
	m_lstInventory.clear();

	for (it = m_lstDeposit.begin(); it != m_lstDeposit.end();)
	{
		lpItem = (*it);
		it = m_lstDeposit.erase(it);
		delete lpItem;
	}
	m_lstDeposit.clear();

	for (it = m_lstStreetTrade.begin(); it != m_lstStreetTrade.end();)
	{
		lpItem = (*it);
		it = m_lstStreetTrade.erase(it);
		delete lpItem;
	}
	m_lstStreetTrade.clear();

	list<FriendInfo *>::iterator itFriend;
	FriendInfo *lpFriend;
	for (itFriend = m_lstFriendList.begin(); itFriend != m_lstFriendList.end();)
	{
		lpFriend = (*itFriend);
		itFriend = m_lstFriendList.erase(itFriend);
		delete lpFriend;
	}
	m_lstFriendList.clear();

	list<BanInfo *>::iterator itBan;
	BanInfo *lpBan;
	for (itBan = m_lstBanList.begin(); itBan != m_lstBanList.end();)
	{
		lpBan = (*itBan);
		itBan = m_lstBanList.erase(itBan);
		delete lpBan;
	}
	m_lstBanList.clear();

	vector<RankingNode *>::iterator itRank;
	RankingNode *lpRank;
	for (itRank = m_lstRanking.begin(); itRank != m_lstRanking.end();)
	{
		lpRank = (*itRank);
		itRank = m_lstRanking.erase(itRank);
		delete lpRank;
	}
	m_lstRanking.clear();

	list<LPQuestInstance>::iterator itQuest;
	LPQuestInstance lpQuest;
	for (itQuest = m_lstQuest.begin(); itQuest != m_lstQuest.end();)
	{
		lpQuest = (*itQuest);
		itQuest = m_lstQuest.erase(itQuest);
		delete lpQuest->m_lpQuestIcon;
		delete lpQuest;
	}
	m_lstQuest.clear();

	for (itQuest = m_lstCompleteQuest.begin(); itQuest != m_lstCompleteQuest.end();)
	{
		lpQuest = (*itQuest);
		itQuest = m_lstQuest.erase(itQuest);
		delete lpQuest->m_lpQuestIcon;
		delete lpQuest;
	}
	m_lstCompleteQuest.clear();

	for (int i = 0; i < QUICK::MAX_QUICK_NUM; ++i)
	{
		if (m_lstQuickSlot[i])
		{
			delete m_lstQuickSlot[i];
			m_lstQuickSlot[i] = NULL;
		}
	}
	m_lpQuickSelected = NULL;

	for (int i = 0; i < SKILL::MAX_SLOT_NUM; ++i)
	{
		if (m_lstSkillSlot[i])
		{
			delete m_lstSkillSlot[i];
			m_lstSkillSlot[i] = NULL;
		}
	}

	vector<CItemInstance *>::iterator itSkill;
	for (itSkill = m_lstClassSkill.begin(); itSkill != m_lstClassSkill.end();)
	{
		lpItem = (*itSkill);
		itSkill = m_lstClassSkill.erase(itSkill);
		delete lpItem;
		lpItem = NULL ; 
	}
	m_lstClassSkill.clear();

	for (int i = 0; i < 15; ++i)
	{
		if (m_lstEquipment[i])
		{
			delete m_lstEquipment[i];
			m_lstEquipment[i] = NULL;
		}
	}

	GM_DELETE( m_lpUpgrade[0] );
	GM_DELETE( m_lpUpgrade[1] ); 

	GM_DELETE( m_lpCompositionOrigin );
	GM_DELETE( m_lpCompositionOffering );
	GM_DELETE( m_lpCompositionResult );
	GM_DELETE( m_lpCompositionTempResult );

	GM_DELETE( m_lpCompensation );
	GM_DELETE( m_lpCompensationResult );


	vector<CItemInstance *>::iterator iter;
	for ( iter = m_lstTempInventoryItem.begin(); iter != m_lstTempInventoryItem.end(); iter++ )
	{
		lpItem = (*iter);
		delete lpItem ;
	}
	m_lstTempInventoryItem.clear();
}

bool CCharStatus::CalculateAbility(const SKILL& skill)
{
	memset(m_AbilityValue, 0, sizeof(m_AbilityValue));

	// 어빌리티 스킬 효과
	for (unsigned char cSlotIndex = 0; cSlotIndex < skill.wSlotNum; ++cSlotIndex) 
	{
		const SKILLSLOT& SkillSlot = skill.SSlot[cSlotIndex];

		unsigned short wSkillID = SkillSlot.SKILLINFO.wSkill;

		// 앨터너티브 스킬의 경우
		const Skill::ProtoType* lpProtoType = CSkillMgr::GetInstance().GetSkillProtoType(wSkillID);
		if (NULL == lpProtoType)
			continue;

		if(lpProtoType->m_eSkillType != Skill::Type::ABILITY)
			continue;

		int cLock = SkillSlot.SKILLINFO.cLockCount;
	
		int id = wSkillID-0x1001;
		m_AbilityValue[Skill::Type::AB_ANIMAL_TALK+id] = static_cast<unsigned short>(lpProtoType[cLock].m_fMaxRange);
/*
			AB_ANIMAL_TALK		= 0,	// 동물NPC와 대화
			AB_SOCIALEX			= 1,	// 확장 소셜모션 사용가능
			AB_NOFIRSTATK		= 2,	// 선공몹 공격 안당함
			AB_MOUNT_SPEEDUP	= 3,	// 탈것 이속 증가
			AB_TEMPINVEN_TIMEUP = 4,	// 임시인벤토리 시간증가
			AB_GETUP_GOLD		= 5,	// 골드 획득량 증가
			AB_RESPAWN_EX		= 6,	// 확장 리스폰 사용 
			AB_FOOD_UP			= 7,	// 음식류 능력치 증가
			AB_SHOP_MEMBERSHIP	= 8,	// 상점 멤버십(구매가격 하락, 판매가격 상승)
			AB_CHEMICAL			= 9,	// 잡템을 회복템으로 변경
			AB_ENDUR_SHILD		= 10,	// 내구도 하락 감소
			AB_RECOVERY_UP		= 11,	// HP,MP 회복도 증가
			AB_AWARD_UP			= 12,	// 퀘스트 보상 증가
			AB_MYEQUIP_UP		= 13,	// 자신의 장비 획득율 증가
			AB_LUCK_UP			= 14,	// 럭찬 증가
			AB_EQUIP_LEVELDOWN	= 15,	// 장비의 제한 레벨을 다운시킨다.
*/
	}
	return true;
}

void CCharStatus::StatusUpdate( int type, bool bRight, bool bPassive )
{
	CharacterStatus	status;
	status.Init( m_Info );

	bool isRideSlot = ( m_cRidePos == 0 ) ? false : true;

	switch ( type )
	{
		case STATUS_UPDATE_BASE:
		{
			m_statInfoBase.m_cCalculateState = FightStatus::CS_NONE;
			m_statInfoBase.CalculateBaseInfo( status, m_Info.Level, m_Info.Class );

			CalculateAbility(m_Skill);

			// 리젠값을 증가시켜준다.
			if(GetAbilityValue(Skill::Type::AB_RECOVERY_UP))
			{
				int Regen = GetAbilityValue(Skill::Type::AB_RECOVERY_UP);

				m_statInfoBase.m_wHPRegen += Regen;
				m_statInfoBase.m_wMPRegen += Regen;
			}
		}

		case STATUS_UPDATE_EQUIP:
		{
			if ( FightStatus::CS_BASE_INFO != m_statInfoBase.m_cCalculateState )
			{
				StatusUpdate(STATUS_UPDATE_BASE, true, true);
				return;
			}
			const Item::CEquipment*	arrEquip[Item::EquipmentPos::MAX_EQUPMENT_POS];
			for ( int i = 0 ; i < Item::EquipmentPos::MAX_EQUPMENT_POS; ++i )
			{
				if(!isRideSlot && i == Item::EquipmentPos::AVATA)	// 라이더
				{
					arrEquip[i] = NULL;
					continue;				
				}
				arrEquip[i] = ( m_lstEquipment[i] ) ? Item::CEquipment::DowncastToEquipment( m_lstEquipment[i]->m_lpItemBase ) : NULL ;
			}

			bool isFirstSlot = ( m_cWeaponPos == 0 ) ? true : false;
            if ( !bRight && !arrEquip[ Item::EquipmentPos::SKILL_ARM ] )
            {
                m_statInfoBase.m_cComboCount = 4;
			}

			m_statInfoBase.CalculateEquipInfo( arrEquip , isFirstSlot, bRight, isRideSlot,
				( bPassive ) ? m_Skill : SKILL(), m_Info.Level, GetAbilityValue(Skill::Type::AB_MOUNT_SPEEDUP), GetAbilityValue(Skill::Type::AB_LUCK_UP), GetAbilityValue(Skill::Type::AB_EQUIP_LEVELDOWN), status, m_Info.Class, m_statInfoEquip, m_iLuck);			

			m_fLuckResistRate = (float)m_iLuck/3.0f;
		}

		case STATUS_UPDATE_ENCHANT:
		{
			if ( FightStatus::CS_EQUIP_INFO != m_statInfoEquip.m_cCalculateState )
			{
				StatusUpdate(STATUS_UPDATE_EQUIP, true, true);
				return;
			}

			m_statInfoEquip.CalculateEnchantInfo( m_aryEnchantLevel, m_statInfoEnchant );
				
		}

        default:
        {
			// 어빌리티 포인트는 100프로 퀘스트로만 받는다
			m_iAbilityPoint = 0;

            // 퀘스트 수행으로 받은 보너스 스킬 포인트를 책정.
            for ( list<LPQuestInstance>::iterator itQuest = m_lstCompleteQuest.begin() ; itQuest != m_lstCompleteQuest.end() ; ++itQuest )    
            {
                if ( (*itQuest)->m_lpQuestScript->m_usBonusSkillPoint != 0 )
                    m_statInfoEnchant.m_wSkillPoint += (*itQuest)->m_lpQuestScript->m_usBonusSkillPoint;

                if ( (*itQuest)->m_lpQuestScript->m_usBonusAbilityPoint != 0 )
					m_iAbilityPoint += (*itQuest)->m_lpQuestScript->m_usBonusAbilityPoint;
            }
        }
		break;
	}

    m_MaxHP = m_statInfoEnchant.m_nMaxHP ;
    m_MaxMP = m_statInfoEnchant.m_nMaxMP ;

	m_llExpMax = g_dtDateTable.MAXEXPTABLE[m_Info.Level];

	float fSpeedFactor = GetSpeedRate() / 100.0f;
	g_CharacterData.m_fWalkSpeed = 150.0f * fSpeedFactor;
	g_CharacterData.m_fRunSpeed = 490.0f * fSpeedFactor;
	g_CharacterData.m_dwMaxCombo = m_statInfoEnchant.m_cComboCount;

    RYLCreature*    pSelfCreature = RYLCreatureManager::Instance()->GetSelfCreature();
    if ( pSelfCreature )
	{
		CharDataNode*   pSelfData = RYLCharacterDataManager::Instance()->GetCharData( pSelfCreature->GetCharID() );
		if ( pSelfData )
		{
			pSelfData->m_ulMaxHP = m_statInfoEnchant.m_nMaxHP ;
			pSelfData->m_ulMaxMP = m_statInfoEnchant.m_nMaxMP ;
		}

		for (int i = 0; i < Item::EquipmentPos::MAX_EQUPMENT_POS; ++i)
		{
			if (m_lstEquipment[i])
			{
				if (CheckItemByStatus(m_lstEquipment[i]))
				{
					m_lstEquipment[i]->m_bCanUsed = TRUE;
				} 
				else
				{
					m_lstEquipment[i]->m_bCanUsed = FALSE;
				}
			}
		}

		if (m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos])
		{
			Item::CEquipment *lpEquip = Item::CEquipment::DowncastToEquipment(m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos]->m_lpItemBase);
			if (lpEquip)
			{
				unsigned long dwType = m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos]->GetItemType();
				if (dwType == Item::ItemType::BOW || dwType == Item::ItemType::CROSSBOW)
				{
					m_bRangedWeapon = TRUE;
				} 
				else
				{
					m_bRangedWeapon = FALSE;
				}
			}
		} 
		else
		{
			g_CharacterData.m_dwMaxCombo = 0;
			m_bRangedWeapon = FALSE;
		}

		// 최대 HP/MP가 줄어드는 경우 현재 HP/MP도 조정해준다.
		CRYLGameData* pGame = CRYLGameData::Instance();
		if (pGame->m_csStatus.m_Info.HP > m_MaxHP)
		{
			pSelfData->m_lCurrHP = pGame->m_csStatus.m_Info.HP = m_MaxHP;
		}

		if (pGame->m_csStatus.m_Info.MP > m_MaxMP)
		{
			pSelfData->m_lCurrMP = pGame->m_csStatus.m_Info.MP = m_MaxMP;
		}
	}
}

void CCharStatus::CheckInvenField(unsigned long dwCommand, CItemInstance *lpItem)
{
	const Item::ItemInfo& itemInfo = lpItem->GetItemInfo();
	unsigned char cPosX, cPosY, cTab;
	lpItem->m_lpItemBase->GetPos().GetPos(cPosX, cPosY, cTab);

	for (int j = cPosY; j < cPosY + itemInfo.m_DetailData.m_cYSize; ++j)
	{
		for (int i = cPosX; i < cPosX + itemInfo.m_DetailData.m_cXSize; ++i)
		{
			switch (dwCommand)
			{
				case REMOVE_FIELD:
					m_aryInven[cTab][j][i] = ITEM_EMPTY;
					break;

				case SET_FIELD:
					m_aryInven[cTab][j][i] = lpItem->m_dwItemUID;
					break;
			}
		}
	}
}

void CCharStatus::CheckDepositField(unsigned long dwCommand, CItemInstance *lpItem)
{
	const Item::ItemInfo& itemInfo = lpItem->GetItemInfo();
	unsigned char cPosX, cPosY, cTab;
	lpItem->m_lpItemBase->GetPos().GetPos(cPosX, cPosY, cTab);

	for (int j = cPosY; j < cPosY + itemInfo.m_DetailData.m_cYSize; ++j)
	{
		for (int i = cPosX; i < cPosX + itemInfo.m_DetailData.m_cXSize; ++i)
		{
			switch (dwCommand)
			{
				case REMOVE_FIELD:
					m_aryDeposit[cTab][j][i] = ITEM_EMPTY;
					break;

				case SET_FIELD:
					m_aryDeposit[cTab][j][i] = lpItem->m_dwItemUID;
					break;
			}
		}
	}
}

void CCharStatus::CheckExchangeField(unsigned long dwCommand, CItemInstance *lpItem, BOOL bSelf)
{
	const Item::ItemInfo& itemInfo = lpItem->GetItemInfo();
	unsigned char cPosX, cPosY, cTab;
	lpItem->m_lpItemBase->GetPos().GetPos(cPosX, cPosY, cTab);

	for (int j = cPosY; j < cPosY + itemInfo.m_DetailData.m_cYSize; ++j)
	{
		for (int i = cPosX; i < cPosX + itemInfo.m_DetailData.m_cXSize; ++i)
		{
			switch (dwCommand)
			{
				case REMOVE_FIELD:
					if (bSelf)
						m_aryExchange[j][i] = ITEM_EMPTY;
					else
						m_aryExchange2[j][i] = ITEM_EMPTY;
					break;

				case SET_FIELD:
					if (bSelf)
						m_aryExchange[j][i] = lpItem->m_dwItemUID;
					else
						m_aryExchange2[j][i] = lpItem->m_dwItemUID;
					break;
			}
		}
	}
}

void CCharStatus::CheckStreetTradeField(unsigned long dwCommand, CItemInstance *lpItem)
{
	const Item::ItemInfo& itemInfo = lpItem->GetItemInfo();
	unsigned char cPosX, cPosY, cTab;
	lpItem->m_lpItemBase->GetPos().GetPos(cPosX, cPosY, cTab);

	for (int j = cPosY; j < cPosY + itemInfo.m_DetailData.m_cYSize; ++j)
	{
		for (int i = cPosX; i < cPosX + itemInfo.m_DetailData.m_cXSize; ++i)
		{
			switch (dwCommand)
			{
				case REMOVE_FIELD:
					m_aryStreetTrade[j][i] = ITEM_EMPTY;
					break;

				case SET_FIELD:
					m_aryStreetTrade[j][i] = lpItem->m_dwItemUID;
					break;
			}
		}
	}
}

__int64 CCharStatus::IsCheckItem(CItemInstance* lpItem, unsigned long dwPos, 
								 unsigned long dwInvenX, unsigned long dwInvenY, unsigned long dwInvenTab)
{
	const Item::ItemInfo& itemInfo = lpItem->GetItemInfo();
	unsigned char cPosX, cPosY, cTab;
	lpItem->m_lpItemBase->GetPos().GetPos(cPosX, cPosY, cTab);

	__int64 ddExistItem = ITEM_EMPTY;
	switch (dwPos)
	{
		case TakeType::TS_INVEN:	
		{
			if (dwInvenX + itemInfo.m_DetailData.m_cXSize > INVENTORY_WIDTH || 
				dwInvenY + itemInfo.m_DetailData.m_cYSize > INVENTORY_HEIGHT)
			{
                return ITEM_FAILED;
			}

			for (unsigned long j = dwInvenY; j < dwInvenY + itemInfo.m_DetailData.m_cYSize; ++j)
			{
				for (unsigned long i = dwInvenX; i < dwInvenX + itemInfo.m_DetailData.m_cXSize; ++i)
				{
					if (m_aryInven[dwInvenTab][j][i] != ITEM_EMPTY)
					{
						if (ddExistItem == ITEM_EMPTY)
						{
							ddExistItem = m_aryInven[dwInvenTab][j][i];
						}
						else if (ddExistItem != m_aryInven[dwInvenTab][j][i])
						{
							return ITEM_FAILED;
						}
					}
				}
			}
/*
			// edith 2008.06.04 인벤토리 퀘스트 아이템 탭 제거
			// 퀘스트 인벤토리
			if (Item::ItemType::QUEST_ITEM == lpItem->m_lpItemBase->GetItemInfo().m_DetailData.m_cItemType)
			{
				if (3 != dwInvenTab)
				{
					return ITEM_QUEST_TYPE;
				}
			}
			else
			{
				if (3 == dwInvenTab)
				{
					return ITEM_NOT_QUEST_TYPE;
				}
			}
*/
			return ddExistItem;
		}

		case TakeType::TS_DEPOSIT:
		{
			if (dwInvenX + itemInfo.m_DetailData.m_cXSize > 8) return ITEM_FAILED;
			if (dwInvenY + itemInfo.m_DetailData.m_cYSize > 12) return ITEM_FAILED;

			for (unsigned long j = dwInvenY; j < dwInvenY + itemInfo.m_DetailData.m_cYSize; ++j)
			{
				for (unsigned long i = dwInvenX; i < dwInvenX + itemInfo.m_DetailData.m_cXSize; ++i)
				{
					if (m_aryDeposit[dwInvenTab][j][i] != ITEM_EMPTY)
					{
						if (ddExistItem == ITEM_EMPTY)
						{
							ddExistItem = m_aryDeposit[dwInvenTab][j][i];
						}
						else if (ddExistItem != m_aryDeposit[dwInvenTab][j][i])
						{
							return ITEM_FAILED;
						}
					}
				}
			}

			return ddExistItem;
		}

		case TakeType::TS_STALL:
		{
			if (dwInvenX + itemInfo.m_DetailData.m_cXSize > 10) return ITEM_FAILED;
			if (dwInvenY + itemInfo.m_DetailData.m_cYSize > 8) return ITEM_FAILED;

			if (lpItem->GetItemPos() != TakeType::TS_INVEN) return ITEM_FAILED;

			for (unsigned long j = dwInvenY; j < dwInvenY + itemInfo.m_DetailData.m_cYSize; ++j)
			{
				for (unsigned long i = dwInvenX; i < dwInvenX + itemInfo.m_DetailData.m_cXSize; ++i)
				{
					if (m_aryStreetTrade[j][i] != ITEM_EMPTY)
					{
						return ITEM_FAILED;
					}
				}
			}

			return ddExistItem;
		}

		case TakeType::TS_EXCHANGE:			
		{
			if (dwInvenX + itemInfo.m_DetailData.m_cXSize > 8) return ITEM_FAILED;
			if (dwInvenY + itemInfo.m_DetailData.m_cYSize > 4) return ITEM_FAILED;

			for (unsigned long j = dwInvenY; j < dwInvenY + itemInfo.m_DetailData.m_cYSize; ++j)
			{
				for (unsigned long i = dwInvenX; i < dwInvenX + itemInfo.m_DetailData.m_cXSize; ++i)
				{
					if (m_aryExchange[j][i] != ITEM_EMPTY)
					{
						if (ddExistItem == ITEM_EMPTY)
						{
							ddExistItem = m_aryExchange[j][i];
						}
						else if (ddExistItem != m_aryExchange[j][i])
						{
							return ITEM_FAILED;
						}
					}
				}
			}

			return ddExistItem;
		}

		case TakeType::TS_EQUIP:
		{
			// 능력치 제한
			if (!lpItem->GetIsEquip()) return ITEM_FAILED;
			if (!CheckItemByStatus(lpItem)) return ITEM_FAILED;
			if (!lpItem->GetNowDurability()) return ITEM_FAILED;

			switch (lpItem->GetItemType())
			{
				// 방어구
				case Item::ItemType::CON_HELM:		case Item::ItemType::DEX_HELM:
					if (Item::EquipmentPos::HELM == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::CON_HEAD:		case Item::ItemType::DEX_HEAD:
					if (m_Info.Race == CClass::RaceType::AKHAN && Item::EquipmentPos::HEAD == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::SHIRT:
					if (Item::EquipmentPos::SHIRT == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::TUNIC:
					if (Item::EquipmentPos::TUNIC == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::CON_BODY:			case Item::ItemType::DEX_BODY:
					if (m_Info.Race == CClass::RaceType::AKHAN && Item::EquipmentPos::BODY == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::CON_ARMOUR:		case Item::ItemType::DEX_ARMOUR:
					if (Item::EquipmentPos::ARMOUR == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::SHIELD:
					if (Item::EquipmentPos::SHIELD_HAND1 == dwInvenX || Item::EquipmentPos::SHIELD_HAND2 == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX])
						{
							if (m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos])
							{
								unsigned char cType = m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos]->GetItemType();
								if (cType == Item::ItemType::BOW || cType == Item::ItemType::CROSSBOW)
								{
									return ITEM_FAILED;
								}
							}
							return m_lstEquipment[dwInvenX]->m_dwItemUID;
						}
						else
						{
							if (m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos])
							{
								unsigned char cType = m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos]->GetItemType();
								if (cType == Item::ItemType::TWOHANDED_BLUNT ||
									cType == Item::ItemType::TWOHANDED_AXE ||
									cType == Item::ItemType::TWOHANDED_SWORD ||
									cType == Item::ItemType::BOW ||
									cType == Item::ItemType::CROSSBOW ||
									cType == Item::ItemType::STAFF)
								{
									return ITEM_FAILED;
								}
							}
							return ITEM_EMPTY;
						}
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::CON_PELVIS:		case Item::ItemType::DEX_PELVIS:
					if (m_Info.Race == CClass::RaceType::AKHAN && Item::EquipmentPos::PELVIS == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::CON_PROTECT_A:			case Item::ItemType::DEX_PROTECT_A:
					if (m_Info.Race == CClass::RaceType::AKHAN && Item::EquipmentPos::PROTECT_ARM == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::SKILL_A_ATTACK:
				case Item::ItemType::SKILL_A_GUARD:
				case Item::ItemType::SKILL_A_GUN:
				case Item::ItemType::SKILL_A_KNIFE:
					if (m_Info.Race == CClass::RaceType::AKHAN && Item::EquipmentPos::SKILL_ARM == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX])
							return m_lstEquipment[dwInvenX]->m_dwItemUID;
						else
						{
							if (m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1])
							{
								if (m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1]->GetTwoHandItem())
								{
									return ITEM_FAILED;
								}
							}
							return ITEM_EMPTY;
						}
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::RIDE:
				case Item::ItemType::TRANSFORM:
					if (Item::EquipmentPos::AVATA == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::CON_GLOVE:			case Item::ItemType::DEX_GLOVE:
					if (Item::EquipmentPos::GLOVE == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::CON_BOOTS:			case Item::ItemType::DEX_BOOTS:
					if (Item::EquipmentPos::BOOTS == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
					} else
						return ITEM_FAILED;
					break;
				
				//--//	start..
				// 악세사리
				case Item::ItemType::NECKLACE:
					if (Item::EquipmentPos::NECKLACE == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID;
						else return ITEM_EMPTY;
					}
					else
					{
						return ITEM_FAILED;
					}
					break;

				case Item::ItemType::RING:
					if (Item::EquipmentPos::RINGL == dwInvenX || Item::EquipmentPos::RINGR == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID;
						else return ITEM_EMPTY;
					}
					else
					{
						return ITEM_FAILED;
					}
					break;
				//--//	end..

				// 단거리 한손무기
				case Item::ItemType::ONEHANDED_SWORD:	case Item::ItemType::ONEHANDED_BLUNT:	case Item::ItemType::ONEHANDED_AXE:		
					if (m_Info.Race == CClass::RaceType::HUMAN )
					{
						if ((Item::EquipmentPos::WEAPON_HAND1 == dwInvenX || Item::EquipmentPos::WEAPON_HAND2 == dwInvenX))
						{
							if (m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos])
							{
								unsigned long dwType = m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos]->GetItemType();

								if (dwType == Item::ItemType::ARROW || dwType == Item::ItemType::BOLT ||
									dwType == Item::ItemType::DAGGER)
									return ITEM_FAILED;
							}

							if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
						}
						// 이도류시 적용
						 else if (m_Info.Class == 6 && (Item::EquipmentPos::SHIELD_HAND1 == dwInvenX || Item::EquipmentPos::SHIELD_HAND2 == dwInvenX))
						{
							if (m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos])
							{
								unsigned long dwType = m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos]->GetItemType();

								if (dwType != Item::ItemType::ONEHANDED_SWORD && dwType != Item::ItemType::ONEHANDED_BLUNT && dwType != Item::ItemType::ONEHANDED_AXE)
									return ITEM_FAILED;
							} else
								return ITEM_FAILED;

							if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
						}/**/ else
							return ITEM_FAILED;
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::DAGGER:
					if (m_Info.Race == CClass::RaceType::HUMAN)
					{
						if ((Item::EquipmentPos::WEAPON_HAND1 == dwInvenX || Item::EquipmentPos::WEAPON_HAND2 == dwInvenX))
						{
							if (m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos])
							{
								unsigned long dwType = m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos]->GetItemType();

								if (dwType == Item::ItemType::ARROW || dwType == Item::ItemType::BOLT ||
									dwType == Item::ItemType::ONEHANDED_SWORD || dwType == Item::ItemType::ONEHANDED_BLUNT ||
									dwType == Item::ItemType::ONEHANDED_AXE)
									return ITEM_FAILED;
							}

							if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
						}
						// 이도류시 적용
						 else if (m_Info.Class == 7 && (Item::EquipmentPos::SHIELD_HAND1 == dwInvenX || Item::EquipmentPos::SHIELD_HAND2 == dwInvenX))
						{
							if (m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos])
							{
								unsigned long dwType = m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos]->GetItemType();

								if (dwType != Item::ItemType::DAGGER)
									return ITEM_FAILED;
							} else
								return ITEM_FAILED;

							if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
						}/**/ else
							return ITEM_FAILED;
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::COM_BLUNT:	case Item::ItemType::COM_SWORD:
				case Item::ItemType::OPP_AXE:	case Item::ItemType::OPP_HAMMER:	case Item::ItemType::OPP_SLUSHER:
				case Item::ItemType::OPP_TALON:
					if (m_Info.Race == CClass::RaceType::AKHAN && Item::EquipmentPos::WEAPON_HAND1 == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX]) return m_lstEquipment[dwInvenX]->m_dwItemUID; else return ITEM_EMPTY;
					} else
						return ITEM_FAILED;
					break;

				// 단거리 양손무기
				case Item::ItemType::STAFF:	
				case Item::ItemType::TWOHANDED_BLUNT:
				case Item::ItemType::TWOHANDED_AXE:	
				case Item::ItemType::TWOHANDED_SWORD:
				case Item::ItemType::OPP_SYTHE:
					if (Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX])
						{
							if (m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos])
							{
								return ITEM_FAILED;
							}

							return m_lstEquipment[dwInvenX]->m_dwItemUID;
						}
						else
						{
							if (m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos])
							{
								return ITEM_FAILED;
							}

							return ITEM_EMPTY;
						}
					} else
					{
						return ITEM_FAILED;
					}
					break;

				// 장거리무기 
				case Item::ItemType::BOW:
					if (Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX])
						{
							if (m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos])
							{
								unsigned long dwType = m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos]->GetItemType();

								if (dwType == Item::ItemType::ARROW)
								{
									return m_lstEquipment[dwInvenX]->m_dwItemUID;
								}

								return ITEM_FAILED;
							}

							return m_lstEquipment[dwInvenX]->m_dwItemUID;
						}
						else
						{
							if (m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos])
							{
								unsigned long dwType = m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos]->GetItemType();

								if (dwType == Item::ItemType::ARROW)
								{
									return ITEM_EMPTY;
								}

								return ITEM_FAILED;
							}

							return ITEM_EMPTY;
						}
					} else
					{
						return ITEM_FAILED;
					}
					break;

				case Item::ItemType::CROSSBOW:
					if (Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos == dwInvenX)
					{
						if (m_lstEquipment[dwInvenX])
						{
							if (m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos])
							{
								unsigned long dwType = m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos]->GetItemType();

								if (dwType == Item::ItemType::BOLT)
								{
									return m_lstEquipment[dwInvenX]->m_dwItemUID;
								}

								return ITEM_FAILED;
							}

							return m_lstEquipment[dwInvenX]->m_dwItemUID;
						}
						else
						{
							if (m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos])
							{
								unsigned long dwType = m_lstEquipment[Item::EquipmentPos::SHIELD_HAND1 + m_cWeaponPos]->GetItemType();

								if (dwType == Item::ItemType::BOLT)
								{
									return ITEM_EMPTY;
								}

								return ITEM_FAILED;
							}

							return ITEM_EMPTY;
						}
					} else
					{
						return ITEM_FAILED;
					}
					break;

				case Item::ItemType::BOLT:
					if (m_Info.Race == CClass::RaceType::HUMAN && (Item::EquipmentPos::SHIELD_HAND1 == dwInvenX || Item::EquipmentPos::SHIELD_HAND2 == dwInvenX))
					{
						if (m_lstEquipment[dwInvenX])
						{
							if (m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos])
							{
								unsigned char cType = m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos]->GetItemType();
								if (cType != Item::ItemType::CROSSBOW)
								{
									return ITEM_FAILED;
								}
							}
							return m_lstEquipment[dwInvenX]->m_dwItemUID;
						}
						else
						{
							if (m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos])
							{
								unsigned char cType = m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos]->GetItemType();
								if (cType == Item::ItemType::CROSSBOW)
								{
									return ITEM_EMPTY;
								}
							}
							return ITEM_FAILED;
						}
					} else
						return ITEM_FAILED;
					break;

				case Item::ItemType::ARROW:
					if (m_Info.Race == CClass::RaceType::HUMAN && (Item::EquipmentPos::SHIELD_HAND1 == dwInvenX || Item::EquipmentPos::SHIELD_HAND2 == dwInvenX))
					{
						if (m_lstEquipment[dwInvenX])
						{
							if (m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos])
							{
								unsigned char cType = m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos]->GetItemType();
								if (cType != Item::ItemType::BOW)
								{
									return ITEM_FAILED;
								}
							}
							return m_lstEquipment[dwInvenX]->m_dwItemUID;
						}
						else
						{
							if (m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos])
							{
								unsigned char cType = m_lstEquipment[Item::EquipmentPos::WEAPON_HAND1 + m_cWeaponPos]->GetItemType();
								if (cType == Item::ItemType::BOW)
								{
									return ITEM_EMPTY;
								}
							}
							return ITEM_FAILED;
						}
					} else
						return ITEM_FAILED;
					break;

				default:
					return ITEM_FAILED;
			}
			break;
		}

		case TakeType::TS_QSLOT:			
			break;
	}

	return ITEM_EMPTY;
}

bool CCharStatus::GetAbleInven(LPFieldInstance lpFieldInstance, Item::ItemPos &pItemIndex, BOOL bStack)
{
	return GetAbleInven(lpFieldInstance->m_dwTypeID, lpFieldInstance->m_cNum, pItemIndex, bStack);
}

bool CCharStatus::GetAbleInven(unsigned short wItemID, unsigned char cNum, Item::ItemPos &pItemIndex, BOOL bStack)
{
	const Item::ItemInfo* lpProtoType = g_ItemMgr.GetItemInfo(wItemID);
	if (NULL != lpProtoType) 
	{
		unsigned long dwSizeX = lpProtoType->m_DetailData.m_cXSize;
		unsigned long dwSizeY = lpProtoType->m_DetailData.m_cYSize;

		// 퀘스트 아이템은 퀘스트 탭에만, 일반 아이템은 일반 탭에만 들어갈 수 있다.
		unsigned long dwStartTab = 0;
		unsigned long dwEndTab = INVENTORY_TAB;

/*
		// edith 2008.06.04 인벤토리 퀘스트 아이템 탭 제거
		unsigned long dwEndTab = INVENTORY_TAB - 1;

		if (Item::ItemType::QUEST_ITEM == lpProtoType->m_DetailData.m_cItemType)
		{
			dwStartTab = INVENTORY_TAB - 1;
			dwEndTab = INVENTORY_TAB;
		}
*/
		for (unsigned long k = dwStartTab; k < dwEndTab; ++k)
		{
			for (unsigned long j = 0; j <= INVENTORY_HEIGHT - dwSizeY; ++j)
			{
				for (unsigned long i = 0; i <= INVENTORY_WIDTH - dwSizeX; ++i)
				{
					bool bAble = true;

					for (unsigned long cy = 0; cy < dwSizeY; ++cy)
					{
						for (unsigned long cx = 0; cx < dwSizeX; ++cx)
						{
							if (m_aryInven[k][j + cy][i + cx] != ITEM_EMPTY)
							{
								CItemInstance* lpItem = GetInventoryItem(m_aryInven[k][j + cy][i + cx]);
								if (NULL != lpItem)
								{
									unsigned char cPosX, cPosY, cTab;
									lpItem->m_lpItemBase->GetPos().GetPos(cPosX, cPosY, cTab);

									if ( TRUE == bStack && (lpProtoType->m_DetailData.m_dwFlags & Item::DetailData::STACKABLE) &&	// 스택이 가능한가?
										(cPosX == i && cPosY == j) && lpItem->GetProtoTypeID() == wItemID &&						// 같은 아이템인가?
										lpItem->GetNowDurability() < lpItem->GetMaxDurability())									// 꽉 차있나?
									{
										if (lpItem->GetMaxDurability() - lpItem->GetNowDurability() < cNum)
										{
											bAble = false;
										}
									} 
									else
									{
										bAble = false;
									}
								}
							}
						}
					}

					if (true == bAble)
					{
						pItemIndex.m_cPos = TakeType::TS_INVEN;
						pItemIndex.SetPos(i, j, k);

						return true;
					} 
				}
			}
		}
	}

	return false;
}

CItemInstance *CCharStatus::GetDepositItem(__int64 ddItemUID)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstDeposit.begin(); it != m_lstDeposit.end(); ++it)
	{
		if ((*it)->m_dwItemUID == ddItemUID) return (*it);
	}

	return NULL;
}

CItemInstance *CCharStatus::GetDepositItem(Item::ItemPos &pIndex)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstDeposit.begin(); it != m_lstDeposit.end(); ++it)
	{
		if ((*it)->m_lpItemBase->GetPos() == pIndex) return (*it);
	}

	return NULL;
}

CItemInstance *CCharStatus::GetDepositItem(unsigned short wIndex)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstDeposit.begin(); it != m_lstDeposit.end(); ++it)
	{
		if ((*it)->m_lpItemBase->GetPos().m_cIndex == wIndex) return (*it);
	}

	return NULL;
}

CItemInstance *CCharStatus::GetDepositItemfromID(unsigned short wProtoTypeID)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstDeposit.begin(); it != m_lstDeposit.end(); ++it)
	{
		if ((*it)->GetProtoTypeID() == wProtoTypeID) return (*it);
	}

	return NULL;
}

BOOL CCharStatus::GetIsInventoryItem(CItemInstance *lpItem)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstInventory.begin(); it != m_lstInventory.end(); ++it)
	{
		if ((*it) == lpItem) return TRUE;
	}

	return FALSE;
}

CItemInstance *CCharStatus::GetInventoryItem(__int64 ddItemUID)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstInventory.begin(); it != m_lstInventory.end(); ++it)
	{
		if ((*it)->m_dwItemUID == ddItemUID) return (*it);
	}

	return NULL;
}

CItemInstance *CCharStatus::GetInventoryItem(Item::ItemPos &pIndex)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstInventory.begin(); it != m_lstInventory.end(); ++it)
	{
		if ((*it)->m_lpItemBase->GetPos() == pIndex) return (*it);
	}

	return NULL;
}

CItemInstance *CCharStatus::GetInventoryItem(unsigned short wIndex)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstInventory.begin(); it != m_lstInventory.end(); ++it)
	{
		if ((*it)->m_lpItemBase->GetPos().m_cIndex == wIndex) return (*it);
	}

	return NULL;
}

CItemInstance *CCharStatus::GetInventoryItemfromID(unsigned short wProtoTypeID)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstInventory.begin(); it != m_lstInventory.end(); ++it)
	{
		if ((*it)->GetProtoTypeID() == wProtoTypeID) return (*it);
	}

	return NULL;
}

CItemInstance *CCharStatus::GetStreetTradeItem(__int64 ddItemUID)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstStreetTrade.begin(); it != m_lstStreetTrade.end(); ++it)
	{
		if ((*it)->m_dwItemUID == ddItemUID) return (*it);
	}

	return NULL;
}

CItemInstance *CCharStatus::GetStreetTradeItem(Item::ItemPos &pIndex)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstStreetTrade.begin(); it != m_lstStreetTrade.end(); ++it)
	{
		if ((*it)->m_lpItemBase->GetPos() == pIndex) return (*it);
	}

	return NULL;
}

CItemInstance *CCharStatus::GetStreetTradeItem(unsigned short wIndex)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstStreetTrade.begin(); it != m_lstStreetTrade.end(); ++it)
	{
		if ((*it)->m_lpItemBase->GetPos().m_cIndex == wIndex) return (*it);
	}

	return NULL;
}

CItemInstance *CCharStatus::GetStreetTradeItemfromID(unsigned short wProtoTypeID)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstStreetTrade.begin(); it != m_lstStreetTrade.end(); ++it)
	{
		if ((*it)->GetProtoTypeID() == wProtoTypeID) return (*it);
	}

	return NULL;
}

CItemInstance *CCharStatus::GetExchangeItem(__int64 ddItemUID, BOOL bSelf)
{
	list<CItemInstance *>::iterator it;
	if (bSelf)
	{
		for (it = m_lstExchange.begin(); it != m_lstExchange.end(); ++it)
		{
			if ((*it)->m_dwItemUID == ddItemUID) return (*it);
		}
	} else
	{
		for (it = m_lstExchange2.begin(); it != m_lstExchange2.end(); ++it)
		{
			if ((*it)->m_dwItemUID == ddItemUID) return (*it);
		}
	}

	return NULL;
}

CItemInstance *CCharStatus::GetExchangeItem(unsigned short wIndex, BOOL bSelf)
{
	list<CItemInstance *>::iterator it;
	if (bSelf)
	{
		for (it = m_lstExchange.begin(); it != m_lstExchange.end(); ++it)
		{
			if ((*it)->m_lpItemBase->GetPos().m_cIndex == wIndex) return (*it);
		}
	} else
	{
		for (it = m_lstExchange2.begin(); it != m_lstExchange2.end(); ++it)
		{
			if ((*it)->m_lpItemBase->GetPos().m_cIndex == wIndex) return (*it);
		}
	}

	return NULL;
}

CItemInstance *CCharStatus::GetExchangeItem(Item::ItemPos &pIndex, BOOL bSelf)
{
	list<CItemInstance *>::iterator it;
	if (bSelf)
	{
		for (it = m_lstExchange.begin(); it != m_lstExchange.end(); ++it)
		{
			if ((*it)->m_lpItemBase->GetPos() == pIndex) return (*it);
		}
	} else
	{
		for (it = m_lstExchange2.begin(); it != m_lstExchange2.end(); ++it)
		{
			if ((*it)->m_lpItemBase->GetPos() == pIndex) return (*it);
		}
	}

	return NULL;
}

CItemInstance *CCharStatus::GetExchangeItemfromID(unsigned short wProtoTypeID, BOOL bSelf)
{
	list<CItemInstance *>::iterator it;
	if (bSelf)
	{
		for (it = m_lstExchange.begin(); it != m_lstExchange.end(); ++it)
		{
			if ((*it)->GetProtoTypeID() == wProtoTypeID) return (*it);
		}
	} else
	{
		for (it = m_lstExchange2.begin(); it != m_lstExchange2.end(); ++it)
		{
			if ((*it)->GetProtoTypeID() == wProtoTypeID) return (*it);
		}
	}

	return NULL;
}

void CCharStatus::DeleteInventoryItem(CItemInstance *lpDelete)
{
	for (list<CItemInstance *>::iterator it = m_lstInventory.begin(); it != m_lstInventory.end(); ++it)
	{
		if ((*it) == lpDelete)
		{
			CheckInvenField(REMOVE_FIELD, lpDelete);
			m_lstInventory.erase(it);
			SetQuickSlot(lpDelete);
			break;
		}
	}
}

void CCharStatus::DeleteDepositItem(CItemInstance *lpDelete)
{
	list<CItemInstance *>::iterator it;
	for (it = m_lstDeposit.begin(); it != m_lstDeposit.end(); ++it)
	{
		if ((*it) == lpDelete)
		{
			CheckDepositField(REMOVE_FIELD, lpDelete);
			m_lstDeposit.erase(it);
			break;
		}
	}
}

void CCharStatus::DeleteExchangeItem(CItemInstance *lpDelete, BOOL bSelf)
{
	list<CItemInstance *>::iterator it;
	if (bSelf)
	{
		for (it = m_lstExchange.begin(); it != m_lstExchange.end(); ++it)
		{
			if ((*it) == lpDelete)
			{
				CheckExchangeField(REMOVE_FIELD, lpDelete, bSelf);
				m_lstExchange.erase(it);
				break;
			}
		}
	} else
	{
		for (it = m_lstExchange2.begin(); it != m_lstExchange2.end(); ++it)
		{
			if ((*it) == lpDelete)
			{
				CheckExchangeField(REMOVE_FIELD, lpDelete, bSelf);
				m_lstExchange2.erase(it);
				break;
			}
		}
	}
}

void CCharStatus::DeleteStreetTradeItem(CItemInstance *lpDelete)
{
	for (list<CItemInstance *>::iterator it = m_lstStreetTrade.begin(); it != m_lstStreetTrade.end(); ++it)
	{
		if (*it == lpDelete)
		{
			CheckStreetTradeField(REMOVE_FIELD, lpDelete);
			m_lstStreetTrade.erase(it);

			delete lpDelete;
			lpDelete = NULL;

			break;
		}
	}
}

void CCharStatus::AddInventoryItem(CItemInstance* lpItem, BOOL bSound)
{
	CItemInstance* lpPreItem = GetInventoryItem(lpItem->m_lpItemBase->GetPos().m_cIndex);
	if (NULL != lpPreItem)
	{
		if (TRUE == lpPreItem->GetIsEnableStack())
		{
			DeleteInventoryItem(lpPreItem);
			delete lpPreItem;
			lpPreItem = NULL ;
		} 
		else
		{
			return;
		}
	}

	m_lstInventory.push_back(lpItem);
	SetQuickSlot(lpItem);
	CheckInvenField(SET_FIELD, lpItem);

	if (TRUE == bSound && NULL != lpItem->m_lpItemBase &&
		strcmp(lpItem->m_lpItemBase->GetItemInfo().m_StringData.m_szEffectSoundName, "NONE"))
	{
		char Temp[MAX_PATH]; 
		sprintf(Temp, "%s\\Sound\\Item\\%s", g_ClientMain.m_strClientPath, lpItem->m_lpItemBase->GetItemInfo().m_StringData.m_szEffectSoundName);
		CSound* pSound; 
		CSoundMgr::_GetInstance()->GetSound( pSound, Temp ); 
		pSound->Play(); 
	}
}

CItemInstance* CCharStatus::GetItem(Item::ItemPos srcPos)
{
	switch (srcPos.m_cPos)
	{
		case TakeType::TS_INVEN:		return GetInventoryItem(srcPos.m_cIndex);
		case TakeType::TS_EQUIP:		return m_lstEquipment[srcPos.m_cIndex];
		case TakeType::TS_EXCHANGE:		return GetExchangeItem(srcPos.m_cIndex);
		case TakeType::TS_DEPOSIT:		return GetDepositItem(srcPos.m_cIndex);

		case TakeType::TS_EXTRA:
		{
			switch (srcPos.m_cIndex)
			{
				case Item::ExtraSpacePos::UPGRADE_EQUIPMENT_POS:
				case Item::ExtraSpacePos::UPGRADE_MATERIAL_POS:
					return m_lpUpgrade[srcPos.m_cIndex];

				case Item::ExtraSpacePos::KIT_MATERIAL_POS:		return m_lpSiegeArmsMaterial;

				case Item::ExtraSpacePos::GRAFT_ORIGINAL_POS:	return m_lpCompositionOrigin;
				case Item::ExtraSpacePos::GRAFT_SACRIFICE_POS:	return m_lpCompositionOffering;
				case Item::ExtraSpacePos::GRAFT_RESULT_POS:		return m_lpCompositionResult;

				case Item::ExtraSpacePos::COMPENSATION_POS:		return m_lpCompensation;
			}
			break;
		}
	}

	return NULL;
}

void CCharStatus::SetItem(Item::ItemPos dstPos, CItemInstance* lpItem, BOOL bSound, BOOL bSelf)
{
	switch (dstPos.m_cPos)
	{
		case TakeType::TS_INVEN:		AddInventoryItem(lpItem, bSound);						break;
		case TakeType::TS_EQUIP:		AddEquipItem(dstPos.m_cIndex, lpItem, bSound);			break;
		case TakeType::TS_EXCHANGE:		AddExchangeItem(lpItem, bSound, bSelf);					break;
		case TakeType::TS_DEPOSIT:		AddDepositItem(lpItem, bSound);							break;
		case TakeType::TS_TEMP:			CRYLGameData::Instance()->SetPickItem(lpItem, bSound);	break;

		case TakeType::TS_EXTRA:
		{
			switch (dstPos.m_cIndex)
			{
				case Item::ExtraSpacePos::UPGRADE_EQUIPMENT_POS:
				case Item::ExtraSpacePos::UPGRADE_MATERIAL_POS:
				{
					m_lpUpgrade[dstPos.m_cIndex] = lpItem;
					break;
				}

				case Item::ExtraSpacePos::KIT_MATERIAL_POS:		m_lpSiegeArmsMaterial = lpItem;		break;

				case Item::ExtraSpacePos::GRAFT_ORIGINAL_POS:	m_lpCompositionOrigin = lpItem;		break;
				case Item::ExtraSpacePos::GRAFT_SACRIFICE_POS:	m_lpCompositionOffering = lpItem;	break;
				case Item::ExtraSpacePos::GRAFT_RESULT_POS:		m_lpCompositionResult = lpItem;		break;

				case Item::ExtraSpacePos::COMPENSATION_POS:		m_lpCompensation = lpItem;			break;
			}
			break;
		}
	}
}

void CCharStatus::RemoveItem(Item::ItemPos dstPos)
{
	switch (dstPos.m_cPos)
	{
		case TakeType::TS_INVEN:		DeleteInventoryItem(GetItem(dstPos));		break;
		case TakeType::TS_EQUIP:		m_lstEquipment[dstPos.m_cIndex] = NULL;		break;
		case TakeType::TS_EXCHANGE:		DeleteExchangeItem(GetItem(dstPos));		break;
		case TakeType::TS_DEPOSIT:		DeleteDepositItem(GetItem(dstPos));			break;

		case TakeType::TS_EXTRA:
		{
			switch (dstPos.m_cIndex)
			{
				case Item::ExtraSpacePos::UPGRADE_EQUIPMENT_POS:
				case Item::ExtraSpacePos::UPGRADE_MATERIAL_POS:
				{
					m_lpUpgrade[dstPos.m_cIndex] = NULL;					
					break;
				}

				case Item::ExtraSpacePos::KIT_MATERIAL_POS:		m_lpSiegeArmsMaterial = NULL;		break;

				case Item::ExtraSpacePos::GRAFT_ORIGINAL_POS:	m_lpCompositionOrigin = NULL;		break;
				case Item::ExtraSpacePos::GRAFT_SACRIFICE_POS:	m_lpCompositionOffering = NULL;		break;
				case Item::ExtraSpacePos::GRAFT_RESULT_POS:		m_lpCompositionResult = NULL;		break;

				case Item::ExtraSpacePos::COMPENSATION_POS:		m_lpCompensation = NULL;			break;
			}
			break;
		}
	}
}

void CCharStatus::AddEquipItem(unsigned long dwPos, CItemInstance* lpItem, BOOL bSound)
{
	m_lstEquipment[dwPos] = lpItem;

	if (bSound && lpItem->m_lpItemBase &&
		strcmp(lpItem->m_lpItemBase->GetItemInfo().m_StringData.m_szEffectSoundName, "NONE"))
	{
		char Temp[MAX_PATH]; 
		sprintf(Temp, "%s\\Sound\\Item\\%s", g_ClientMain.m_strClientPath, lpItem->m_lpItemBase->GetItemInfo().m_StringData.m_szEffectSoundName);
		CSound* pSound; 
		CSoundMgr::_GetInstance()->GetSound( pSound, Temp ); 
		pSound->Play(); 

		//아머 입을 시에 ... 미니탭 변경하는 부분.
		if ( m_Info.Race == CClass::RaceType::HUMAN )			// 현재 인간만이 아머 타입이 있다...
		{
			CRYLGameScene*				pScene		= ( CRYLGameScene * )CRYLSceneManager::Instance()->FindScene( GAME_SCENE ) ;
			CRYLHumanInventoryFrameDlg*	pDlg		= ( CRYLHumanInventoryFrameDlg* )pScene->GetHumanInventoryFrameDlg() ;
			CRYLHumanInventoryPanel*	pPanel		= ( CRYLHumanInventoryPanel* )pDlg->GetHumanInvenPanel() ;
			if ( !pPanel )
				return;

			switch ( lpItem->GetItemType() )
			{
			case Item::ItemType::SHIRT:
				pPanel->UpdateArmorCheckButton( 0 ) ;
				break;

			case Item::ItemType::TUNIC:
				pPanel->UpdateArmorCheckButton( 1 ) ;
				break;

			case Item::ItemType::CON_ARMOUR:		
			case Item::ItemType::DEX_ARMOUR:
				pPanel->UpdateArmorCheckButton( 2 ) ;
				break;

			default:
				break;
			}
		}
	}
}

void CCharStatus::AddDepositItem(CItemInstance* lpItem, BOOL bSound)
{
	CItemInstance *lpPreItem = GetDepositItem(lpItem->m_lpItemBase->GetPos().m_cIndex);
	if (lpPreItem)
	{
		if (lpPreItem->GetIsEnableStack())
		{
			DeleteDepositItem(lpPreItem);
			delete lpPreItem;
			lpPreItem = NULL ;
		} 
		else
		{
			return;
		}
	}

	m_lstDeposit.push_back(lpItem);
	CheckDepositField(SET_FIELD, lpItem);

	if (bSound && lpItem->m_lpItemBase &&
		strcmp(lpItem->m_lpItemBase->GetItemInfo().m_StringData.m_szEffectSoundName, "NONE"))
	{
		char Temp[MAX_PATH]; 
		sprintf(Temp, "%s\\Sound\\Item\\%s", g_ClientMain.m_strClientPath, lpItem->m_lpItemBase->GetItemInfo().m_StringData.m_szEffectSoundName);
		CSound* pSound; 
		CSoundMgr::_GetInstance()->GetSound( pSound, Temp ); 
		pSound->Play(); 
	}
}

void CCharStatus::AddExchangeItem(CItemInstance* lpItem, BOOL bSelf, BOOL bSound)
{
	CItemInstance* lpPreItem = GetExchangeItem(lpItem->m_lpItemBase->GetPos().m_cIndex, bSelf);
	if (lpPreItem)
	{
		if (lpPreItem->GetIsEnableStack())
		{
			DeleteExchangeItem(lpPreItem, bSelf);
			delete lpPreItem;
			lpPreItem = NULL ;
		} 
		else
		{
			return;
		}
	}

	if (bSelf)
	{
		m_lstExchange.push_back(lpItem);
	} 
	else
	{
		m_lstExchange2.push_back(lpItem);
	}
	CheckExchangeField(SET_FIELD, lpItem, bSelf);

	if (bSound && lpItem->m_lpItemBase &&
		strcmp(lpItem->m_lpItemBase->GetItemInfo().m_StringData.m_szEffectSoundName, "NONE"))
	{
		char Temp[MAX_PATH]; 
		sprintf(Temp, "%s\\Sound\\Item\\%s", g_ClientMain.m_strClientPath, lpItem->m_lpItemBase->GetItemInfo().m_StringData.m_szEffectSoundName);
		CSound* pSound; 
		CSoundMgr::_GetInstance()->GetSound( pSound, Temp ); 
		pSound->Play(); 
	}
}

SKILLSLOT *CCharStatus::GetSkillSlot(unsigned short wSkill)
{
	const Skill::ProtoType *lpSkillBase = g_SkillMgr.GetSkillProtoType(wSkill);

	if (lpSkillBase->m_usParentSkill) wSkill = lpSkillBase->m_usParentSkill;		// 얼터너티브 스킬을 위해 부모를 검색
	
	for (int i = 0; i < SKILL::MAX_SLOT_NUM; ++i)
	{
		if (m_Skill.SSlot[i].SKILLINFO.wSkill == wSkill)
		{
			return &m_Skill.SSlot[i];
		}
	}

	return NULL;
}

void CCharStatus::DestroyParty()
{
	m_Info.PID = 0;
	m_PartyInfo.m_cMemberNum	= 0;
	m_PartyInfo.m_dwPartyID		= 0;
	m_PartyInfo.m_dwLeaderID	= 0;	

	for (int i = 0; i < 10; ++i)
	{
		m_PartyInfo.ServerID[i] = 0;
		m_PartyInfo.MemberCID[i] = 0;

	    m_PartyInfo.m_wClass[i] = 0;	// 멤버 클래스 
	    m_PartyInfo.m_cLevel[i] = 0;	// 멤버 레벨
	    m_PartyInfo.m_dwGID[i]  = 0;    // 멤버 길드 아이디

		strcpy(m_PartyInfo.Name[i], "");

        m_PartyInfo.m_bAutoRouting[i] = 0;

		m_PartyInfo.m_Position[i].fPointX = 0;
		m_PartyInfo.m_Position[i].fPointY = 0;
		m_PartyInfo.m_Position[i].fPointZ = 0;
	}
}

void CCharStatus::DeletePartyPlayer(unsigned long dwChrID)
{
	for (int i = 0; i < m_PartyInfo.m_cMemberNum; ++i)
	{
		if (m_PartyInfo.MemberCID[i] == dwChrID)
		{
			for (int j = i; j < PARTY::MAX_MEM - 1; ++j)
			{
				m_PartyInfo.MemberCID[j] = m_PartyInfo.MemberCID[j + 1];
				m_PartyInfo.ServerID[j] = m_PartyInfo.ServerID[j + 1];

	            m_PartyInfo.m_wClass[j] = m_PartyInfo.m_wClass[j + 1];	// 멤버 클래스 
	            m_PartyInfo.m_cLevel[j] = m_PartyInfo.m_cLevel[j + 1];	// 멤버 레벨
	            m_PartyInfo.m_dwGID[j]  = m_PartyInfo.m_dwGID[j + 1];   // 멤버 길드 아이디

                m_PartyInfo.m_bAutoRouting[j] = m_PartyInfo.m_bAutoRouting[j + 1];

		        m_PartyInfo.m_Position[j].fPointX = m_PartyInfo.m_Position[j + 1].fPointX;
		        m_PartyInfo.m_Position[j].fPointY = m_PartyInfo.m_Position[j + 1].fPointX;
		        m_PartyInfo.m_Position[j].fPointZ = m_PartyInfo.m_Position[j + 1].fPointX;

                strcpy(m_PartyInfo.Name[j], m_PartyInfo.Name[j + 1]);
				m_aryPartyExtraInfo[j] = m_aryPartyExtraInfo[j + 1];
			}

			--m_PartyInfo.m_cMemberNum;

			m_PartyInfo.MemberCID[PARTY::MAX_MEM - 1] = 0;
			m_PartyInfo.ServerID[PARTY::MAX_MEM - 1] = 0;

	        m_PartyInfo.m_wClass[PARTY::MAX_MEM - 1] = 0;
	        m_PartyInfo.m_cLevel[PARTY::MAX_MEM - 1] = 0;
	        m_PartyInfo.m_dwGID[PARTY::MAX_MEM - 1]  = 0;

            m_PartyInfo.m_bAutoRouting[PARTY::MAX_MEM - 1] = 0;

			m_PartyInfo.m_Position[PARTY::MAX_MEM - 1].fPointX = 0;
			m_PartyInfo.m_Position[PARTY::MAX_MEM - 1].fPointY = 0;
			m_PartyInfo.m_Position[PARTY::MAX_MEM - 1].fPointZ = 0;

			m_aryPartyExtraInfo[PARTY::MAX_MEM - 1] = PartyExtraInfo();
			strcpy(m_PartyInfo.Name[PARTY::MAX_MEM - 1], "");

            RYLCreature* pTempCreature = RYLCreatureManager::Instance()->GetCreature( dwChrID );
            if ( pTempCreature )
            {
                CharDataNode* pTempData = RYLCharacterDataManager::Instance()->GetCharData( pTempCreature->GetCharID() );
                if ( pTempData )
                {
                    pTempData->m_ulPartyID = 0;
                }
            }
			break;
		}
	}
}

void CCharStatus::AddPartyPlayer(unsigned long dwChrID, unsigned long dwServerID, char *strName)
{
	for (int i = 0; i < PARTY::MAX_MEM; ++i)
	{
		if (!m_PartyInfo.MemberCID[i])
		{
			m_PartyInfo.MemberCID[i] = dwChrID;
			m_PartyInfo.ServerID[i] = dwServerID;
			m_aryPartyExtraInfo[i] = PartyExtraInfo();
			strcpy(m_PartyInfo.Name[i], strName);

	        m_PartyInfo.m_wClass[i] = 0;
	        m_PartyInfo.m_cLevel[i] = 0;
	        m_PartyInfo.m_dwGID[i]  = 0;

            m_PartyInfo.m_bAutoRouting[i] = 0;

			m_PartyInfo.m_Position[i].fPointX = 0;
			m_PartyInfo.m_Position[i].fPointY = 0;
			m_PartyInfo.m_Position[i].fPointZ = 0;

			++m_PartyInfo.m_cMemberNum;
	
            RYLCreature* pTempCreature = RYLCreatureManager::Instance()->GetCreature( dwChrID );
            if ( pTempCreature )
            {
                CharDataNode* pTempData = RYLCharacterDataManager::Instance()->GetCharData( pTempCreature->GetCharID() );
                if ( pTempData )
                {
                    pTempData->m_ulPartyID = m_PartyInfo.m_dwPartyID;
                }
            }
			break;
		}
	}
}

char* CCharStatus::LoginPartyPlayer(unsigned long dwChrID, unsigned long dwServerID)
{
	for (int i = 0; i < PARTY::MAX_MEM; ++i)
	{
		if (m_PartyInfo.MemberCID[i] == dwChrID)
		{
			m_PartyInfo.ServerID[i] = dwServerID;
			return m_PartyInfo.Name[i];
		}
	}

	return NULL;
}

char* CCharStatus::LogoutPartyPlayer(unsigned long dwChrID)
{
	for (int i = 0; i < PARTY::MAX_MEM; ++i)
	{
		if (m_PartyInfo.MemberCID[i] == dwChrID)
		{
			m_PartyInfo.ServerID[i] = 0;
			m_PartyInfo.m_Position[i].fPointX = 0;
			m_PartyInfo.m_Position[i].fPointY = 0;
			m_PartyInfo.m_Position[i].fPointZ = 0;
			return m_PartyInfo.Name[i];
		}
	}

	return NULL;
}

bool CCharStatus::IsPartyMember(unsigned long dwChrID)
{
    for (int i = 0; i < PARTY::MAX_MEM; ++i)
    {
        if (dwChrID == m_PartyInfo.MemberCID[i])
        {
            return true;
        }
    }

    return false;
}

bool CCharStatus::IsPartyMember(const char* szName)
{
    for (int i = 0; i < PARTY::MAX_MEM; ++i)
    {
        if (0 == strcmp(m_PartyInfo.Name[i], szName))
        {
            return true;
        }
    }

    return false;
}


BOOL CCharStatus::GetIsInstallSocket(CItemInstance *lpEquipItem, CItemInstance *lpSocketItem)
{
	// 소켓 밖기
	if ( !lpEquipItem || !lpSocketItem )
		return FALSE;
	if (lpEquipItem->GetIsEquip())
	{
		Item::CEquipment *lpEquip = Item::CEquipment::DowncastToEquipment(lpEquipItem->m_lpItemBase);
		if (lpEquip)
		{
			if (lpSocketItem->GetNowDurability() == 1)
			{
				if (lpSocketItem->GetItemType() == Item::ItemType::GEM)
				{
					int sn;
					for (sn = 0; sn < lpEquip->GetMaxSocketNum(); sn++)
					{
						if (!lpEquip->GetSocket(sn) ) break;
					}

					if (lpSocketItem->GetProtoTypeID() == Item::EtcItemID::GEM_MAXSOCKET_ID)
					{
						if(lpEquip->GetMaxSocketNum() == Item::EquipmentInfo::MAX_SOCKET_NUM-1)
							return TRUE;

						return FALSE;
					}

					if (lpSocketItem->GetProtoTypeID() == Item::EtcItemID::GEM_UNSOCKET_ID)
					{
						if(sn > 0)
							return TRUE;

						return FALSE;
					}

					if (Item::EtcItemID::GEM_RUNE_START_ID <= lpSocketItem->GetProtoTypeID() &&
						lpSocketItem->GetProtoTypeID() <= Item::EtcItemID::GEM_RUNE_END_ID)
					{
						switch (Item::CItemType::GetEquipType(lpEquip->GetItemInfo().m_DetailData.m_dwFlags))
						{
						case Item::CItemType::HELM_TYPE:
						case Item::CItemType::ARMOUR_TYPE:
							return TRUE;
						}

						return FALSE;
					}

					if (sn < lpEquip->GetMaxSocketNum())
					{
						return TRUE;
					}
				}
				/*
				else if (lpSocketItem->GetItemType() == Item::ItemType::RUNE)
				{
					if (!lpEquip->m_usRuneSocket)
					{
						return TRUE;
					}
				}
				*/

			}
		}
	}

	return FALSE;
}

BOOL CCharStatus::GetIsRuneSocket(CItemInstance *lpExistItem, CItemInstance *lpRuneItem, int &nPosition )
{
	if ( !lpExistItem || !lpRuneItem )
	{
		return FALSE;
	}

	Item::CEquipment *lpExist = Item::CEquipment::DowncastToEquipment(lpExistItem->m_lpItemBase);
	if (lpExist)
	{
		if (lpRuneItem->GetNowDurability() == 1)
		{
			if (lpRuneItem->GetItemType() == Item::ItemType::RUNE )
			{
                nPosition = lpExist->GetRuneEquipable();

				// 소멸룬일 경우
				if ( Item::EtcItemID::DESTRUCTION_RUNE == lpRuneItem->m_lpItemBase->GetPrototypeID() )
				{
					if ( nPosition == 1 )  // 아무것도 없다면...
					{
						return FALSE;			
					}

					// 둘다 있거나 하나라도 있다면...
					return TRUE;
				}

				if (0 != nPosition )
				{
					nPosition = nPosition - 1;
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


BOOL CCharStatus::GetIsChemicalitem( CItemInstance *lpExistItem, CItemInstance *lpPickItem, unsigned char &result )
{
	// 아이템 합성
	if ( !lpExistItem || !lpPickItem )
	{
		return FALSE;
	}

	Item::ChemicalInfo  chemInfo;


	chemInfo.m_wPickkingItemID = lpPickItem->GetProtoTypeID();
	chemInfo.m_wTargetItemID = lpExistItem->GetProtoTypeID();
	chemInfo.m_wResultItemID = 0;
	chemInfo.m_cPickkingItemNum = lpPickItem->GetNowDurability();
	chemInfo.m_cTargetItemNum = lpExistItem->GetNowDurability();
	chemInfo.m_cResultItemNum = 0;

	result = ( Item::CItemMgr::GetInstance() ).GetChemicalResult( chemInfo );

	// 1차 성공시에 퀘스트랑 어빌리티를 다시 확인한다.
	if(Item::Chemical::S_SUCCESS == result && chemInfo.m_usQuestID != 0)
	{
		result = Item::Chemical::F_NOT_QUEST;

		// 퀘스트 확인
		list<LPQuestInstance>::iterator it;
		for (it = m_lstCompleteQuest.begin(); it != m_lstCompleteQuest.end(); ++it)
		{
			if(	chemInfo.m_usQuestID == (*it)->m_dwQuestID )
			{
				result = Item::Chemical::S_SUCCESS;
				break;
			}
		}		
	}

	if(Item::Chemical::S_SUCCESS == result && chemInfo.m_usAbilityID != 0)
	{
		result = Item::Chemical::F_NOT_ABILITY;

		// 스킬정보에서 어빌리티를 확인한다.
		// 스킬을 뒤지자
		int iIndex = 0;
		for(iIndex = 0; iIndex <= m_Skill.wSlotNum; ++iIndex)
		{
			if(m_Skill.SSlot[iIndex].SKILLINFO.wSkill == chemInfo.m_usAbilityID && 
				m_Skill.SSlot[iIndex].SKILLINFO.cLockCount >= chemInfo.m_cAbilityLevel)
			{
				result = Item::Chemical::S_SUCCESS;
				break;
			}
		}
	}

	return ( Item::Chemical::S_SUCCESS == result ) ? TRUE : FALSE;
}

BOOL CCharStatus::CheckPartyMember(unsigned long dwChrID)
{
	for (int i = 0; i < 10; ++i)
	{
		if (m_PartyInfo.MemberCID[i] == dwChrID) 
		{
			return TRUE;
		}
	}

	return FALSE;
}

char *CCharStatus::GetPartyName(unsigned long dwChrID)
{
	for (int i = 0; i < 10; ++i)
	{
		if (m_PartyInfo.MemberCID[i] == dwChrID) 
		{
			return m_PartyInfo.Name[i];
		}
	}

	return NULL;
}

BOOL CCharStatus::IsCheckStatusforSkill(unsigned short wSkill)
{
	const Skill::ProtoType* lpSkill = g_SkillMgr.GetSkillProtoType(wSkill);
	if (NULL == lpSkill) 
	{
		return FALSE;
	}

	SKILLSLOT* lpSkillSlot = GetSkillSlot(lpSkill->m_usSkill_ID);
	if (NULL != lpSkillSlot)
	{
		for (int i = 0; i < Skill::ProtoType::MAX_LIMIT_NUM; ++i)
		{
			if (lpSkill[lpSkillSlot->SKILLINFO.cLockCount].m_StatusLimitType[i])
			{
				unsigned long dwValue = (lpSkillSlot->SKILLINFO.cLockCount * CSkillMgr::MAX_SKILL_LEVEL + 
					lpSkillSlot->SKILLINFO.cSkillLevel) * lpSkill[lpSkillSlot->SKILLINFO.cLockCount].m_StatusLimitValue[i];

				switch (lpSkill[lpSkillSlot->SKILLINFO.cLockCount].m_StatusLimitType[i])
				{
					case Skill::StatusLimit::STR:	if (m_Info.STR - 20 < static_cast<unsigned short>(dwValue))	{ return FALSE;	}	break;
					case Skill::StatusLimit::DEX:	if (m_Info.DEX - 20 < static_cast<unsigned short>(dwValue))	{ return FALSE;	}	break;
					case Skill::StatusLimit::CON:	if (m_Info.CON - 20 < static_cast<unsigned short>(dwValue))	{ return FALSE;	}	break;
					case Skill::StatusLimit::INT:	if (m_Info.INT - 20 < static_cast<unsigned short>(dwValue))	{ return FALSE;	}	break;
					case Skill::StatusLimit::WIS:	if (m_Info.WIS - 20 < static_cast<unsigned short>(dwValue))	{ return FALSE;	}	break;
					case Skill::StatusLimit::LEVEL:	if (m_Info.Level < lpSkill->m_StatusLimitValue[i])			{ return FALSE;	}	break;	//--//
				}
			}
		}
	}

	return TRUE;
}

unsigned long CCharStatus::IsCheckClassforSkill(unsigned long dwClassLimit)
{		
	if(!dwClassLimit)
	{
		return 0;
	}

	if (0 == (dwClassLimit & (0x00000001 << (m_Info.Class - 1)))) 
	{
		return 3;
	}
	else
	{
		return 1;
	}	
}

long CCharStatus::GetUseIP(unsigned long dwKindStatus)
{
	switch (m_Info.Class)
	{
		case 1:
			if (dwKindStatus == STATUS_STR || dwKindStatus == STATUS_CON) return 2; else return 1;
			break;

		case 2:
			if (dwKindStatus == STATUS_DEX || dwKindStatus == STATUS_STR) return 2; else return 1;
			break;

		case 3:
			if (dwKindStatus == STATUS_INT || dwKindStatus == STATUS_DEX) return 2; else return 1;
			break;

		case 4:
			if (dwKindStatus == STATUS_WIS || dwKindStatus == STATUS_CON) return 2; else return 1;
			break;

		case 5:
			if (dwKindStatus == STATUS_STR || dwKindStatus == STATUS_CON) return 2; else return 1;
			break;

		case 6:
			if (dwKindStatus == STATUS_STR || dwKindStatus == STATUS_CON) return 2; else return 1;
			break;

		case 7:
			if (dwKindStatus == STATUS_DEX || dwKindStatus == STATUS_STR) return 2; else return 1;
			break;

		case 8:
			if (dwKindStatus == STATUS_DEX || dwKindStatus == STATUS_STR) return 2; else return 1;
			break;

		case 9:
			if (dwKindStatus == STATUS_INT || dwKindStatus == STATUS_DEX) return 2; else return 1;
			break;

		case 10:
			if (dwKindStatus == STATUS_INT || dwKindStatus == STATUS_DEX) return 2; else return 1;
			break;

		case 11:
			if (dwKindStatus == STATUS_WIS || dwKindStatus == STATUS_CON) return 2; else return 1;
			break;

		case 12:
			if (dwKindStatus == STATUS_WIS || dwKindStatus == STATUS_CON) return 2; else return 1;
			break;

		// 아칸
		case 17:
			if (dwKindStatus == STATUS_STR) return 2; else return 1;
//			if (dwKindStatus == STATUS_STR || dwKindStatus == STATUS_CON || dwKindStatus == STATUS_DEX) return 2; else return 1;
			break;

		case 18:
			if (dwKindStatus == STATUS_DEX) return 2; else return 1;
//			if (dwKindStatus == STATUS_STR || dwKindStatus == STATUS_INT || dwKindStatus == STATUS_WIS || dwKindStatus == STATUS_DEX) return 2; else return 1;
			break;

		case 19:
			if (dwKindStatus == STATUS_STR || dwKindStatus == STATUS_CON) return 2; else return 1;
			break;

		case 20:
			if (dwKindStatus == STATUS_STR || dwKindStatus == STATUS_CON) return 2; else return 1;
			break;

		case 21:
			if (dwKindStatus == STATUS_STR || dwKindStatus == STATUS_DEX) return 2; else return 1;
			break;

		case 22:
			if (dwKindStatus == STATUS_INT || dwKindStatus == STATUS_DEX) return 2; else return 1;
			break;

		case 23:
			if (dwKindStatus == STATUS_WIS || dwKindStatus == STATUS_DEX) return 2; else return 1;
			break;

		case 24:
			if (dwKindStatus == STATUS_STR || dwKindStatus == STATUS_DEX) return 2; else return 1;
			break;
	}

	return 0;
}

unsigned long CCharStatus::CheckUseSkillBook(CItemInstance *lpItem)
{
	CRYLGameData* pGame = CRYLGameData::Instance() ;

	// 어빌리티북과 스킬북을때 따로 처리하게 한다.

	// 클래스 구분
	unsigned short wSkill = 0;
	if (Item::ItemType::SKILL_BOOK == lpItem->GetItemType())
	{
		wSkill = lpItem->GetSkillID();
	}

	if(wSkill >= 0x8000 && m_Skill.wSkillNum < pGame->m_csStatus.GetSkillPoint())
	{
		const Item::ItemInfo* lpBase = Item::CItemMgr::GetInstance().GetItemInfo(lpItem->GetProtoTypeID());

		unsigned long dwClassLimit = 0;

		if(lpBase)
		{
			dwClassLimit = lpBase->m_UseLimit.m_dwClassLimit;
		}

		unsigned long dwClassResult = IsCheckClassforSkill(dwClassLimit);

		if (dwClassResult != 1)
		{
			switch (dwClassResult)
			{
				case 0:		return 7;
				case 2:		return 8;
				case 3:		return 9;
			}

			return 0;
		}

		SKILLSLOT* lpSkillSlot = GetSkillSlot(wSkill);
		if (lpSkillSlot)
		{
			if (lpSkillSlot->SKILLINFO.cLockCount == lpItem->m_lpItemBase->GetItemInfo().m_UseItemInfo.m_usSkill_LockCount)
			{
				if (lpSkillSlot->SKILLINFO.cSkillLevel < 6)
				{
					if (IsCheckStatusforSkill(wSkill))
					{
						return 1;
					} 
					else
					{
						// 능력치가 적어 외울 수 없습니다.
						return 2;
					}
				} 
				else
				{
					// 스킬이 꽉 찼습니다.
					return 3;
				}
			} 
			else
			{
				// 외울 수 있는 스킬이 아닙니다.
				return 4;
			}
		} 
		else
		{
			if (m_Skill.wSlotNum < SKILL::MAX_SLOT_NUM)
			{
				if (lpItem->m_lpItemBase->GetItemInfo().m_UseItemInfo.m_usSkill_LockCount == 0)
				{
					if (IsCheckStatusforSkill(wSkill))
					{
						return 1;
					} 
					else
					{
						return 2;
					}
				} 
				else
				{
					return 4;
				}
			} 
			else
			{
				// 스킬 슬롯이 꽉 찼습니다.
				return 5;
			}
		}
	} 
	else if(0x1000 <= wSkill && wSkill < 0x2000 )	// 어빌리티북일때
	{
		const Item::ItemInfo* lpBase = Item::CItemMgr::GetInstance().GetItemInfo(lpItem->GetProtoTypeID());

		SKILLSLOT* lpSkillSlot = GetSkillSlot(wSkill);
		if (lpSkillSlot)	// 이미 배운 스킬이면
		{
			// 내가 가지고 있는 락카운트보다 더 높은 스킬이면 배우면 된다.
			if (lpSkillSlot->SKILLINFO.cLockCount+1 == lpItem->m_lpItemBase->GetItemInfo().m_UseItemInfo.m_usSkill_LockCount)
			{
				return 1;
			} 
			else if (lpSkillSlot->SKILLINFO.cLockCount+1 < lpItem->m_lpItemBase->GetItemInfo().m_UseItemInfo.m_usSkill_LockCount)
			{
				// 어빌리티 레벨이 부족하여 배울수 없습니다.
				return 2;
			}
			else
			{
				// 이단계의 스킬은 배웠습니다.
				return 3;
			}
		} 
		else
		{
			// 슬롯이 남으면 
			if (m_Skill.wSlotNum < SKILL::MAX_SLOT_NUM)
			{
				// 사용한 스킬이 0이면 
				if (lpItem->m_lpItemBase->GetItemInfo().m_UseItemInfo.m_usSkill_LockCount == 0)
				{
					return 1;
				} 
				else
				{
					return 4;
				}
			} 
			else
			{
				// 스킬 슬롯이 꽉 찼습니다.
				return 5;
			}
		}
	}
	// 레벨이 너무 낮습니다.
	return 6;
}

bool CCharStatus::CheckUsePotion(CItemInstance *lpItem)
{
	// 쿨다운 타임을 체크한 후 사용 유무를 결정한다.
	Item::CUseItem *lpPotion = Item::CUseItem::DowncastToUseItem(lpItem->m_lpItemBase);
	if (!lpPotion) return false;

	vector<PotionCoolTime>::iterator it;
	PotionCoolTime *lpCool;
	for (it = m_lstPotionCoolTime.begin(); it != m_lstPotionCoolTime.end(); ++it)
	{
		lpCool = &(*it);
		if (lpCool->m_wPotionSkillID == lpPotion->GetItemInfo().m_UseItemInfo.m_usSkill_ID)
		{
			if (lpCool->m_lPotionCoolDownTime > 0)
			{
				return false;
			}

			break;
		}
	}

	return true;
}

BOOL CCharStatus::SetQuickSlotItemNumber(CItemInstance *lpItem)
{
	list<CItemInstance *>::iterator it;
	unsigned long dwItemNumber = 0;
	for (it = m_lstInventory.begin(); it != m_lstInventory.end(); ++it)
	{
		if (lpItem->m_lpItemBase->GetPrototypeID() == (*it)->m_lpItemBase->GetPrototypeID())
		{
			dwItemNumber += (*it)->GetNowDurability();
		}
	}

	if (dwItemNumber == 0) 
	{
		return FALSE;
	}

	if (dwItemNumber > 99)
	{
		lpItem->SetNowDurability(99);
	}
	else
	{
		lpItem->SetNowDurability(dwItemNumber);
	}

	return TRUE;
}

void CCharStatus::DelQuickSlot(unsigned short wSkill)
{
	for (int i = 0; i < QUICK::MAX_QUICK_NUM; ++i)
	{
		if (NULL != m_lstQuickSlot[i])
		{
			if (m_lstQuickSlot[i]->m_lpItemBase->GetPrototypeID() == wSkill)
			{
				if (m_lpQuickSelected == m_lstQuickSlot[i])
					m_lpQuickSelected = NULL;

				CRYLNetworkData::SendQuickSlotMove(m_lstQuickSlot[i], TakeType::TS_NONE, i);
				return;
			}
		}
	}
}

void CCharStatus::SetQuickSlot(CItemInstance *lpItem)
{
	if (true == lpItem->m_lpItemBase->IsSet(Item::DetailData::QUICKSLOT_IN)) 
	{
		for (int i = 0; i < QUICK::MAX_QUICK_NUM; ++i)
		{
			if (NULL != m_lstQuickSlot[i])
			{
				if (m_lstQuickSlot[i]->m_lpItemBase->GetPrototypeID() == lpItem->m_lpItemBase->GetPrototypeID())
				{
					// 퀵슬롯에 등록된 아이템을 더이상 가지고 있지 않은 경우, 퀵슬롯에서 제거
					if (FALSE == SetQuickSlotItemNumber(m_lstQuickSlot[i]))
					{
						if (m_lpQuickSelected == m_lstQuickSlot[i])
						{
							m_lpQuickSelected = NULL;
						}

						CRYLNetworkData::SendQuickSlotMove(m_lstQuickSlot[i], TakeType::TS_NONE, i);
					}
				}
			}
		}
	}
}

BOOL CCharStatus::CheckItemByStatus(CItemInstance *lpItem)
{
	const Item::UseLimit *lpLimit = &lpItem->GetItemInfo().m_UseLimit;

	Item::CEquipment* lpEquip = Item::CEquipment::DowncastToEquipment( lpItem->m_lpItemBase ) ;

	// 2009.08.18 장착레벨 제한 (LimitStatus 제한) 
	int iCurLevel = m_Info.Level;
	if ( lpEquip )
	{
		if(3 == lpEquip->GetSeasonRecord())	// 시즌레코드 3이면 레벨제한을 5 다운시킨다)
		{
			iCurLevel = m_Info.Level+lpEquip->GetCoreLevel();
		}
	}

	// 어빌리티 레벨 보너스
	iCurLevel += GetAbilityValue(Skill::Type::AB_EQUIP_LEVELDOWN);

	switch ( lpLimit->m_cLimitStatus )
	{
	case Item::StatusLimit::NONE:
		break;
	case Item::StatusLimit::STR:
		if ( GetSTR() < lpLimit->m_wLimitValue )	return FALSE;
		break;
	case Item::StatusLimit::DEX:
		if ( GetDEX() < lpLimit->m_wLimitValue )	return FALSE;
		break;
	case Item::StatusLimit::CON:
		if ( GetCON() < lpLimit->m_wLimitValue )	return FALSE;
		break;
	case Item::StatusLimit::INT:
		if ( GetINT() < lpLimit->m_wLimitValue )	return FALSE;
		break;
	case Item::StatusLimit::WIS:
		if ( GetWIS() < lpLimit->m_wLimitValue )	return FALSE;
		break;
	//--//	start..
	case Item::StatusLimit::LEVEL:
		if ( iCurLevel < lpLimit->m_wLimitValue )	return FALSE;
		break;
	//--//	end..
	}

	if ( lpEquip )
	{
		if ( iCurLevel < lpEquip->GetRuneEquipLimitLevel() ) return FALSE;
	}


/*
	if (m_Info.Level < lpLimit->m_nStatusLimit[Item::StatusLimit::LEVEL]) return FALSE;
	if (m_Info.STR < lpLimit->m_nStatusLimit[Item::StatusLimit::STR]) return FALSE;
	if (m_Info.DEX < lpLimit->m_nStatusLimit[Item::StatusLimit::DEX]) return FALSE;
	if (m_Info.CON < lpLimit->m_nStatusLimit[Item::StatusLimit::CON]) return FALSE;
	if (m_Info.INT < lpLimit->m_nStatusLimit[Item::StatusLimit::INT]) return FALSE;
	if (m_Info.WIS < lpLimit->m_nStatusLimit[Item::StatusLimit::WIS]) return FALSE;
*/

/*	if (lpLimit->m_nCanUseRace != Item::Race::ALL)
	{
		switch (lpLimit->m_nCanUseRace)
		{
			case Item::Race::HUMAN:
				if (m_Info.Race != CClass::RaceType::HUMAN) return FALSE;
				break;
			case Item::Race::AKHAN:
				if (m_Info.Race != CClass::RaceType::AKHAN) return FALSE;
				break;
		}
	}*/

	if (!(lpLimit->m_dwClassLimit & (0x00000001 << (m_Info.Class - 1))))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCharStatus::GetExistExchangeItem()
{
	if (m_lstExchange.size() || m_lstExchange2.size() || m_dwExchangeGold || m_dwExchange2Gold)
		return TRUE;
	else
		return FALSE;
}

// 소셜모션을 작동시킨다
BOOL CCharStatus::UseSocialAction(unsigned short wSkill, unsigned short wGrade)
{
	CRYLGameData*		pGame		= CRYLGameData::Instance();

	// 공격처리가 끝나지 않으면 공격하지 않는다.
	if(pGame->m_bSkillFlag)
		return TRUE;

	RYLCreature* pSelfCreature = RYLCreatureManager::Instance()->GetSelfCreature();
	if ( !pSelfCreature )
		return TRUE;

	if ( pSelfCreature->GetIsDead() )
		return TRUE;

    CZ3DGeneralChrModel* pSelfModel = pSelfCreature->GetCharModel();
    if( !pSelfModel )
        return TRUE;

	// 물위에서 수영금지
	CollisionType CT = CSceneManager::GetCharacterCollisionType( pSelfModel );

	if( CT == CT_WATER )
		return TRUE;


	CRYLNetworkData*	pNetwork	= CRYLNetworkData::Instance();

	unsigned short usSkillID = wSkill;
	unsigned long dwLock = wGrade;
	unsigned long dwLevel = wGrade;

	unsigned long dwResult = CheckSkillCooldown(usSkillID); // Minotaurs

	if(dwResult != 0)
	{
		char* lpBuffer = NULL;
		switch (dwResult)
		{
			case 1:		lpBuffer = CRYLStringTable::m_strString[814];					break;
			case 2:		lpBuffer = CRYLStringTable::m_strString[143];					break;
		}

		if (NULL != lpBuffer)
		{
			CRYLChattingDlg* pChat = CRYLChattingDlg::Instance();
			pChat->AddMessage(lpBuffer, pNetwork->m_dwMyChrID, 129);
		}
		return TRUE;
	}

	// 소셜모션은 점프중 사용이 불가능하다.
	if(0x4000 < wSkill && wSkill < 0x5000)
	{
		// 타임체크
		if(pSelfCreature->GetFlying(true))
		{
			CRYLChattingDlg* pChat = CRYLChattingDlg::Instance();
			pChat->AddMessage(CRYLStringTable::m_strString[4238], pNetwork->m_dwMyChrID, 129);

			return TRUE;
		}
	}

	const Skill::ProtoType *pSkillType = g_SkillMgr.GetSkillProtoType(usSkillID);
	if (pSkillType == NULL)
	{
		return TRUE;
	}

	// 스킬 레벨에 맞는 스킬을 지정한다.
	pSkillType = &pSkillType[dwLock];
	if (strcmp(pSkillType->m_szCastingFileName, "NULL"))
	{
		pSelfCreature->EndSkill((char *)pSkillType->m_szCastingFileName);
	}

	pSelfCreature->m_ulSkillComboCount = 0;
	pSelfCreature->m_bSkillCombo = false;

	// 스킬을 집어넣는다.
	CheckUseSkillforStatus(dwLock, dwLevel);

	// edith 2009.08.06 캐릭터가 스킬을 쓸때 동작을 초기화한다.
	CRYLNetworkData::SendMovingPacket( CA_WAIT, CA_WAIT,
		pSelfCreature->GetDirection(), *(pSelfCreature->GetPosition()) );

	pGame->SetSkillCoolDown(usSkillID, dwLock, pSkillType->m_cEndCoolDown);

	pGame->m_lpInterface->m_dwSpecialTargetChrID = pNetwork->m_dwMyChrID;
	
	BOOL bGod = TRUE;		// 무적 상태
	BOOL bCanDead = FALSE;	// 죽은 사람
	g_CharacterData.MakeSkillAttackInfo(pGame->m_lpInterface->m_dwSpecialTargetChrID, 
		usSkillID, dwLock, dwLevel, pSkillType->m_cStrikeNum, bGod, bCanDead);

	pSelfCreature->SetStillCasting( false );

	return TRUE;
}

unsigned long CCharStatus::CheckSkillCooldown(unsigned short wSkill)
{
	int i;

	if (m_lpQuickSelected && m_lpQuickSelected->GetProtoTypeID() == wSkill && m_lpQuickSelected->m_bClassSkill)
	{
		if (m_lpQuickSelected->m_lCoolDownTimeCount <= 0)
			return 0;		
		else
			return 2;		
	} 
	else
	{
		for(i = 0; i < SKILL::MAX_SLOT_NUM; i++)
		{
			if (m_lstSkillSlot[i])
			{
				if (m_lstSkillSlot[i]->GetProtoTypeID() != wSkill)
					continue;

				if (m_lstSkillSlot[i]->m_lCoolDownTimeCount <= 0)
					return 0;		
				else
					return 2;		
			}
		}
	}

	// 소셜모션리스트확인
	if(!m_lstSocialSlot.empty())
	{
		int iSize = m_lstSocialSlot.size();
		for(i = 0; i < iSize; ++i)
		{
			if (m_lstSocialSlot[i]->GetProtoTypeID() != wSkill)
				continue;

			if (m_lstSocialSlot[i]->m_lCoolDownTimeCount <= 0)
				return 0;		
			else
				return 2;		
		}
	}

	for(i = 0; i < QUICK::MAX_QUICK_NUM; i++)
	{
		if (m_lstQuickSlot[i])
		{
			if (m_lstQuickSlot[i]->GetProtoTypeID() != wSkill)
				continue;

			if (m_lstQuickSlot[i]->m_lCoolDownTimeCount <= 0)
				return 0;		
			else
				return 2;		
		}
	}
	
	return 1;
}

unsigned long CCharStatus::CheckUseSkill()
{
	g_CharacterData.m_bComboSuccess = FALSE;

	if (NULL != m_lpQuickSelected)
	{
		if (Item::ItemType::SKILL_ITEM == m_lpQuickSelected->GetItemType())
		{
			// 스킬 아이템은 누구든 언제나 사용 가능
			return 0;
		}

		const Skill::ProtoType *lpSkill = g_SkillMgr.GetSkillProtoType(m_lpQuickSelected->GetProtoTypeID());
		if ( !lpSkill )
		{
			return 6;	// 스킬이 존재 하지 않는다.
		}

		RYLCreature* pSelfCreature = RYLCreatureManager::Instance()->GetSelfCreature();
		if ( !pSelfCreature )
			return 7;	// 내 캐릭터 존재 안함

		CharDataNode* pSelfData = RYLCharacterDataManager::Instance()->GetCharData( pSelfCreature->GetCharID() );
		if ( !pSelfData )
			return 7;

		CRYLGameData* pGame = CRYLGameData::Instance() ;

		switch (m_lpQuickSelected->GetProtoTypeID())
		{
			case 0x8704 :	// 스텔스 (어쌔)
			case 0x9804 :	// 스텔스 (쉐옵)
			case 0x8805 :	// 캐모플라쥐 (아처)
			case 0x9504 :	// 캐모플라쥐 (거너)
			{
				if (!m_bUseStealth)
				{
					return 5 ;	// 스텔스를 사용 할수 없다.
				}
			}
			break;

			case 0x8302 :	// 블러디 마나
			case 0x9206 :	// 블러디 마나
			case 0x9106 :	// 스플릿라이프
			{
				unsigned long dwLock = pGame->m_csStatus.m_lpQuickSelected->GetSkillLockCount();
				unsigned long dwLevel = pGame->m_csStatus.m_lpQuickSelected->GetNowDurability();

				long lHPCost = ( dwLevel + dwLock * 6 ) * 20 * ( 100 + GetMagicPower() ) / 100.f;
				if ( pSelfData->m_lCurrHP <= lHPCost )
				{
					return 8;
				}
			}
			break;

			case 0x8503 :	// 셰어드 폐인
			{
				unsigned long dwLock = pGame->m_csStatus.m_lpQuickSelected->GetSkillLockCount();
				unsigned long dwLevel = pGame->m_csStatus.m_lpQuickSelected->GetNowDurability();

				if(4 <= dwLock)
				{
					if (10000 > pSelfData->m_lCurrHP)
					{
						return 9;
					}
					else if (4000 > pSelfData->m_lCurrMP)
					{
						return 10;
					}
				}
				else
				{
					if (( dwLevel + dwLock * 6 ) * 30 * ( 1 + pSelfData->m_cLevel * 0.05f ) > pSelfData->m_lCurrHP)
					{
						return 8;
					}
				}

				
			}
			break;

			case 0x8B05 :	// 운즈크래프팅
			case 0x8C04 :	// 운즈크래프팅
			case 0x9703 :	// 운즈크래프팅
			{
				unsigned long dwLock = pGame->m_csStatus.m_lpQuickSelected->GetSkillLockCount();
				unsigned long dwLevel = pGame->m_csStatus.m_lpQuickSelected->GetNowDurability();

				long lHPCost = ( dwLevel + dwLock * 6 ) * 10 * ( 100 + GetMagicPower() ) / 100.f;
				if ( pSelfData->m_lCurrHP <= lHPCost )
				{
					return 8;
				}
			}
			break;

			// edith 2009.04.26 점프중 스킬사용 막기
			case 0x8104 :	// 하드히트
			case 0x9104 :	// 하드히트
			case 0x8603 :	// 체인액션
			case 0x9305 :	// 패스트히트
			case 0x9407:	// 쇼크웨이브
			case 0x9402:	// 드레인 파워
			{
				// 타임체크
				if(pSelfCreature->GetFlying(true))
				{
					return 11;
				}
			}
			break;
		}

		// 소셜모션은 점프중 사용이 불가능하다.
		if(0x4000 < m_lpQuickSelected->GetProtoTypeID() && m_lpQuickSelected->GetProtoTypeID() < 0x5000)
		{
			// 타임체크
			if(pSelfCreature->GetFlying(true))
			{
				return 11;
			}
		}


		if (m_lpQuickSelected->m_lCoolDownTimeCount <= 0 || (lpSkill->m_bGauge && g_CharacterData.m_bComboSuccess))
		{
			// 성공
			return 0;		
		} 
		else
		{
			// 쿨다운 타임이 안되었다.
			return 2;		
		}

		// 공성 병기일때는 공성스킬을 사용할수 있도록 리턴한다.
        RYLCreature* pSiegeCreature = RYLCreatureManager::Instance()->GetSelfCreature();
		if ( CRYLGameData::Instance()->GetClientIdentity( pSiegeCreature->GetCharID() ) == Creature::CT_SIEGE_OBJECT )
		{
			return 0 ;
		}
	
		if ( Skill::Type::PASSIVE == lpSkill->m_eSkillType ) 
		{
			// 패시브이면 못 쓴다.
			return 4 ;		
		}

		/*if ( IsCheckClassforSkill( lpSkill->m_usSkill_ID ) != 1 )
		{
			// 클래스가 맞지 않는다.
			return 1 ;		
		}*/
	}

	// 선택된 스킬이 없다.
	return 3 ;		
}


void CCharStatus::SetActivateSkill(CItemInstance *lpUseSkill)
{
	m_lpQuickSelected = lpUseSkill;

    RYLCreature* pSelfCreature = RYLCreatureManager::Instance()->GetSelfCreature();
    if ( !pSelfCreature )
        return;

    pSelfCreature->m_ulSkillComboCount = 0;
    pSelfCreature->m_bSkillCombo = false;
    
	CRYLGameData::SetSkillUseCounter(0);
}

BOOL CCharStatus::CheckStatus()
{
	long TIP = m_Info.IP;

	switch (m_Info.Class)
	{
		case 1:							// 파이터
			TIP += (m_Info.STR - 20 - (m_Info.Level - 1)) * 2;
			TIP += (m_Info.DEX - 20);
			TIP += (m_Info.CON - 20 - (m_Info.Level - 1)) * 2;
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 2:							// 로그
			TIP += (m_Info.STR - 20 - (m_Info.Level - 1)) * 2;
			TIP += (m_Info.DEX - 20 - (m_Info.Level - 1)) * 2;
			TIP += (m_Info.CON - 20);
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 3:							// 메이지
			TIP += (m_Info.STR - 20);
			TIP += (m_Info.DEX - 20 - (m_Info.Level - 1)) * 2;
			TIP += (m_Info.CON - 20);
			TIP += (m_Info.INT - 20 - (m_Info.Level - 1)) * 2;
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 4:							// 어콜라이트
			TIP += (m_Info.STR - 20);
			TIP += (m_Info.DEX - 20);
			TIP += (m_Info.CON - 20 - (m_Info.Level - 1)) * 2;
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20 - (m_Info.Level - 1)) * 2;
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 5:							// 디펜더
			TIP += (m_Info.STR - 20 - int((m_Info.Level - 1) * 1.5)) * 2;
			TIP += (m_Info.DEX - 20);
			TIP += (m_Info.CON - 20 - int((m_Info.Level - 1) * 1.5 + 0.5)) * 2;
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 6:							// 워리어
			TIP += (m_Info.STR - 20 - (m_Info.Level - 1) * 2) * 2;
			TIP += (m_Info.DEX - 20);
			TIP += (m_Info.CON - 20 - (m_Info.Level - 1) * 1) * 2;
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 7:							// 어쌔신
			TIP += (m_Info.STR - 20 - (m_Info.Level - 1) * 1) * 2;
			TIP += (m_Info.DEX - 20 - (m_Info.Level - 1) * 2) * 2;
			TIP += (m_Info.CON - 20);
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 8:							// 아쳐
			TIP += (m_Info.STR - 20 - (int)((m_Info.Level - 1) * 1.5 + 0.5)) * 2;
			TIP += (m_Info.DEX - 20 - (int)((m_Info.Level - 1) * 1.5)) * 2;
			TIP += (m_Info.CON - 20);
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 9:							// 소서러
			TIP += (m_Info.STR - 20);
			TIP += (m_Info.DEX - 20 - (m_Info.Level - 1) * 1) * 2;
			TIP += (m_Info.CON - 20);
			TIP += (m_Info.INT - 20 - (m_Info.Level - 1) * 2) * 2;
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 10:						// 인챈터
			TIP += (m_Info.STR - 20);
			TIP += (m_Info.DEX - 20 - (int)((m_Info.Level - 1) * 1.5 + 0.5)) * 2;
			TIP += (m_Info.CON - 20);
			TIP += (m_Info.INT - 20 - (int)((m_Info.Level - 1) * 1.5)) * 2;
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 11:						// 프리스트
			TIP += (m_Info.STR - 20);
			TIP += (m_Info.DEX - 20);
			TIP += (m_Info.CON - 20 - (int)((m_Info.Level - 1) * 1.5 + 0.5)) * 2;
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20 - (int)((m_Info.Level - 1) * 1.5)) * 2;
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 12:						// 클레릭
			TIP += (m_Info.STR - 20);
			TIP += (m_Info.DEX - 20);
			TIP += (m_Info.CON - 20 - (m_Info.Level - 1) * 1) * 2;
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20 - (m_Info.Level - 1) * 2) * 2;
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 17:						// 컴배턴트
			TIP += (m_Info.STR - 20 - (m_Info.Level - 1)) * 2;
//			TIP += (m_Info.STR - 20);
			TIP += (m_Info.DEX - 20);
//			TIP += (m_Info.CON - 20 - (m_Info.Level - 1)) * 2;
			TIP += (m_Info.CON - 20);
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20);
			if (TIP > 10)
				return FALSE;
			break;

		case 18:						// 오피세이터
			TIP += (m_Info.STR - 20);
			TIP += (m_Info.DEX - 20 - (m_Info.Level - 1)) * 2;
			TIP += (m_Info.CON - 20);
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20);
			if (TIP > 10)
				return FALSE;
			break;

		case 19:						// 템플러
			TIP += (m_Info.STR - 20 - (int)((m_Info.Level - 1) * 1.5)) * 2;
			TIP += (m_Info.DEX - 20);
			TIP += (m_Info.CON - 20 - (int)((m_Info.Level - 1) * 1.5 + 0.5)) * 2;
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 20:						// 어태커
			TIP += (m_Info.STR - 20 - (m_Info.Level - 1) * 2) * 2;
			TIP += (m_Info.DEX - 20);
			TIP += (m_Info.CON - 20 - (m_Info.Level - 1) * 1) * 2;
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 21:						// 거너
//			TIP += (m_Info.STR - 20);
			TIP += (m_Info.STR - 20 - (int)((m_Info.Level - 1) * 1.5 + 0.5)) * 2;
			TIP += (m_Info.DEX - 20 - (int)((m_Info.Level - 1) * 1.5)) * 2;
//			TIP += (m_Info.CON - 20 - (int)((m_Info.Level - 1) * 1.5 + 0.5)) * 2;
			TIP += (m_Info.CON - 20);
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 22:						// 룬 오프
			TIP += (m_Info.STR - 20);
			TIP += (m_Info.DEX - 20 - (m_Info.Level - 1) * 1) * 2;
			TIP += (m_Info.CON - 20);
			TIP += (m_Info.INT - 20 - (m_Info.Level - 1) * 2) * 2;
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 23:						// 라이프 오프
			TIP += (m_Info.STR - 20);
			TIP += (m_Info.DEX - 20 - (m_Info.Level - 1) * 1) * 2;
			TIP += (m_Info.CON - 20);
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20 - (m_Info.Level - 1) * 2) * 2;
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;

		case 24:						// 쉐도우 오프
			TIP += (m_Info.STR - 20 - (int)((m_Info.Level - 1) * 1.5 + 0.5)) * 2;
			TIP += (m_Info.DEX - 20 - (int)((m_Info.Level - 1) * 1.5)) * 2;
			TIP += (m_Info.CON - 20);
			TIP += (m_Info.INT - 20);
			TIP += (m_Info.WIS - 20);
			if (TIP > (10 + (m_Info.Level - 1) * 2))
				return FALSE;
			break;
	}

	return TRUE;
}

void CCharStatus::Destroy()
{
	int i, j, k;

	for (k = 0; k < INVENTORY_TAB; ++k)
	{
		for (j = 0; j < INVENTORY_HEIGHT; ++j)
		{
			for (i = 0; i < INVENTORY_WIDTH; ++i)
			{
				m_aryInven[k][j][i] = ITEM_EMPTY;
			}
		}
	}

	for (k = 0; k < 4; ++k)
	{
		for (j = 0; j < 12; ++j)
		{
			for (i = 0; i < 8; ++i)
			{
				m_aryDeposit[k][j][i] = ITEM_EMPTY;
			}
		}
	}

	for (j = 0; j < 4; ++j)
	{
		for (i = 0; i < 8; ++i)
		{
			m_aryExchange[j][i] = ITEM_EMPTY;
			m_aryExchange2[j][i] = ITEM_EMPTY;
		}
	}

	for (j = 0; j < 8; ++j)
	{
		for (i = 0; i < 10; ++i)
		{
			m_aryStreetTrade[j][i] = ITEM_EMPTY;
		}
	}

	m_dwExchangeGold = 0;
	m_dwExchange2Gold = 0;

	m_dwDepositGold = 0;
	m_cSavePass = 0;
	m_dwMaxTab = 0;
	m_dwDepositFlag = 0;
	m_cUsedDeposit = 0;

	for (i = 0; i < Skill::SpellID::MAX_SPELL_TYPE; ++i)
	{
		m_aryEnchantLevel[i] = 0;
		m_aryEnchantTime[i] = 0;
	}

	m_PartyInfo.m_cMemberNum = 0;
	m_PartyInfo.m_dwPartyID = 0;

	for (i = 0; i < 10; ++i)
	{
		m_PartyInfo.MemberCID[i] = 0;
	}

	vector<PotionCoolTime>::iterator itPotion;
	for (itPotion = m_lstPotionCoolTime.begin(); itPotion != m_lstPotionCoolTime.end(); itPotion++)
	{
		(*itPotion).m_lPotionCoolDownTime = 0;
	}

	list<CItemInstance *>::iterator it;
	CItemInstance *lpItem;
	for (it = m_lstInventory.begin(); it != m_lstInventory.end();)
	{
		lpItem = (*it);
		it = m_lstInventory.erase(it);
		delete lpItem;
		lpItem = NULL ;
	}
	m_lstInventory.clear();

	for (it = m_lstDeposit.begin(); it != m_lstDeposit.end();)
	{
		lpItem = (*it);
		it = m_lstDeposit.erase(it);
		delete lpItem;
		lpItem = NULL ;
	}
	m_lstDeposit.clear();

	for (it = m_lstStreetTrade.begin(); it != m_lstStreetTrade.end();)
	{
		lpItem = (*it);
		it = m_lstStreetTrade.erase(it);
		delete lpItem;
		lpItem = NULL ;
	}
	m_lstStreetTrade.clear();

    vector<CItemInstance *>::iterator tempinven_pos = m_lstTempInventoryItem.begin();
    vector<CItemInstance *>::iterator tempinven_end = m_lstTempInventoryItem.end();
    for (; tempinven_pos != tempinven_end; ++tempinven_pos)
    {
        lpItem = (*tempinven_pos);        
        delete lpItem;        
    }

    m_lstTempInventoryItem.clear();
    lpItem = NULL ;

	list<FriendInfo *>::iterator itFriend;
	FriendInfo *lpFriend;
	for (itFriend = m_lstFriendList.begin(); itFriend != m_lstFriendList.end();)
	{
		lpFriend = (*itFriend);
		itFriend = m_lstFriendList.erase(itFriend);
		delete lpFriend;
		lpFriend = NULL ;
	}
	m_lstFriendList.clear();

	list<BanInfo *>::iterator itBan;
	BanInfo *lpBan;
	for (itBan = m_lstBanList.begin(); itBan != m_lstBanList.end();)
	{
		lpBan = (*itBan);
		itBan = m_lstBanList.erase(itBan);
		delete lpBan;
		lpBan = NULL ;
	}
	m_lstBanList.clear();

	list<LPQuestInstance>::iterator itQuest;
	LPQuestInstance lpQuest;
	for (itQuest = m_lstQuest.begin(); itQuest != m_lstQuest.end();)
	{
		lpQuest = (*itQuest);
		itQuest = m_lstQuest.erase(itQuest);
		delete lpQuest->m_lpQuestIcon;
		lpQuest->m_lpQuestIcon =NULL ;
		delete lpQuest;
		lpQuest = NULL ;
	}
	m_lstQuest.clear();

	for (itQuest = m_lstCompleteQuest.begin(); itQuest != m_lstCompleteQuest.end();)
	{
		lpQuest = (*itQuest);
		itQuest = m_lstQuest.erase(itQuest);
		delete lpQuest->m_lpQuestIcon;
		lpQuest->m_lpQuestIcon = NULL ;
		delete lpQuest;
		lpQuest = NULL ;
	}
	m_lstCompleteQuest.clear();

	for (i = 0; i < Skill::SpellID::MAX_SPELL_TYPE; ++i)
	{
		m_aryEnchantLevel[i] = 0;
		m_aryEnchantTime[i] = 0;
	}

	m_wUseChantSkillID = 0;

	for (i = 0; i < QUICK::MAX_QUICK_NUM; ++i)
	{
		if (m_lstQuickSlot[i])
		{
			delete m_lstQuickSlot[i];
			m_lstQuickSlot[i] = NULL;
		}
	}
	m_lpQuickSelected = NULL;
	for (i = 0; i < SKILL::MAX_SLOT_NUM; ++i)
	{
		if (m_lstSkillSlot[i])
		{
			delete m_lstSkillSlot[i];
			m_lstSkillSlot[i] = NULL;
		}
	}
	vector<CItemInstance *>::iterator itSkill;
	for (itSkill = m_lstClassSkill.begin(); itSkill != m_lstClassSkill.end();)
	{
		lpItem = (*itSkill);
		itSkill = m_lstClassSkill.erase(itSkill);
		delete lpItem;
		lpItem = NULL ;
	}
	for (i = 0; i < 15; ++i)
	{
		if (m_lstEquipment[i])
		{
			delete m_lstEquipment[i];
			m_lstEquipment[i] = NULL;
		}
	}

	if (m_lpUpgrade[0]) { delete m_lpUpgrade[0]; m_lpUpgrade[0] = NULL; }
	if (m_lpUpgrade[1]) { delete m_lpUpgrade[1]; m_lpUpgrade[1] = NULL; }
}

void CCharStatus::PayDepositPrice(unsigned short wSrcPos, unsigned short wDstPos)
{
	if (wDstPos == TakeType::TS_DEPOSIT)
	{
		if (wSrcPos != TakeType::TS_DEPOSIT)
		{
			m_Info.Gold -= (m_Info.Level * 20);
		}
	}
}

void CCharStatus::ChantDisable()
{
	for (unsigned long i = 0; i < SKILL::MAX_SLOT_NUM; ++i)
	{
		if (CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[i].SKILLINFO.wSkill)
		{
			const Skill::ProtoType *lpSkillProtoType = g_SkillMgr.GetSkillProtoType(CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[i].SKILLINFO.wSkill);

			if (lpSkillProtoType && lpSkillProtoType->m_cEndCoolDown)
			{
				CRYLGameData::Instance()->SetSkillCoolDown(CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[i].SKILLINFO.wSkill);
			}
		}
	}

	m_wUseChantSkillID = 0;
}

BOOL CCharStatus::GetCorrectClass(unsigned long dwClassFlag)
{
	if (dwClassFlag & (0x00000001 << (m_Info.Class - 1)))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL QuestInstance::StartPhase(unsigned long dwPhase)
{
	// Trigger_Start를 찾아서 이벤트를 수행시킨다.
	m_lstTriggerCount.clear();

	m_lpPhaseScript = m_lpQuestScript->GetPhase(dwPhase);

	if (m_lpPhaseScript)
	{
		// 페이즈를 수행한다. 
		m_dwPresentPhase = dwPhase;

		vector<LPTriggerNode>::iterator it;
		for (it = m_lpPhaseScript->m_lstTrigger.begin(); it != m_lpPhaseScript->m_lstTrigger.end(); ++it)
		{
			m_lstTriggerCount.push_back((*it)->m_dwMaxCount);
		}

		CRYLGameScene* pScene = static_cast<CRYLGameScene*>( CRYLSceneManager::Instance()->FindScene( GAME_SCENE ) ) ;
		if ( pScene && pScene->GetStatusAndQuestFrameDlg() )
		{
			CRYLStatusAndQuestFrameDlg*	pDlg = static_cast<CRYLStatusAndQuestFrameDlg*>( pScene->GetStatusAndQuestFrameDlg() ) ;
			if ( pDlg->GetExecutionQuestPanel() )
			{
				CRYLExecutionQuestPanel* pPanel = static_cast<CRYLExecutionQuestPanel*>( pDlg->GetExecutionQuestPanel() ) ;
				CRYLExecutionQuestSlot*	 pSlot = pPanel->FindPtr( m_lpQuestScript->m_strQuestTitle ) ;
				if ( pSlot ) pSlot->SetQuestLevelAndPhase( m_lpQuestScript->m_strQuestLevel, dwPhase, m_lpQuestScript->m_wMaxPhase ) ;
			}

            CRYLQuestDetailDataDlg* pDetailDlg = static_cast<CRYLQuestDetailDataDlg*>( pScene->GetQuestDetailDataDlg() ) ;
			if ( pDetailDlg ) pDetailDlg->SetDetailDataDlg( this ) ;
		}
	}

	return TRUE;
}

BOOL QuestInstance::StartTrigger(LPTriggerNode lpTrigger, BOOL bSuccess)
{
	vector<LPEventNode>::iterator it;
	if (bSuccess)
	{
		if (lpTrigger->m_lstMsgBox.size())
		{
			CRYLMessageBox *lpMessageBox = new CRYLMessageBox;
			vector<char *>::iterator it;

			BOOL bModal = TRUE;

			for (it = lpTrigger->m_lstMsgBox.begin(); it != lpTrigger->m_lstMsgBox.end(); ++it)
			{
				if((*it)[0] == 'a')
					bModal = FALSE;

				lpMessageBox->AddWord((*it)+1);
			}
			lpMessageBox->Create( MB_CONFIRM | MB_EXIT | MB_THEATER ) ;
			lpMessageBox->SetModal(bModal);
		}

		for (it = lpTrigger->m_lstEvent.begin(); it != lpTrigger->m_lstEvent.end(); ++it)
		{
			switch ((*it)->m_dwEventKind)
			{
				case EVENT_PHASE:
					EventPhase((*it)->m_dwEventNumber);
					break;

				case EVENT_THEATERMODE:
				{
					CRYLGameData::Instance()->m_bMovieVersion = true;
					CRYLGameData::Instance()->m_bSimpleMode = TRUE;
					CRYLGameScene*		 pScene = ( CRYLGameScene * )CRYLSceneManager::Instance()->FindScene( GAME_SCENE ) ;
					pScene->MovieMode( FALSE ) ;
					break;
				}

				case EVENT_END:
					return FALSE;
			}
		}
	} else
	{
		if (lpTrigger->m_lstFalseMsgBox.size())
		{
			CRYLMessageBox *lpMessageBox = new CRYLMessageBox;
			vector<char *>::iterator it;

			BOOL bModal = TRUE;

			for (it = lpTrigger->m_lstFalseMsgBox.begin(); it != lpTrigger->m_lstFalseMsgBox.end(); ++it)
			{
				if((*it)[0] == 'a')
					bModal = FALSE;

				lpMessageBox->AddWord((*it)+1);
			}
			lpMessageBox->Create( MB_CONFIRM | MB_EXIT ) ;
			lpMessageBox->SetModal(bModal);
		}

		for (it = lpTrigger->m_lstFalseEvent.begin(); it != lpTrigger->m_lstFalseEvent.end(); ++it)
		{
			switch ((*it)->m_dwEventKind)
			{
				case EVENT_PHASE:
					EventPhase((*it)->m_dwEventNumber);
					break;

				case EVENT_THEATERMODE:
				{
					CRYLGameData::Instance()->m_bMovieVersion = true;
					CRYLGameData::Instance()->m_bSimpleMode = TRUE;
					CRYLGameScene*		 pScene = ( CRYLGameScene * )CRYLSceneManager::Instance()->FindScene( GAME_SCENE ) ;
					pScene->MovieMode( FALSE ) ;
					break;
				}

				case EVENT_END:
					return FALSE;
			}
		}
	}

	return TRUE;
}

LPQuestInstance CCharStatus::GetProcessQuest(unsigned short wQuestID)
{
	list<LPQuestInstance>::iterator it;
	for (it = m_lstQuest.begin(); it != m_lstQuest.end(); ++it)
	{
		if ((unsigned short)(*it)->m_dwQuestID == wQuestID) return (*it);
	}

	return NULL;
}

BOOL QuestInstance::EventMsgBox(char *strWord)
{
	CRYLMessageBox *lpMessageBox = new CRYLMessageBox;
	lpMessageBox->Create(strWord);

	return TRUE;
}

BOOL QuestInstance::EventPhase(unsigned long dwPhase)
{
	StartPhase(dwPhase);

	return TRUE;
}

LPTriggerNode CCharStatus::GetTriggertoTalk(LPQuestInstance &lpQuest, unsigned long *dwCount, unsigned long dwChrID)
{
	list<LPQuestInstance>::iterator itQuest;
	LPTriggerNode lpTriggerNode;
	for (itQuest = m_lstQuest.begin(); itQuest != m_lstQuest.end(); itQuest++)
	{
		lpTriggerNode = GetTriggertoTalkInQuest((*itQuest), dwCount, dwChrID);
		if (lpTriggerNode)
		{
			return lpTriggerNode;
		}
	}

	return NULL;
}

LPTriggerNode CCharStatus::GetTriggertoTalkInQuest(LPQuestInstance lpQuest, unsigned long *dwCount, unsigned long dwChrID)
{
	if(lpQuest->m_lpPhaseScript == NULL)
		return NULL;

	unsigned long dwTriggerCount;
	for (dwTriggerCount = 0; dwTriggerCount < lpQuest->m_lpPhaseScript->m_lstTrigger.size(); dwTriggerCount++)
	{
		if (lpQuest->m_lstTriggerCount[dwTriggerCount] > 0)
		{
			if (lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwTriggerKind == TRIGGER_TALK &&
				lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwTriggerID == dwChrID)
			{
				*dwCount = dwTriggerCount;

				return lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount];
			}
		}
	}

	return NULL;
}

LPTriggerNode CCharStatus::GetTriggertoLevelTalk(LPQuestInstance &lpQuest, unsigned long *dwCount, unsigned long dwChrID)
{
	list<LPQuestInstance>::iterator itQuest;
	LPTriggerNode lpTriggerNode;
	for (itQuest = m_lstQuest.begin(); itQuest != m_lstQuest.end(); itQuest++)
	{
		lpTriggerNode = GetTriggertoLevelTalkInQuest((*itQuest), dwCount, dwChrID);
		if (lpTriggerNode)
		{
			return lpTriggerNode;
		}
	}

	return NULL;
}

LPTriggerNode CCharStatus::GetTriggertoLevelTalkInQuest(LPQuestInstance lpQuest, unsigned long *dwCount, unsigned long dwChrID)
{
	if(lpQuest->m_lpPhaseScript == NULL)
		return NULL;

	unsigned long dwTriggerCount;
	for (dwTriggerCount = 0; dwTriggerCount < lpQuest->m_lpPhaseScript->m_lstTrigger.size(); dwTriggerCount++)
	{
		if (lpQuest->m_lstTriggerCount[dwTriggerCount] > 0)
		{
			if (lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwTriggerKind == TRIGGER_LEVEL_TALK &&
				lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwTriggerID == dwChrID)
			{
#ifdef __NEWNPCSCRIPT
				*dwCount = dwTriggerCount;

				return lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount];
#else
				if (lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwLevel <= static_cast<unsigned long>(m_Info.Level))
				{
					*dwCount = dwTriggerCount;

					return lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount];
				}
				else
				{
					if (!lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_strElseMsg.empty())
					{
						CRYLMessageBox* pMessageBox = new CRYLMessageBox;
						pMessageBox->Create(lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_strElseMsg.c_str());
					}
				}
#endif
			}
		}
	}

	return NULL;
}

LPTriggerNode CCharStatus::GetTriggertoFameTalk(LPQuestInstance &lpQuest, unsigned long *dwCount, unsigned long dwChrID)
{
	list<LPQuestInstance>::iterator itQuest;
	LPTriggerNode lpTriggerNode;
	for (itQuest = m_lstQuest.begin(); itQuest != m_lstQuest.end(); itQuest++)
	{
		lpTriggerNode = GetTriggertoFameTalkInQuest((*itQuest), dwCount, dwChrID);
		if (lpTriggerNode)
		{
			return lpTriggerNode;
		}
	}

	return NULL;
}

LPTriggerNode CCharStatus::GetTriggertoFameTalkInQuest(LPQuestInstance lpQuest, unsigned long *dwCount, unsigned long dwChrID)
{
	if(lpQuest->m_lpPhaseScript == NULL)
		return NULL;

	unsigned long dwTriggerCount;
	for (dwTriggerCount = 0; dwTriggerCount < lpQuest->m_lpPhaseScript->m_lstTrigger.size(); dwTriggerCount++)
	{
		if (lpQuest->m_lstTriggerCount[dwTriggerCount] > 0)
		{
			if (lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwTriggerKind == TRIGGER_FAME_TALK &&
				lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwTriggerID == dwChrID)
			{
#ifdef __NEWNPCSCRIPT
				*dwCount = dwTriggerCount;

				return lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount];
#else
				if (lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwLevel <= static_cast<unsigned long>(m_Info.Fame))
				{
					*dwCount = dwTriggerCount;

					return lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount];
				}
				else
				{
					if (!lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_strElseMsg.empty())
					{
						CRYLMessageBox* pMessageBox = new CRYLMessageBox;
						pMessageBox->Create(lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_strElseMsg.c_str());
					}
				}
#endif
			}
		}
	}

	return NULL;
}


LPTriggerNode CCharStatus::GetTriggertoAbilityTalk(LPQuestInstance &lpQuest, unsigned long *dwCount, unsigned long dwChrID)
{
	list<LPQuestInstance>::iterator itQuest;
	LPTriggerNode lpTriggerNode;
	for (itQuest = m_lstQuest.begin(); itQuest != m_lstQuest.end(); itQuest++)
	{
		lpTriggerNode = GetTriggertoAbilityTalkInQuest((*itQuest), dwCount, dwChrID);
		if (lpTriggerNode)
		{
			return lpTriggerNode;
		}
	}

	return NULL;
}

LPTriggerNode CCharStatus::GetTriggertoAbilityTalkInQuest(LPQuestInstance lpQuest, unsigned long *dwCount, unsigned long dwChrID)
{
	if(lpQuest->m_lpPhaseScript == NULL)
		return NULL;

	unsigned long dwTriggerCount;
	for (dwTriggerCount = 0; dwTriggerCount < lpQuest->m_lpPhaseScript->m_lstTrigger.size(); dwTriggerCount++)
	{
		if (lpQuest->m_lstTriggerCount[dwTriggerCount] > 0)
		{
			if (lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwTriggerKind == TRIGGER_ABILITY_TALK &&
				lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwTriggerID == dwChrID)
			{
#ifdef __NEWNPCSCRIPT
				*dwCount = dwTriggerCount;

				return lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount];
#else

				// 스킬을 뒤지자
				int iIndex = 0;
				for(iIndex = 0; iIndex <= m_Skill.wSlotNum; ++iIndex)
				{
					if(m_Skill.SSlot[iIndex].SKILLINFO.wSkill == lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwZoneID && 
						(unsigned long)(m_Skill.SSlot[iIndex].SKILLINFO.cLockCount) >= lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwLevel)
					{
						*dwCount = dwTriggerCount;

						return lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount];
					}
				}

				// 실패
				if (!lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_strElseMsg.empty())
				{
					CRYLMessageBox* pMessageBox = new CRYLMessageBox;
					pMessageBox->Create(lpQuest->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_strElseMsg.c_str());
				}
#endif
			}
		}
	}

	return NULL;
}

static BOOL GetInSphere(vector3 vecCenter, float fRadius, vector3 vecPos)
{
	vector3 vecRay = vecPos - vecCenter;
	if (vecRay.GetLens() <= fRadius) return TRUE;

	return FALSE;
}

LPTriggerNode CCharStatus::GetTriggertoGeton(LPQuestInstance &lpQuest, unsigned long *dwCount, vector3 vecPos)
{
	list<LPQuestInstance>::iterator itQuest;
	unsigned long dwTriggerCount;
	vector3 vecCenter;
	for (itQuest = m_lstQuest.begin(); itQuest != m_lstQuest.end(); itQuest++)
	{
		if(!(*itQuest)->m_lpPhaseScript)
			continue;

		for (dwTriggerCount = 0; dwTriggerCount < (*itQuest)->m_lpPhaseScript->m_lstTrigger.size(); dwTriggerCount++)
		{
			if ((*itQuest)->m_lstTriggerCount[dwTriggerCount] > 0)
			{
				if ((*itQuest)->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwTriggerKind == TRIGGER_GETON &&
					(*itQuest)->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_dwZoneID == CRYLNetworkData::Instance()->m_dwPresentZone)
				{
					vecCenter.x = (*itQuest)->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_fPosX;
					vecCenter.y = (*itQuest)->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_fPosY;
					vecCenter.z = (*itQuest)->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_fPosZ;
					if (GetInSphere(vecCenter, (*itQuest)->m_lpPhaseScript->m_lstTrigger[dwTriggerCount]->m_fDistance, vecPos))
					{
						*dwCount = dwTriggerCount;
						lpQuest = (*itQuest);

						return (*itQuest)->m_lpPhaseScript->m_lstTrigger[dwTriggerCount];
					}
				}
			}
		}
	}

	return NULL;
}

LPQuestNode	CCharStatus::GetQuestToGeton(vector3 vecPos)
{
	if (g_QuestList.m_lstQuestNode.empty())
	{
		return NULL;
	}

	LPQuestNode lpQuest = NULL;
	std::vector<LPQuestNode>::iterator itQuest = g_QuestList.m_lstQuestNode.begin();

	unsigned short aryComplete[1000], aryQuest[QuestInstance::MAX_EXECUTING_QUEST];
	unsigned long i;
	ZeroMemory(aryComplete, sizeof(aryComplete));
	ZeroMemory(aryQuest, sizeof(aryQuest));
	list<LPQuestInstance>::iterator it;
	for (it = m_lstQuest.begin(), i = 0; it != m_lstQuest.end(); ++it, ++i)
	{
		aryQuest[i] = (*it)->m_dwQuestID;
	}

	for (it = m_lstCompleteQuest.begin(), i = 0; it != m_lstCompleteQuest.end(); ++it, ++i)
	{
		aryComplete[i] = (*it)->m_dwQuestID;
	}

	vector3 vecCenter;
	while (itQuest != g_QuestList.m_lstQuestNode.end())
	{
		lpQuest = (*itQuest);
		if (lpQuest)
		{
			if (lpQuest->GetQuestType() == AREA_QUEST && lpQuest->m_dwZoneID == CRYLNetworkData::Instance()->m_dwPresentZone)
			{
				if (QuestNode::RES_SUCCESS == lpQuest->CheckQuest(m_Info.Level, m_Info.Fame, m_Info.Class,
																  CRYLGameData::Instance()->m_cAccountNation,
																  aryComplete, (unsigned short)i, aryQuest))
				{
					vecCenter.x = lpQuest->m_fPosX;
					vecCenter.y = lpQuest->m_fPosY;
					vecCenter.z = lpQuest->m_fPosZ;
					if (GetInSphere(vecCenter, lpQuest->m_fDistance, vecPos))
					{
						return lpQuest;
					}
				}
			}
		}

		++itQuest;
	}

	return NULL;
}

BOOL CCharStatus::AddFriend(unsigned long dwChrID, char *strName, unsigned long dwGID, unsigned short wClass,
							char cLevel, unsigned long dwServerID)
{
	if (!GetIsFriend(dwChrID))
	{
		FriendInfo *lpFriend = new FriendInfo;
		lpFriend->m_dwCID		= dwChrID;
		lpFriend->m_dwGID		= dwGID;
		lpFriend->m_wClass		= wClass;
		lpFriend->m_cLevel		= cLevel;
		lpFriend->m_dwServerID	= dwServerID;

		strcpy(lpFriend->m_szName, strName);
		lpFriend->SetLoginStatus(true);

		CRYLGameData::Instance()->m_csStatus.m_lstFriendList.push_back(lpFriend);

		return TRUE;
	}

	return FALSE;
}

BOOL CCharStatus::AddBan(unsigned long dwChrID, char *strName, unsigned long dwGID, unsigned short wClass, char cLevel, unsigned long dwServerID)
{
	if (!GetIsBan(dwChrID))
	{
		BanInfo *lpBan		= new BanInfo;
		lpBan->m_dwCID		= dwChrID;
		lpBan->m_dwGID		= dwGID;
		lpBan->m_wClass		= wClass;
		lpBan->m_cLevel		= cLevel;
		lpBan->m_dwServerID	= dwServerID;

		strcpy(lpBan->m_szName, strName);
		CRYLGameData::Instance()->m_csStatus.m_lstBanList.push_back(lpBan);

		return TRUE;
	}

	return FALSE;
}

BOOL CCharStatus::DeleteFriend(unsigned long dwChrID)
{
	list<FriendInfo *>::iterator it;
	for (it = m_lstFriendList.begin(); it != m_lstFriendList.end(); ++it)
	{
		if ((*it)->m_dwCID == dwChrID)
		{
			FriendInfo *lpFriend = (*it);
			m_lstFriendList.erase(it);
			delete lpFriend;
			lpFriend = NULL ;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CCharStatus::DeleteBan(unsigned long dwChrID)
{
	list<BanInfo *>::iterator it;
	for (it = m_lstBanList.begin(); it != m_lstBanList.end(); ++it)
	{
		if ((*it)->m_dwCID == dwChrID)
		{
			BanInfo *lpBan = (*it);
			m_lstBanList.erase(it);
			delete lpBan;
			lpBan = NULL ;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CCharStatus::GetIsFriend(unsigned long dwChrID)
{
	list<FriendInfo *>::iterator it;
	for (it = m_lstFriendList.begin(); it != m_lstFriendList.end(); ++it)
	{
		if ((*it)->m_dwCID == dwChrID) return TRUE;
	}

	return FALSE;
}

BOOL CCharStatus::GetIsBan(unsigned long dwChrID)
{
	list<BanInfo *>::iterator it;
	for (it = m_lstBanList.begin(); it != m_lstBanList.end(); ++it)
	{
		if ((*it)->m_dwCID == dwChrID) return TRUE;
	}

	return FALSE;
}

BanInfo	*CCharStatus::GetBan(unsigned long dwChrID)
{
	list<BanInfo *>::iterator it;
	for (it = m_lstBanList.begin(); it != m_lstBanList.end(); ++it)
	{
		if ((*it)->m_dwCID == dwChrID) return (*it);
	}

	return NULL;
}

FriendInfo *CCharStatus::GetFriend(unsigned long dwChrID)
{
	list<FriendInfo *>::iterator it;
	for (it = m_lstFriendList.begin(); it != m_lstFriendList.end(); ++it)
	{
		if ((*it)->m_dwCID == dwChrID) return (*it);
	}

	return NULL;
}

FriendInfo *CCharStatus::GetFriendtoNum(unsigned long dwNum)
{
	list<FriendInfo *>::iterator it;
	unsigned long dwCount;
	for (it = m_lstFriendList.begin(), dwCount = 0; it != m_lstFriendList.end(); ++it, dwCount++)
	{
		if (dwCount < dwNum) continue;

		return (*it);
	}

	return NULL;
}

BanInfo *CCharStatus::GetBantoNum(unsigned long dwNum)
{
	list<BanInfo *>::iterator it;
	unsigned long dwCount;
	for (it = m_lstBanList.begin(), dwCount = 0; it != m_lstBanList.end(); ++it, dwCount++)
	{
		if (dwCount < dwNum) continue;

		return (*it);
	}

	return NULL;
}

INT	 CCharStatus::DeleteQuest(LPQuestInstance lpDeleteQuest)
{
	list<LPQuestInstance>::iterator		it = m_lstQuest.begin() ; 
	INT iIndex = 0 ;

	while( it != m_lstQuest.end() )
	{
		LPQuestInstance lpDestQuest = ( *it ) ;	
		
		if ( lpDestQuest == lpDeleteQuest )
		{
			/*
			GM_DELETE( lpDestQuest->m_lpQuestIcon ) ;
			GM_DELETE( lpDestQuest ) ;
			*/
			m_lstQuest.erase( it ) ;
			
			return iIndex ;
		}
		++ it ;
		++ iIndex ;
	}
	
	return iIndex;
}

void CCharStatus::AddCompleteQuest(LPQuestInstance lpDeleteQuest)
{
	m_lstCompleteQuest.push_front(lpDeleteQuest);

    StatusUpdate( STATUS_UPDATE_BASE, true, true );

    // 퀘스트 수행으로 받은 보너스 스킬 포인트를 책정.
    for ( list<LPQuestInstance>::iterator itQuest = m_lstCompleteQuest.begin() ; itQuest != m_lstCompleteQuest.end() ; ++itQuest )    
    {
        if ( (*itQuest)->m_lpQuestScript->m_usBonusSkillPoint != 0 )
            m_statInfoBase.m_wSkillPoint += (*itQuest)->m_lpQuestScript->m_usBonusSkillPoint;

        if ( (*itQuest)->m_lpQuestScript->m_usBonusAbilityPoint != 0 )
            m_iAbilityPoint += (*itQuest)->m_lpQuestScript->m_usBonusAbilityPoint;
    }

    StatusUpdate( STATUS_UPDATE_EQUIP, true, true );
}

unsigned short CCharStatus::GetSkillIDbyChantID(unsigned char cChant)
{
	switch(cChant)
	{
	case Skill::SpellID::CD_8D12:	return 0x8D12;
	case Skill::SpellID::CD_8D14:	return 0x8D14;
	case Skill::SpellID::CD_8D16:	return 0x8D16;
	case Skill::SpellID::CD_99A1:	return 0x99A1;
	case Skill::SpellID::CD_99A2:	return 0x99A2;
	case Skill::SpellID::CD_99A3:	return 0x99A3;
	}


	unsigned short wSkill;

	vector<CItemInstance *>::iterator it;
	for (it = m_lstClassSkill.begin(); it != m_lstClassSkill.end(); ++it)
	{
		wSkill = (*it)->GetSkillIDbyChantID(cChant);
		if (wSkill)
		{
			return wSkill;
		}
	}

	for (int i = 0; i < SKILL::MAX_SLOT_NUM; ++i)
	{
		if (CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i])
		{
			wSkill = CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i]->GetSkillIDbyChantID(cChant);
			if (wSkill)
			{
				return wSkill;
			}
		}
	}

	return 0;
}

void CCharStatus::UpdateQuickSlot(unsigned short wSkill, unsigned short wGrade)
{
	// 퀵 슬롯 상태 변화
	const Skill::ProtoType *lpSkillBase  = g_SkillMgr.GetSkillProtoType( wSkill ) ;

	long lCoolTime;
	for(int i = 0; i < QUICK::MAX_QUICK_NUM; i++)
	{
		if (m_lstQuickSlot[i] &&
			m_lstQuickSlot[i]->m_lpItemBase->GetPrototypeID() == wSkill)
		{
			lCoolTime = m_lstQuickSlot[i]->m_lCoolDownTimeCount;

			BOOL bSelected = false;
			if (m_lpQuickSelected == m_lstQuickSlot[i])
			{
				bSelected = true;
				CRYLNetworkData::SetChant(NULL);
				SetActivateSkill(NULL);
			}

			// 현재 스킬이 사용중이라서 쿨다운 타임에 변경을 줄수 있기 때문에 이전 값들을 기억해서 다시 설정해준다.
			long 			lPrevCoolDownTime		= m_lstQuickSlot[i]->m_lCoolDownTime ;
			long 			lPrevCoolDownTimeCount	= m_lstQuickSlot[i]->m_lCoolDownTimeCount ;
			unsigned char	cPrevCountCooldown		= m_lstQuickSlot[i]->m_cCountCooldown ;

			delete m_lstQuickSlot[i];
			m_lstQuickSlot[i] = new CItemInstance;
			m_lstQuickSlot[i]->SetSkillInfo(wSkill,
					wGrade,
					1,
					TakeType::TS_QSLOT, i, lCoolTime);

			// 현재 스킬이 사용중이라서 쿨다운 타임에 변경을 줄수 있기 때문에 이전 값들을 기억해서 다시 설정해준다.
			m_lstQuickSlot[i]->m_lCoolDownTime		= lPrevCoolDownTime ;
			m_lstQuickSlot[i]->m_lCoolDownTimeCount	= lPrevCoolDownTimeCount ;
			m_lstQuickSlot[i]->m_cCountCooldown		= cPrevCountCooldown ;

			if (bSelected)
			{
				CRYLNetworkData::SetChant(m_lstQuickSlot[i]);
				SetActivateSkill(m_lstQuickSlot[i]);
			}
		}	
	}

}

void CCharStatus::CheckGratherQuickSlot()
{
	// 관계없는 퀵슬롯은
	if(GetAbilityValue(Skill::Type::AB_HERBS))
		DelQuickSlot(0x9E01);
	else
		DelQuickSlot(0x9E03);

	if(GetAbilityValue(Skill::Type::AB_MINING))
		DelQuickSlot(0x9E02);
	else
		DelQuickSlot(0x9E04);

	// 스킬슬롯 갱신
	int iSize = m_lstClassSkill.size();

	for (int i = 0; i < iSize; ++i)
	{
		if(m_lstClassSkill[i]->m_lpItemBase->GetPrototypeID() >= 0x9E03 )
		{
			UpdateQuickSlot(m_lstClassSkill[i]->m_lpItemBase->GetPrototypeID(), m_lstClassSkill[i]->GetSkillGrade());
		}
	}

}

void CCharStatus::SetClassSkill(void)
{
	unsigned short wClassSkill[20];
	unsigned char cSkillNum;

	vector<CItemInstance *>::iterator itSkill;
	CItemInstance *lpSkill;
	for (itSkill = m_lstClassSkill.begin(); itSkill != m_lstClassSkill.end();)
	{
		lpSkill = (*itSkill);
		itSkill = m_lstClassSkill.erase(itSkill);
		delete lpSkill;
		lpSkill = NULL ;
	}
	m_lstClassSkill.clear();

	g_SkillMgr.GetClassSkill(m_Info.Class, wClassSkill, &cSkillNum);

	CRYLGameScene*		 pScene				= ( CRYLGameScene * )CRYLSceneManager::Instance()->FindScene( GAME_SCENE ) ;
	CRYLHumanInventoryFrameDlg* pDlg		= ( CRYLHumanInventoryFrameDlg* )pScene->GetHumanInventoryFrameDlg() ;
	CRYLSkillPanel*		pPanel				= ( CRYLSkillPanel* )pDlg->GetSkillPanel() ;
	CRYLAbilityPanel*	pAbilityPanel		= ( CRYLAbilityPanel* )pDlg->GetAbilityPanel() ;
	CRYLSocialActionPanel*	pSocialPanel	= ( CRYLSocialActionPanel* )pDlg->GetSocialActionPanel() ;

	pPanel->ClearSkillSlot() ;
	pPanel->ClearVirtualSkill() ;

	pAbilityPanel->ClearAbilitySlot();
	pAbilityPanel->ClearVirtualAbility();

	pSocialPanel->ClearSocialAction();

	// 어빌리티 포인트 계산
	CalculateAbility(m_Skill);

	// 기본스킬을 준다. ClassSkill같이 준다.

	// 채집을 준다.
	if(0)	// 채광과 채집을 자동으로 분배한다.
	{
		unsigned short usSkillID = 0x9E01;
		unsigned char cGrade = 0;

		if(m_AbilityValue[Skill::Type::AB_HERBS])
		{
			usSkillID = 0x9E03;
			cGrade = m_AbilityValue[Skill::Type::AB_HERBS]-1;
		}

		CItemInstance *lpClassSkill = new CItemInstance;
		lpClassSkill->SetSkillInfo(usSkillID, cGrade, 1, TakeType::TS_SSLOT, m_lstClassSkill.size(), 0);
		m_lstClassSkill.push_back(lpClassSkill);
		// 스킬 추가
		pPanel->InsertSkillSlot( lpClassSkill, 
								usSkillID,
								cGrade,
								1,
								TakeType::TS_SSLOT, m_lstClassSkill.size(), CLASSSKILL ) ;





		usSkillID = 0x9E02;	// 채굴
		cGrade = 0;

		if(m_AbilityValue[Skill::Type::AB_MINING])
		{
			usSkillID = 0x9E04;	
			cGrade = m_AbilityValue[Skill::Type::AB_MINING]-1;
		}

		lpClassSkill = new CItemInstance;
		lpClassSkill->SetSkillInfo(usSkillID, cGrade, 1, TakeType::TS_SSLOT, m_lstClassSkill.size(), 0);
		m_lstClassSkill.push_back(lpClassSkill);
		// 스킬 추가
		pPanel->InsertSkillSlot( lpClassSkill, 
								usSkillID,
								cGrade,
								1,
								TakeType::TS_SSLOT, m_lstClassSkill.size(), CLASSSKILL ) ;
	
	}


	if (cSkillNum)
	{
		unsigned long i;
		for (i = 0; i < cSkillNum; ++i)
		{
			if (wClassSkill[i])
			{
				CItemInstance *lpClassSkill = new CItemInstance;
				lpClassSkill->SetSkillInfo(wClassSkill[i], 0, 1, TakeType::TS_SSLOT, m_lstClassSkill.size(), 0);
				m_lstClassSkill.push_back(lpClassSkill);
				// 스킬 추가
				pPanel->InsertSkillSlot( lpClassSkill, 
										 wClassSkill[i],
										 0,
										 1,
										 TakeType::TS_SSLOT, m_lstClassSkill.size(), CLASSSKILL ) ;

			}
		}
	}


	for (int i = 0; i < SKILL::MAX_SLOT_NUM; ++i)
	{
		WORD wSkillID = CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[i].SKILLINFO.wSkill ;
		// 해당 아이디가 스킬 아이디면
		if ( wSkillID >= 0x8000 )
		{
			if ( CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i] )
			{
				delete CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i] ;
				CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i] = NULL ;
			}

			CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i] = new CItemInstance;
			CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i]->SetSkillInfo( 
				CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.wSkill,				// 스킬 ID
				CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.cLockCount,			// Lock Count
				CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.cSkillLevel,			// 스킬k 레벨
				TakeType::TS_SSLOT, i, 0 ) ;												// 위치, 인덱스, 쿨다운 타임

			// 스킬 추가
			pPanel->InsertSkillSlot( CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i], 
				CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.wSkill,
				CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.cLockCount,
				CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.cSkillLevel,
				TakeType::TS_SSLOT, i, GENERALSKILL ) ;

			pPanel->InsertVirtualSkill( CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[ i ], false ) ;
			
			const Skill::ProtoType *lpSkillBase = g_SkillMgr.GetSkillProtoType( wSkillID ) ;//, *lpSkill;
			
			if (lpSkillBase->m_usChildSkill)
			{
				WORD wAlterSkillID = lpSkillBase->m_usChildSkill ;
				lpSkillBase = g_SkillMgr.GetSkillProtoType( wAlterSkillID ) ;

				CItemInstance* pItem = new CItemInstance;
				pItem->SetSkillInfo( wAlterSkillID,				// 스킬 ID
					CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.cLockCount,			// Lock Count
					CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.cSkillLevel,			// 스킬k 레벨
					TakeType::TS_SSLOT, i, ALTERNATIVESKILL ) ;		

				pPanel->InsertAlterSkillSlot( pItem, 
					wAlterSkillID,
					TakeType::TS_SSLOT, 
					ALTERNATIVESKILL ) ;

				pPanel->InsertVirtualSkill( pItem, true ) ;
			}
		} 
		// 해당 아이디가 어빌리티 아이디면
		else if( 0x1000 <= wSkillID && wSkillID < 0x2000 )
		{
			if ( CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i] )
			{
				delete CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i] ;
				CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i] = NULL ;
			}

			CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i] = new CItemInstance;
			CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i]->SetSkillInfo( 
				CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.wSkill,				// 스킬 ID
				CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.cLockCount,			// Lock Count
				CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.cSkillLevel,			// 스킬k 레벨
				TakeType::TS_SSLOT, i, 0 ) ;												// 위치, 인덱스, 쿨다운 타임

			// 어빌리티 추가
			pAbilityPanel->InsertAbilitySlot( CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i], 
				CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.wSkill,
				CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.cLockCount,
				CRYLGameData::Instance()->m_csStatus.m_Skill.SSlot[ i ].SKILLINFO.cSkillLevel,
				TakeType::TS_SSLOT, i) ;

			pAbilityPanel->InsertVirtualAbility( CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[ i ] ) ;
		}
		else	
		{
			CRYLGameData::Instance()->m_csStatus.m_lstSkillSlot[i] = NULL;
		}
	}

	pSocialPanel->InitSocialAction();
	// 확장 소셜모션이 존재한다면.. 해당 소설모션을 추가한다.
	if(GetAbilityValue(Skill::Type::AB_SOCIALEX))
		pSocialPanel->InitSocialActionAbilityEx(GetAbilityValue(Skill::Type::AB_SOCIALEX));


	pSocialPanel->ActionSlotAlign() ;

	pPanel->SkillSlotVisible() ;
	pAbilityPanel->AbilitySlotVisible();
}

void CCharStatus::AttackPartyPlayer(DefenserNode& defNode, AtType attackType)
{
    for (int i = 0; i < PARTY::MAX_MEM; ++i)
	{
		if (m_PartyInfo.MemberCID[i] == defNode.m_dwCharID)
		{
			m_aryPartyExtraInfo[i].m_lPartyAttackTimer    = 500;
            m_aryPartyExtraInfo[i].m_wAttackType          = attackType.m_wType;
            m_aryPartyExtraInfo[i].m_cSkillLockcount      = attackType.m_cSkillLockCount;
            m_aryPartyExtraInfo[i].m_cSkillLevel          = attackType.m_cSkillLevel;

            m_aryPartyExtraInfo[i].m_usMaxHP      = defNode.m_wMaxHP;
            m_aryPartyExtraInfo[i].m_usMaxMP      = defNode.m_wMaxMP;
            m_aryPartyExtraInfo[i].m_usCurrHP     = defNode.m_sCurrHP;
            m_aryPartyExtraInfo[i].m_usCurrMP     = defNode.m_sCurrMP;
			break;
		}
	}
}

void CCharStatus::CheckUseSkillforStatus(unsigned long &dwLock, unsigned long &dwLevel)
{
	if (NULL != m_lpQuickSelected && 
		Item::ItemType::SKILL_ITEM != m_lpQuickSelected->GetItemType())
	{
		const Skill::ProtoType *lpSkill = g_SkillMgr.GetSkillProtoType(m_lpQuickSelected->GetProtoTypeID());
		unsigned long dwLimitValue, dwSkillNum;
		BOOL bCanUse = TRUE;
		unsigned long i, j;
		dwSkillNum = (dwLock * CSkillMgr::MAX_SKILL_LEVEL + dwLevel);

		for (j = 0; j < Skill::ProtoType::MAX_LIMIT_NUM; ++j)
		{
			bCanUse = TRUE;
			for (i = 0; i < dwSkillNum; ++i)
			{
				dwLimitValue = i * lpSkill->m_StatusLimitValue[j];

				switch (lpSkill[dwLock].m_StatusLimitType[j])
				{
					case Skill::StatusLimit::STR:
						if ((unsigned long)(GetSTR() - 20) < dwLimitValue)
						{
							bCanUse = FALSE;
						}
						break;

					case Skill::StatusLimit::DEX:
						if ((unsigned long)(GetDEX() - 20) < dwLimitValue)
						{
							bCanUse = FALSE;
						}
						break;

					case Skill::StatusLimit::CON:
						if ((unsigned long)(GetCON() - 20) < dwLimitValue)
						{
							bCanUse = FALSE;
						}
						break;

					case Skill::StatusLimit::INT:
						if ((unsigned long)(GetINT() - 20) < dwLimitValue)
						{
							bCanUse = FALSE;
						}
						break;

					case Skill::StatusLimit::WIS:
						if ((unsigned long)(GetWIS() - 20) < dwLimitValue)
						{
							bCanUse = FALSE;
						}
						break;

					//--//	start..
					case Skill::StatusLimit::LEVEL:
						if (m_Info.Level < lpSkill->m_StatusLimitValue[j])
						{
							bCanUse = FALSE;
						}
						break;
					//--//	end..
				}

				if (!bCanUse) break;
			}

			if (!bCanUse)
			{
				dwLock = i / 6;
				dwLevel = i % 6;
			}
		}
	}
}

void CCharStatus::CheckUseSkillforMP(unsigned long &dwLock, unsigned long &dwLevel)
{
	if ( !m_lpQuickSelected )
		return;

	if ( Item::ItemType::SKILL_ITEM == m_lpQuickSelected->GetItemType() )
		return;

	const Skill::ProtoType *lpSkill = g_SkillMgr.GetSkillProtoType(m_lpQuickSelected->GetProtoTypeID());
	if ( lpSkill == NULL )
		return;

	const Skill::ProtoType *lpSkillType = &(lpSkill[dwLock]);
	if ( lpSkillType == NULL )
		return;

	unsigned long dwSkillNum;
	BOOL bCanUse = TRUE;
	dwSkillNum = (dwLock * CSkillMgr::MAX_SKILL_LEVEL + dwLevel);

	bCanUse = TRUE;
	unsigned long level, lockAddMP, startMP, levelMP;
	lockAddMP = lpSkillType->m_LockMP ;
	
	unsigned long i = 0;
	for (; i < dwSkillNum; ++i)
	{
		levelMP = lpSkill[ i / CSkillMgr::MAX_SKILL_LEVEL ].m_LevelMP + lockAddMP ;
		startMP = lpSkill[ i / CSkillMgr::MAX_SKILL_LEVEL ].m_StartMP + lockAddMP ;
		level = (i % CSkillMgr::MAX_SKILL_LEVEL) + 1;
		
		if ( static_cast<unsigned short>(GetCurrentMP()) < startMP + level * levelMP )
		{
            bCanUse = FALSE;
			break;
		}
	}

	if (!bCanUse)
	{
		dwLock = i / 6;
		dwLevel = i % 6;
	}
}


void CCharStatus::SetStatusInit(unsigned short wClass, ChState &pState)
{
	if (0 >= wClass || CClass::JobType::MAX_CLASS <= wClass)
	{
		pState.m_wIP = 0;
		pState.m_wSTR = 0;
		pState.m_wDEX = 0;
		pState.m_wCON = 0;
		pState.m_wINT = 0;
		pState.m_wWIS = 0;
		return;
	}

	// 아칸 기본 클래스는 IP가 나중에 주워진다.
	if (17 == wClass || 18 == wClass)
	{
		pState.m_wIP = 10;
	}
	else
	{
		pState.m_wIP = 8 + m_Info.Level * 2;
	}

	pState.m_wSTR = ClassTable[wClass].GetMinState( CClass::StatusType::STR, m_Info.Level ) ;
	pState.m_wDEX = ClassTable[wClass].GetMinState( CClass::StatusType::DEX, m_Info.Level ) ;
	pState.m_wCON = ClassTable[wClass].GetMinState( CClass::StatusType::CON, m_Info.Level ) ;
	pState.m_wINT = ClassTable[wClass].GetMinState( CClass::StatusType::INT, m_Info.Level ) ;
	pState.m_wWIS = ClassTable[wClass].GetMinState( CClass::StatusType::WIS, m_Info.Level ) ;
}

long CCharStatus::GetPotionCoolTime(unsigned short wPotionSkillID)
{
	vector<PotionCoolTime>::iterator it;
	for (it = m_lstPotionCoolTime.begin(); it != m_lstPotionCoolTime.end(); ++it)
	{
		if ((*it).m_wPotionSkillID == wPotionSkillID)
		{
			return (*it).m_lPotionCoolDownTime;
		}
	}

	return 0;
}

void			
CCharStatus::AddTempInventory( CItemInstance* pItem )
{
	if ( m_lstTempInventoryItem.size() == 0 )
	{
		m_dwTempInventoryTime = timeGetTime() ;
	}
	m_lstTempInventoryItem.push_back( pItem );

}

CItemInstance*
CCharStatus::RemoveTopTempInventory()
{
	CItemInstance* pItem = NULL;
	if ( m_lstTempInventoryItem.size() == 0 )
	{
		return NULL;
	}

	m_dwTempInventoryTime = timeGetTime() ;
	pItem = *(m_lstTempInventoryItem.begin());
	m_lstTempInventoryItem.erase( m_lstTempInventoryItem.begin() );
	return pItem;
}


VOID	CCharStatus::InitTestItemInfo()
{
	for (int i = 0; i < INVENTORY_TAB; ++i)
	{
		for (int j = 0; j < INVENTORY_HEIGHT; ++j)
		{
			for (int k = 0; k < INVENTORY_WIDTH; ++k)
			{
				m_aryTestInven[i][j][k] = m_aryInven[i][j][k];
			}
		}
	}

	m_mapTestItem.clear();
}

BOOL	CCharStatus::CheckInvenMoney(unsigned long dwAddGold, unsigned long dwDeductGold)
{
	if (m_Info.Gold - dwDeductGold <= ULONG_MAX - dwAddGold)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL	CCharStatus::CheckAbleInven(unsigned short wItemID, unsigned char cNum)
{
	const Item::ItemInfo& itemInfo = *(g_ItemMgr.GetItemInfo(wItemID));
	
	unsigned long dwSizeX = itemInfo.m_DetailData.m_cXSize;
	unsigned long dwSizeY = itemInfo.m_DetailData.m_cYSize;

	bool bAble = true;
	std::map<unsigned long, TestItemInfo>::iterator testItemItr;

	// 퀘스트 아이템은 퀘스트 탭에만, 일반 아이템은 일반 탭에만 들어갈 수 있다.
	unsigned long dwStartTab = 0;
	unsigned long dwEndTab = CCharStatus::INVENTORY_TAB;
/*
	// edith 2008.06.04 인벤토리 퀘스트 아이템 탭 제거
	unsigned long dwEndTab = CCharStatus::INVENTORY_TAB - 1;
	if (Item::ItemType::QUEST_ITEM == itemInfo.m_DetailData.m_cItemType)
	{
		dwStartTab = CCharStatus::INVENTORY_TAB - 1;
		dwEndTab = CCharStatus::INVENTORY_TAB;
	}
*/
	for (unsigned long k = dwStartTab; k < dwEndTab; ++k)
	{
		for (unsigned long j = 0; j <= CCharStatus::INVENTORY_HEIGHT - dwSizeY; ++j)
		{
			for (unsigned long i = 0; i <= CCharStatus::INVENTORY_WIDTH - dwSizeX; ++i)
			{
				bAble = true;

				for (unsigned long cy = 0; cy < dwSizeY; ++cy)
				{
					for (unsigned long cx = 0; cx < dwSizeX; ++cx)
					{
						if (m_aryTestInven[k][j + cy][i + cx] != ITEM_EMPTY)
						{
							unsigned long dwKey = (k << 16) | ((j + cy) << 8) | (i + cx);
							testItemItr = m_mapTestItem.find(dwKey);

							// 인벤토리에 놓인 물품이 들아걸수 있는지 체크해서 들어간 아이템이라면 (스택될 수 있다.)
							if (testItemItr != m_mapTestItem.end())
							{
								TestItemInfo& tempInfo = testItemItr->second;
								if (itemInfo.m_usProtoTypeID != tempInfo.wItemID ||
									0 == (itemInfo.m_DetailData.m_dwFlags & Item::DetailData::STACKABLE) ||
									cNum + tempInfo.cNowNum > itemInfo.m_DetailData.m_cMaxDurabilityOrStack)
								{
									bAble = false;
								}
							}
							// 이미 인벤토리에 있던 아이템이라면 (스택 체크를 하지 않는다.)
							else
							{
								bAble = false;
							}
						}
					}
				}

				if (true == bAble)
				{
					unsigned long dwKey = (k << 16) | (j << 8) | i;
					testItemItr = m_mapTestItem.find(dwKey);
					if (testItemItr != m_mapTestItem.end())
					{
						TestItemInfo& tempInfo = testItemItr->second;
						tempInfo.cNowNum += cNum;
					}
					else
					{
						TestItemInfo tempInfo;
						tempInfo.wItemID = itemInfo.m_usProtoTypeID;
						tempInfo.cNowNum = cNum;

						m_mapTestItem.insert(std::make_pair(dwKey, tempInfo)).second;
					}

					for (unsigned long cy = 0; cy < dwSizeY; ++cy)
					{
						for (unsigned long cx = 0; cx < dwSizeX; ++cx)
						{
							m_aryTestInven[k][j + cy][i + cx] = 0;
						}
					}

					m_aryTestInven[k][j][i] = itemInfo.m_usProtoTypeID;
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOL	CCharStatus::CheckQuestInvenSpace(QuestNode* lpQuest)
{
	if ( !lpQuest )
	{
		return FALSE;
	}

	InitTestItemInfo();

	map<unsigned short, unsigned char>::iterator itemItr = lpQuest->m_mapEventGetItem.begin();
	for (; itemItr != lpQuest->m_mapEventGetItem.end(); ++itemItr)
	{
		const Item::ItemInfo* lpItemInfo = g_ItemMgr.GetItemInfo(itemItr->first);
		if ( lpItemInfo )
		{
			if ( lpItemInfo->m_DetailData.m_cItemType == Item::ItemType::QUEST_ITEM )
			{
				if ( FALSE == CheckAbleInven(itemItr->first, itemItr->second) )
				{
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}