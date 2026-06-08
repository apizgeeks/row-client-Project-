#define NOMINMAX

#include "stdafx.h"

#include <Item/Item.h>
#include "BroadcastCharacterData.h"

#include <utility>
#include "GMMemory.h"


namespace Broadcast2nd
{

const unsigned char CNetworkEquipGrade::TransferNetworkEquipGrade(Item::CItem* lpItem)
{
	Item::CEquipment* lpEquip = Item::CEquipment::DowncastToEquipment(lpItem);
	if (NULL != lpEquip)
	{
		Item::Grade::GradeInfo gradeInfo = lpEquip->GetItemGrade();

		// A 그레이드부터 이펙트를 표기한다.
		if (Item::EquipType::B_GRADE > gradeInfo.m_eItemGrade)
		{
			return (Item::EquipType::B_GRADE - gradeInfo.m_eItemGrade);
		}
	}

	return 0;
}

CAggresiveCreatureData::CAggresiveCreatureData()
:   m_dwCID(0), m_usHP(0), m_dwDataFlag(0), m_dwLastUpdatedPulse(0)
{


}

CAggresiveCreatureData::~CAggresiveCreatureData()
{

}

void CAggresiveCreatureData::SetEnchantInfo(const EnchantInfo& enchantInfo)
{
	std::copy(enchantInfo.m_dwStatusFlag, 
        enchantInfo.m_dwStatusFlag + EnchantInfo::MAX_ARRAY, m_EnchantInfo.m_dwStatusFlag);
}

void CAggresiveCreatureData::ClearData()
{
    m_dwCID					= 0;
    m_usHP					= 0;
    m_dwDataFlag			= 0;	
    m_dwLastUpdatedPulse	= 0;

	m_NetworkPos.Initialize(0, 0, 0, 0, 0);	
	m_EnchantInfo.ClearFlag();
}

CCharacterData::CCharacterData()
{
	memset(m_szName, 0, sizeof(char) * MAX_NAME);
	memset(m_szStallName, 0, sizeof(char) * StallNode::MAX_STALL_NAME);
	memset(m_dwDoubleEquipsID, 0, sizeof(unsigned long) * HALF_EQUIP_POS);

	memset(&m_CommunityInfo, 0, sizeof(CommunityInfo));
	memset(&m_StatusInfo, 0, sizeof(bcStatusInfo));
	memset(&m_NetworkEquipGrade, 0, sizeof(CNetworkEquipGrade));
	memset(&m_WarFlagInfo, 0, sizeof(WarFlagInfo));

	memset(&m_cRealmPoint, 0, sizeof(unsigned char));
}

CCharacterData::~CCharacterData()
{

}

void CCharacterData::ClearData()
{
    CAggresiveCreatureData::ClearData();

	memset(m_szName, 0, sizeof(char) * MAX_NAME);
	memset(m_szStallName, 0, sizeof(char) * StallNode::MAX_STALL_NAME);
	memset(m_dwDoubleEquipsID, 0, sizeof(unsigned long) * HALF_EQUIP_POS);

	memset(&m_CommunityInfo, 0, sizeof(CommunityInfo));
	memset(&m_StatusInfo, 0, sizeof(bcStatusInfo));
	memset(&m_NetworkEquipGrade, 0, sizeof(CNetworkEquipGrade));
	memset(&m_WarFlagInfo, 0, sizeof(WarFlagInfo));
	memset(&m_cRealmPoint, 0, sizeof(unsigned char));
}

void CCharacterData::GetEquipmentsID(unsigned short* lpEquipmentsID_Out, unsigned char nIDNum) const
{
	unsigned short* dupEquipsID = (unsigned short*)m_dwDoubleEquipsID;
	std::copy(dupEquipsID, dupEquipsID + 
		std::min(nIDNum, unsigned char(Item::EquipmentPos::MAX_EQUPMENT_POS)), lpEquipmentsID_Out);
}

void CCharacterData::SetEquipmentsID(unsigned short* lpEquipmentsID_In, const unsigned char nIDNum)
{
	std::copy(lpEquipmentsID_In, lpEquipmentsID_In + 
		std::min(nIDNum, unsigned char(Item::EquipmentPos::MAX_EQUPMENT_POS)), reinterpret_cast<unsigned short*>(m_dwDoubleEquipsID));
}

void Broadcast2nd::CCharacterData::SetEquipmentsID(unsigned long dwEquipmentsID_In, const unsigned char nIDIndex)
{
	if (nIDIndex < HALF_EQUIP_POS)
	{
		m_dwDoubleEquipsID[nIDIndex] = dwEquipmentsID_In;
	}
}

void CCharacterData::SetName(const char* szName, const size_t nNameLen)
{
	memcpy(m_szName, szName, std::min(size_t(MAX_NAME), nNameLen));
}

void CCharacterData::SetStallName(const char* szStallName, const size_t nStallNameLen)
{
	memcpy(m_szStallName, szStallName, std::min(size_t(StallNode::MAX_STALL_NAME), nStallNameLen));
}

bool CCharacterData::UpdateData(unsigned long dwCID, unsigned long dwCurrentPulse,
											  const char* szData, int& nBufferSize_InOut)
{
    // 데이터 순서는 다음과 같다.
    // UpdateFlag(4byte)
    // 장비들(16개 * 2byte = 32byte)
    // 이름(16byte)
    // 노점상 이름(32byte)
    // 주소 정보 (public addr/private addr 12byte)
    // 커뮤니티 정보 ( 길드ID / 파티ID / 용병 / 명성 / 마일리지 )
    // 몸 정보 ( 4byte )
    // 레벨 및 
    // Total 138 Bytes

	unsigned long dwUpdateFlag = 0;

	const char* szDataPos = szData;
	const int   nBufferSize = nBufferSize_InOut;
	if (nBufferSize < sizeof(unsigned long))
	{
		return false;
	}

	COPY_AND_ADVANCE_SRC(&dwUpdateFlag, szDataPos, sizeof(unsigned long));

	// 읽어온 UpdateFlag로, 필요한 데이터의 양을 추산한다. 데이터 양이 부족하면 에러를 뱉는다.
	int nEstimateBufferSize = EstimateBufferSize(dwUpdateFlag);

	if (nBufferSize < nEstimateBufferSize )
	{
		// 버퍼 크기가 추산한 길이보다 작은 경우는 실패     
		return false;
	}

	// 데이터 복사. 순서 바뀌면 죽는다!!.
	m_dwCID = dwCID;

	if (0 != dwCurrentPulse)
	{
		// Pulse가 0으로 오면 CellLogin이다. Pulse를 업데이트하지 않는다.
		m_dwLastUpdatedPulse = dwCurrentPulse;
	}

	int nCount = 0;

	for (; nCount < HALF_EQUIP_POS; ++nCount)
	{
		if (dwUpdateFlag & (1 << nCount))
		{
			COPY_AND_ADVANCE_SRC(m_dwDoubleEquipsID + nCount, szDataPos, sizeof(unsigned long));
		}
	}

	if (dwUpdateFlag & CHAR_DELTA_CHAR_NAME)  {   COPY_AND_ADVANCE_SRC(m_szName, szDataPos, sizeof(unsigned char) * MAX_NAME);    }
	if (dwUpdateFlag & CHAR_DELTA_STALL_NAME) {   COPY_AND_ADVANCE_SRC(m_szStallName, szDataPos, sizeof(unsigned char) * StallNode::MAX_STALL_NAME);       }
	if (dwUpdateFlag & CHAR_DELTA_GUILD_ID)   {   COPY_AND_ADVANCE_SRC(&m_CommunityInfo.m_dwGuild_ID, szDataPos, sizeof(unsigned long));   }
	if (dwUpdateFlag & CHAR_DELTA_GUILD_ID)   {   COPY_AND_ADVANCE_SRC(&m_CommunityInfo.m_cTacticsFlag, szDataPos, sizeof(unsigned char)); }
	
    if (dwUpdateFlag & CHAR_DELTA_PARTY_ID)   {   COPY_AND_ADVANCE_SRC(&m_CommunityInfo.m_dwParty_ID, szDataPos, sizeof(unsigned long));     }	

	if (dwUpdateFlag & CHAR_DELTA_FAME)       {   COPY_AND_ADVANCE_SRC(&m_CommunityInfo.m_dwFame,     szDataPos, sizeof(unsigned long));   }
	if (dwUpdateFlag & CHAR_DELTA_FAME)       {   COPY_AND_ADVANCE_SRC(&m_CommunityInfo.m_dwMileage,  szDataPos, sizeof(unsigned long));   }
    
    if (dwUpdateFlag & CHAR_DELTA_BODYINFO)	  {   COPY_AND_ADVANCE_SRC(&m_StatusInfo.m_dwBodyInfo,    szDataPos, sizeof(unsigned long));   }
    if (dwUpdateFlag & CHAR_DELTA_CLASS)      {   COPY_AND_ADVANCE_SRC(&m_StatusInfo.m_cClass,        szDataPos, sizeof(unsigned char));   }
    if (dwUpdateFlag & CHAR_DELTA_LEVEL)      {   COPY_AND_ADVANCE_SRC(&m_StatusInfo.m_cLevel,        szDataPos, sizeof(unsigned char));   }

	for (nCount = 0; nCount < EnchantInfo::MAX_ARRAY; ++nCount)
	{
		if (dwUpdateFlag & (CHAR_DELTA_ENCHANT_INFO << nCount))
		{  
			COPY_AND_ADVANCE_SRC(m_EnchantInfo.m_dwStatusFlag + nCount, szDataPos, sizeof(unsigned long));   
		}
	}

	if (dwUpdateFlag & CHAR_DELTA_NETWORK_POS)	{  COPY_AND_ADVANCE_SRC(&m_NetworkPos, szDataPos, sizeof(CNetworkPos));}

	if (dwUpdateFlag & CHAR_DELTA_HP)			{  COPY_AND_ADVANCE_SRC(&m_usHP, szDataPos, sizeof(unsigned short));          	}

	if (dwUpdateFlag & CHAR_DELTA_EQUIP_GRADE)	{  COPY_AND_ADVANCE_SRC(&m_NetworkEquipGrade, szDataPos, sizeof(CNetworkEquipGrade));	}

	if (dwUpdateFlag & CHAR_DELTA_WAR_FLAG)		{  COPY_AND_ADVANCE_SRC(&m_WarFlagInfo, szDataPos, sizeof(WarFlagInfo));	}

	if (dwUpdateFlag & CHAR_DELTA_REALM_POINT)	{	COPY_AND_ADVANCE_SRC(&m_cRealmPoint, szDataPos, sizeof(unsigned char));	}

	nBufferSize_InOut = static_cast<int>(szDataPos - szData);

	if (0 != dwUpdateFlag)
	{
		// 변경된 데이터가 있는지 살피고, 변경되었다면 데이터 변경 비트를 세팅한다.
		SetDataFlag(CHARDATA_CHANGED);
	}

	return true;
}


int CCharacterData::EstimateBufferSize(unsigned long dwUpdateFlag)
{
    // CID와 UpdateFlag의 길이만큼 더한다.
	int nEstimateBufferSize = 0;
	int nCount = 0;

	for (; nCount < Broadcast2nd::CCharacterData::HALF_EQUIP_POS; ++nCount)
    {
		if (dwUpdateFlag & (1 << nCount))
		{
			nEstimateBufferSize += sizeof(unsigned long);
		}        
    }

    if (dwUpdateFlag & CHAR_DELTA_CHAR_NAME)   	{   nEstimateBufferSize += sizeof(unsigned char) * MAX_NAME;					}
    if (dwUpdateFlag & CHAR_DELTA_STALL_NAME)  	{   nEstimateBufferSize += sizeof(unsigned char) * StallNode::MAX_STALL_NAME;	}
    
	if (dwUpdateFlag & CHAR_DELTA_GUILD_ID)    	{   nEstimateBufferSize += sizeof(unsigned long);		}
	if (dwUpdateFlag & CHAR_DELTA_GUILD_ID)    	{   nEstimateBufferSize += sizeof(unsigned char);		}

	if (dwUpdateFlag & CHAR_DELTA_PARTY_ID)    	{   nEstimateBufferSize += sizeof(unsigned long);		}
    if (dwUpdateFlag & CHAR_DELTA_FAME)        	{   nEstimateBufferSize += sizeof(unsigned long) * 2;	}    
    if (dwUpdateFlag & CHAR_DELTA_BODYINFO)    	{   nEstimateBufferSize += sizeof(unsigned long);		}
    if (dwUpdateFlag & CHAR_DELTA_CLASS)       	{   nEstimateBufferSize += sizeof(unsigned char);		}
    if (dwUpdateFlag & CHAR_DELTA_LEVEL)       	{   nEstimateBufferSize += sizeof(unsigned char);		}

	for (nCount = 0; nCount < EnchantInfo::MAX_ARRAY; ++nCount)
	{
		if (dwUpdateFlag & (CHAR_DELTA_ENCHANT_INFO << nCount))
		{
			nEstimateBufferSize += sizeof(unsigned long);
		}
	}

	if (dwUpdateFlag & CHAR_DELTA_NETWORK_POS) 	{   nEstimateBufferSize += sizeof(CNetworkPos);     	}
	if (dwUpdateFlag & CHAR_DELTA_HP)          	{   nEstimateBufferSize += sizeof(unsigned short);  	}
	if (dwUpdateFlag & CHAR_DELTA_EQUIP_GRADE)	{   nEstimateBufferSize += sizeof(CNetworkEquipGrade);	}
	if (dwUpdateFlag & CHAR_DELTA_WAR_FLAG)		{	nEstimateBufferSize += sizeof(WarFlagInfo);			}
	
	if (dwUpdateFlag & CHAR_DELTA_REALM_POINT)	{	nEstimateBufferSize += sizeof(unsigned char);		}

	return nEstimateBufferSize;
}

void CMonsterData::ClearData()
{
    CAggresiveCreatureData::ClearData();
    m_dwMasterCID = 0;
}


CMonsterData::CMonsterData()
: m_dwMasterCID(0)
{

}

CMonsterData::~CMonsterData()
{

}

bool CMonsterData::UpdateData(unsigned long dwCID, unsigned long dwCurrentPulse, 
                              const char* szData, int& nBufferSize_InOut)
{
	const char* szDataPos = szData;
	const int   nBufferSize = nBufferSize_InOut;
	if (nBufferSize < sizeof(unsigned char))
	{
		return false;
	}

    unsigned char cUpdateFlag = 
        *reinterpret_cast<const unsigned char*>(szDataPos);

    ++szDataPos;	

	// 읽어온 UpdateFlag로, 필요한 데이터의 양을 추산한다. 데이터 양이 부족하면 에러를 뱉는다.
	int nEstimateBufferSize = EstimateBufferSize(cUpdateFlag);
	if (nBufferSize < nEstimateBufferSize)
	{
		// 버퍼 크기가 추산한 길이보다 작은 경우는 실패     
		return false;
	}

	// 데이터 복사. 순서 바뀌면 죽는다!!.
	m_dwCID = dwCID;

	if (0 != dwCurrentPulse)
	{
		// Pulse가 0으로 오면 CellLogin이다. Pulse를 업데이트하지 않는다.
		m_dwLastUpdatedPulse = dwCurrentPulse;
	}
	
	for (int nCount = 0; nCount < EnchantInfo::MAX_ARRAY; ++nCount)
	{
		if (cUpdateFlag & (MON_DELTA_ENCHANT_INFO << nCount))
		{  
			COPY_AND_ADVANCE_SRC(
                m_EnchantInfo.m_dwStatusFlag + nCount, 
                szDataPos, sizeof(unsigned long));   
		}
	}

    const CNetworkPos oldNetPos = m_NetworkPos;

	if (cUpdateFlag & MON_DELTA_NETWORK_POS) {  COPY_AND_ADVANCE_SRC(&m_NetworkPos, szDataPos, sizeof(CNetworkPos));       }
	if (cUpdateFlag & MON_DELTA_HP)          {  COPY_AND_ADVANCE_SRC(&m_usHP, szDataPos, sizeof(unsigned short));          }
    if (cUpdateFlag & MON_DELTA_MASTERCID)   {  COPY_AND_ADVANCE_SRC(&m_dwMasterCID, szDataPos, sizeof(unsigned long));    }

    /* 
    // TODO : REMOVE HERE
    if (49 < oldNetPos.GetSquaredRange(CNetworkPos(0, 0, 0)) &&
        49 < oldNetPos.GetSquaredRange(m_NetworkPos))
    {
        const int MAX_BUFFER = 256;
        char szBuffer[MAX_BUFFER];

        if (0 < _snprintf(szBuffer, MAX_BUFFER - 1, 
            "PktBroadcast2nd : CID:0x%08x : 순간이동하나?.. %f거리를 눈앞에서 이동했다!"
            "/CurrentPulse:%d (%f,%f,%f)->(%f,%f,%f)\n", 
            m_dwCID, oldNetPos.GetSquaredRange(m_NetworkPos), dwCurrentPulse,
            oldNetPos.GetXPos(), oldNetPos.GetYPos(), oldNetPos.GetZPos(),
            m_NetworkPos.GetXPos(), m_NetworkPos.GetYPos(), m_NetworkPos.GetZPos()))
        {
            szBuffer[MAX_BUFFER - 1] = 0;
            OutputDebugString(szBuffer);
        }        
    }
    */

	nBufferSize_InOut = static_cast<int>(szDataPos - szData);

	if (0 != cUpdateFlag)
	{
		// 변경된 데이터가 있는지 살피고, 변경되었다면 데이터 변경 비트를 세팅한다.
		SetDataFlag(CHARDATA_CHANGED);
	}

    return true;
}

int CMonsterData::EstimateBufferSize(unsigned char cUpdateFlag)
{
    // CID와 UpdateFlag의 길이만큼 더한다.
	int nEstimateBufferSize = 0;
	int nCount = 0;

	for (nCount = 0; nCount < EnchantInfo::MAX_ARRAY; ++nCount)
	{
		if (cUpdateFlag & (MON_DELTA_ENCHANT_INFO << nCount))
		{
			nEstimateBufferSize += sizeof(unsigned long);
		}
	}

	if (cUpdateFlag & MON_DELTA_NETWORK_POS) {   nEstimateBufferSize += sizeof(CNetworkPos);     }
	if (cUpdateFlag & MON_DELTA_HP)          {   nEstimateBufferSize += sizeof(unsigned short);  }
    if (cUpdateFlag & MON_DELTA_MASTERCID)   {   nEstimateBufferSize += sizeof(unsigned long);   }

	return nEstimateBufferSize;
}




}
