// ItemInstance.h: interface for the CItemInstance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMINSTANCE_H__AB450B03_AEF2_455D_85C8_5BF3CA68831F__INCLUDED_)
#define AFX_ITEMINSTANCE_H__AB450B03_AEF2_455D_85C8_5BF3CA68831F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Network/ClientSocket/ClientConstants.h>
#include <Item/Item.h>

#include "RYLSprite.h"
#include "GUITooltip.h"

#include <vertex.h>
#include <vector>

// 전방 참조
class CRYLSpriteEx ;
namespace Skill
{
	struct ProtoType;
}

namespace Item
{
	class CSkillItem : public CItem
	{
	public:

		CSkillItem();
		virtual ~CSkillItem() { }

		inline void				SetProtoTypeID(unsigned short usProtoTypeID) { m_ItemData.m_usProtoTypeID = usProtoTypeID; }
		inline unsigned char    GetMaxNumOrDurability() const { return m_cMaxDurability; }
		inline void             SetMaxNumOrDurability(unsigned char cMaxNumOrDurability) { m_cMaxDurability = cMaxNumOrDurability; }

		bool SetSkillInfo(const Skill::ProtoType *lpSkillInfo);

	protected:

		Item::ItemInfo m_ItemInfoInstance;
		unsigned char m_cMaxDurability;		// 최대 내구도
	};
}

class CItemInstance  
{
	public:

		CItemInstance();
		~CItemInstance();

		void ShowTooltip(BOOL bShow) { m_bShowTooltip = bShow; }
		int SetItemInfo(const char* lpSerializedItem_In, size_t& nParseLength_InOut);
		int SetItemInfo(CItemInstance *lpItem, Item::ItemPos pItemPos);
		int SetItemInfofromID(unsigned short wProtoTypeID, Item::ItemPos &pItemIndex);
		int SetItemInfofromItem( Item::CItem* pBaseItem, Item::ItemPos &pItemIndex);

		Skill::ProtoType* Render(LPDIRECT3DDEVICE8 lpD3DDevice, bool bAlterSkill = false );

		void RenderEnableChant(LPDIRECT3DDEVICE8 lpD3DDevice);

		// 아이템의 프로토타입 ID를 반환
		unsigned short GetProtoTypeID( bool bAlterSkill = false );

		// 각 아이템 인스턴스 타입에 알맞은 스킬 ID를 반환
		unsigned short GetSkillID( bool bAlterSkill = false );

		// 최대내구도 반환
		unsigned char GetMaxDurability(void)
		{
			if (m_bSkill)
			{
				return GetSkillLockCount();
			} 
			else
			{
				return m_lpItemBase->GetMaxNumOrDurability();
			}
		}

		// 최대내구도 설정
		void SetMaxDurability(unsigned char cMaxDurability)
		{
			if (m_bSkill)
				return;

			return m_lpItemBase->SetMaxNumOrDurability(cMaxDurability);
		}

		// 현재 내구도 설정
		void SetNowDurability(unsigned char cNowDurability)
		{
			m_lpItemBase->SetNumOrDurability(cNowDurability);
		}

		// 현재 내구도 반환
		unsigned char GetNowDurability(void)
		{
			return m_lpItemBase->GetNumOrDurability();
		}

		unsigned char GetSkillLockCount(void);

		const Item::ItemInfo &GetItemInfo(void) { return m_lpItemBase->GetItemInfo(); }

		// 아이템 인덱스 설정
		void SetPos(unsigned short wIndex) { m_lpItemBase->SetPos(wIndex); }

		// 아이템 인덱스 설정
		void SetPos(Item::ItemPos &pItemIndex)
		{
			SetItemPos(pItemIndex.m_cPos);
			SetPos(pItemIndex.m_cIndex);
			if (pItemIndex.m_cPos != TakeType::TS_TEMP)
			{
				m_ipPreIndex = m_lpItemBase->GetPos();
			}
		}

		// 아이템 인덱스 설정
		void SetPos(unsigned char &cXIndex, unsigned char &cYIndex, unsigned char &cZIndex)
		{
			m_lpItemBase->SetPos(cXIndex, cYIndex, cZIndex);
		}

		bool IsZeroPos()
		{
			if(m_wPutX == 0 && m_wPutY == 0)
				return true;

			return false;
		}

		unsigned short GetSkillIDbyChantID(unsigned char cChant);		// 챈트 프로토타입 ID 반환
		unsigned char GetSkillType(void);								// 스킬 타입 반환
		void ClearCoolDown(void);										// CoolDown reset

		bool GetIsEnableUpgrade(void);									// 제련 가능 여부
		bool GetIsEnableAddOption(void);								// 옵션이 달릴 수 있는지 여부		

		// 스킬 정보 설정
		BOOL SetSkillInfo(unsigned short wSkillID, unsigned char cGrade, unsigned char cLevel, unsigned char cPos, unsigned char cIndex, long lCoolDown);
		BOOL SetSkillInfo(CItemInstance *lpSkill);

		void SetSkillEnable(BOOL bEnable)		{ m_bSkillEnable = bEnable; }

		unsigned short GetSkillGrade(void)		{ return m_wGrade;				}
		void SetCoolDownTime(long lCoolTime)	{ m_lCoolDownTime = lCoolTime;	}
		void StartCoolDown(unsigned short wGrade = 0, unsigned char cCountCooldown = 0, unsigned short wSkillID = 0, unsigned int iCurCooldown = 0);
		BOOL UpdateCoolDownTime(void);

		// 아이템 위치 설정
		void SetItemPos(unsigned short wItemPos)
		{
			m_lpItemBase->SetItemPos(wItemPos);
			if (wItemPos != TakeType::TS_TEMP)
			{
				m_ipPreIndex.m_cPos = wItemPos;
			}
		}

		unsigned short GetItemPos(void)			{ return m_lpItemBase->GetItemPos();							}
		unsigned char GetItemType(void)			{ return m_lpItemBase->GetItemInfo().m_DetailData.m_cItemType;	}

		BOOL GetIsEquip(void)
		{
			if (!m_bSkill && m_lpItemBase->GetEnableEquip()) return TRUE;

			return FALSE;
		}

		BOOL GetIsEnableRepair(void)
		{
			if (!m_bSkill && m_lpItemBase->GetEnableRepair()) return TRUE;

			return FALSE;
		}

		BOOL GetIsEnableStack(void)
		{
			if (!m_bSkill && m_lpItemBase->GetEnableStack()) return TRUE;

			return FALSE;
		}

		BOOL GetIsSocketItem(void)
		{
			if (m_lpItemBase->GetSocketItem()) return TRUE;

			return FALSE;
		}

		BOOL GetIsRuneItem(void)
		{
			if( m_lpItemBase->GetRuneItem() ) return TRUE;

			return FALSE;
		}

		BOOL GetTwoHandItem(void)
		{
			if (m_lpItemBase->GetTwohandItem()) return TRUE;

			return FALSE;
		}

		BOOL IsOverMouse(void);

		BOOL	m_bStoreItem;
		BOOL 	m_bTradeItem;
		BOOL 	m_bMedalBuyItem;
		BOOL 	m_bSkillCouponBuyItem;

		short	m_wPutX;		// 화면 X 좌표
		short	m_wPutY;		// 화면 Y 좌표
		short	m_wStartX;		// 생성 X 좌표
		short	m_wStartY;		// 생성 Y 좌표

		BOOL	m_bSkillEnable;
		BOOL 	m_bSkill;
		BOOL 	m_bClassSkill;

		long 			m_lCoolDownTime;
		long 			m_lCoolDownTimeCount;
		TLVertex		m_pCoolDownVertices[10];
		unsigned short	m_wCoolDownIndex[24];
		unsigned char	m_cCountCooldown;

		BOOL 	m_bShowTooltip;
		BOOL 	m_bCanUsed;

		unsigned short	m_wGrade;
		Item::ItemPos	m_ipPreIndex;
		unsigned short	m_wPreProtoTypeID;

		__int64			m_dwItemUID;

		Item::CItem*	m_lpItemBase;
		CRYLSprite*		m_lpSprite;
		CRYLSprite*		m_lpDSprite;
		CGUITooltip*	m_lpTooltip;

		std::vector<CRYLSpriteEx*>	m_vecSpriteEx ;
};

#endif // !defined(AFX_ITEMINSTANCE_H__AB450B03_AEF2_455D_85C8_5BF3CA68831F__INCLUDED_)

