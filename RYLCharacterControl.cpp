
#include <Item/ItemConstants.h>
#include <Skill/SkillMgr.h>

#include <Creature/CreatureStructure.h>
#include <Creature/EnemyCheck.h>
#include <Creature/Monster/MonsterMgr.h>

#include "RYLCharacterControl.h"
#include "RYLObjectControl.h"
#include "WinInput.h"
#include "FrameTimer.h"
#include "BaseGraphicsLayer.h"

#include "RYLCreatureManager.h"
#include "RYLCharacterDataManager.h"

#include "RYLNetworkData.h"
#include "RYLGameData.h"

#include "RYLStringTable.h"
#include "RYLChattingDlg.h"
#include "GMMemory.h"

#include "InputJoystick.h"

#include "RYLClientMain.h"

#include "RYLJoystickConfigurePanel.h"

CRYLCharacterControl::CRYLCharacterControl( CCharacterControl* lpObjControl ) 
{
	m_lpObjControl = lpObjControl ;
	m_lpCreature   = NULL ;

	m_bLeftMousePress		= FALSE;
	m_bRightMousePress		= FALSE;
	m_bClickPlayerAndNpc	= FALSE;
	m_bClickSkill			= FALSE;

	m_dwNormalTargetID		= 0xFFFFFFFF;
	m_dwSpecialTargetID		= 0xFFFFFFFF;

	m_bLeftClickDown		= FALSE;
	m_bKeyPress				= false;

}
	
CRYLCharacterControl::~CRYLCharacterControl() 
{
}

HRESULT CRYLCharacterControl::UpdateControl( BOOL bKeyAble, BOOL bEdge, int nMouseX,int nMouseY ) 
{
	if ( CRYLNetworkData::Instance()->GetNetworkFlag() != NF_NOWAITING )
		return E_FAIL ;		
			
    RYLCreature* pSelfCreature = RYLCreatureManager::Instance()->GetSelfCreature();
    if ( !pSelfCreature )
        return E_FAIL;

	if(KeyControl::GetCheckInputJoystick())
	{
		CRYLGameData* pGame = CRYLGameData::Instance() ;
								
		if (pGame->m_bMouseMode)
		{			
			pGame->m_bMouseMode = FALSE;			
			g_CharacterData.MouseModeChange();
			pGame->m_bShowCursor = FALSE;

			CEffScript* test_script = new CEffScript;
			test_script->GetScriptBinData("inter_interface-type-keyboard.esf");	

			test_script->m_InterfacePos[0] = (float)g_ClientMain.m_iScreenWidth / 2.0f;
			test_script->m_InterfacePos[1] = (float)g_ClientMain.m_iScreenHeight / 8.0f;

			CSceneManager::m_EffectManager.AddInterfaceScript(test_script);
		} 
	}
	else
	{
		if ( m_lpObjControl->m_MouseClickMode == 0 )
		{
			// edith 2009.05.20 마우스 모드일때의 캐릭터 상황처리
			MouseClickModeSelfCharacterUpdate( bKeyAble, bEdge, nMouseX, nMouseY );
			return E_FAIL;
		}
	}

    CZ3DGeneralChrModel* pSelfModel = pSelfCreature->GetCharModel();
    CharDataNode* pSelfData = RYLCharacterDataManager::Instance()->GetCharData( pSelfCreature->GetCharID() );

    if ( !pSelfModel || !pSelfData )
        return E_FAIL;

    CCharacterLightShadowManager::CCharacterDataNode CheckNode;
	CSceneManager::CharacterCollisionDetectType( pSelfModel, CDT_FULL );

	float fAngle = pSelfModel->GetDirection();
    fAngle -= pSelfCreature->GetFixAngle();
    pSelfCreature->SetFixAngle( 0.0f );
    pSelfModel->SetDirection( fAngle );

    pSelfCreature->SetPosition( pSelfModel->GetPosition() );
    pSelfCreature->SetDirection( pSelfModel->GetDirection() );

	vector3 vecChrPos = pSelfModel->GetPosition();
	vecChrPos += vector3( 0.0f, 150.0f, 0.0f );

	float fChrDir = pSelfModel->GetDirection();
	fChrDir	= -fChrDir;
	fChrDir	-= FLOAT_PHI / 2.0f;

	vector3 vecChrToward( cosf( fChrDir ), 0.0f, sinf( fChrDir ) );
	vecChrToward.Normalize();

	// 왼쪽 노말
	vector3 vecChrLeft( cosf( fChrDir + FLOAT_PHI / 2.0f ), 0.0f, sinf( fChrDir + FLOAT_PHI / 2.0f ) );

	if ( bKeyAble && (g_DeviceInput.GetIsLeftMouseDown() || KeyControl::IsKeyState(KeyControl::KEY_COLUM_ATTACK)))
	{
		m_bLeftClickDown = TRUE;

		// edith 2008.06.25 말탔을때 공격 금지
		// 말을 타고있을땐 캐스팅이 무조건 안된다.
		CRYLGameData* pGame = CRYLGameData::Instance() ;
		if(pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::AVATA] != NULL)
		{
			unsigned char cType = pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::AVATA]->GetItemInfo().m_DetailData.m_cItemType;
			if(cType != Item::ItemType::TRANSFORM && pGame->m_csStatus.m_cRidePos == 1)
					m_bLeftClickDown = FALSE;
		}
	} 
    else if (g_DeviceInput.GetIsLeftMouseUp())
	{
		m_bLeftClickDown = FALSE;
	}

	m_lpObjControl->CheckAttackedTiming();
	m_lpObjControl->CheckCameraShakeTiming();

    CollisionType   CT = CSceneManager::GetCharacterCollisionType( pSelfModel );	

    if ( true == pSelfCreature->Landing( m_lpObjControl ) )
	{
		pSelfCreature->InitCharAction();
		if ( m_lpObjControl->ChangeWeapon ) 
			m_lpObjControl->ChangeWeapon();
	}

	m_bKeyPress = false;

	float	fRunFactor  = CFrameTimer::GetUpdateTimer( m_lpObjControl->m_dwRunFactorTimer );

    if ( bKeyAble && !pSelfCreature->GetIsDead() )
	{                                                   
        CollisionType   CT      = CSceneManager::GetCharacterCollisionType( pSelfModel );
		bool            bSpace  = false;

		CheckNode = CSceneManager::GetCharacterDataNode( pSelfModel );		


		//=============================================================================
		// 스킬 유지
		unsigned long dwFunction;
        if ( pSelfCreature->GetStillCasting() && m_lpObjControl->CheckStillCasting( dwFunction ) )
		{
			m_bKeyPress = true;

			if ( pSelfCreature->Casting() )
			{
				m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
									                  pSelfCreature->GetLowerActionID(),
                                                      pSelfCreature->GetDirection(),
                                                      *( pSelfCreature->GetPosition() ) );
			}
		}
		//=============================================================================
#ifdef _NEW_KEY_
		//=============================================================================
		// 스킬 발동
		else if(g_DeviceInput.GetSkillKey())
		{
			// 스턴 상태가 아니면 스킬을 사용해라.
            if ( !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stun ) )
			{
				// 액션이 뛰는중이 아니다.
				if ( pSelfCreature->GetLowerActionID() != CA_LEFTDASH &&
					 pSelfCreature->GetLowerActionID() != CA_RIGHTDASH &&
					 pSelfCreature->GetLowerActionID() != CA_BACKDASH )
				{
					// 함수포인터가 지정되어있으면
					if ( m_lpObjControl->CheckSkillStart )
					{
						CRYLGameData* pGame = CRYLGameData::Instance() ;

						if ( m_lpObjControl->CheckSkillStart( TRUE ) )
						{
							// edith 1~0키를 눌렀을때 스킬이 발동되는 부분
							g_DeviceInput.SetSkillKey(TRUE);

							if ( pGame->GetAlterSkillEnable() )
							{
								if ( pGame->GetAlterSkillType() == Skill::Type::CHANT || pGame->GetAlterSkillType() == Skill::Type::PASSIVE )		
								{
									return S_OK ;
								}
							}

							// 캐스팅을 한다.
							pSelfCreature->SetStillCasting( true );

							// 이 스킬이 인스턴트면 바로 발동 된다.
							m_lpObjControl->m_dwStartSkillTick		= timeGetTime();
							m_lpObjControl->m_dwCastingSkillTick	= 0;
							m_lpObjControl->m_dwCastingSkillGrade	= 0;
						}
						else if ( (g_DeviceInput.GetIsRightMouseDown() || KeyControl::IsKeyPress(KeyControl::KEY_COLUM_CASTING)) && pGame->m_csStatus.m_lpQuickSelected )
						{
							if ( pGame->m_csStatus.m_lpQuickSelected->GetSkillType() == Skill::Type::CHANT )
							{
								CRYLNetworkData::Instance()->SetChant( pGame->m_csStatus.m_lpQuickSelected ) ;
								pGame->m_csStatus.SetActivateSkill( pGame->m_csStatus.m_lpQuickSelected ) ;
								return S_OK ;
							}
						}
					}
				}
			}
			// edith 1~0키를 눌렀을때 스킬이 발동되는 부분
			//g_DeviceInput.SetSkillKey(FALSE);
		}
#endif

        if ( !m_bKeyPress && CT != CT_WATER && !pSelfCreature->GetSitMode() )
		{
            if ( !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stun ) )
			{
				if ( pSelfCreature->GetLowerActionID() != CA_LEFTDASH &&
					 pSelfCreature->GetLowerActionID() != CA_RIGHTDASH &&
					 pSelfCreature->GetLowerActionID() != CA_BACKDASH )
				{
					if ( m_lpObjControl->CheckSkillStart )
					{
						CRYLGameData* pGame = CRYLGameData::Instance() ;

						// edith 2009.05.20 키보드 모드일때의 스킬발동검사
						if ( m_lpObjControl->CheckSkillStart( TRUE ) )
						{
							if ( pGame->GetAlterSkillEnable() )
							{
								if ( pGame->GetAlterSkillType() == Skill::Type::CHANT || pGame->GetAlterSkillType() == Skill::Type::PASSIVE )		
								{
									return S_OK ;
								}
							}

							// edith 2009.05.20키보드 모드일때의 스킬발동
							pSelfCreature->SetStillCasting( true );

							// 이 스킬이 인스턴트면 바로 발동 된다.
							m_lpObjControl->m_dwStartSkillTick		= timeGetTime();
							m_lpObjControl->m_dwCastingSkillTick	= 0;
							m_lpObjControl->m_dwCastingSkillGrade	= 0;
						}
						else if ( (g_DeviceInput.GetIsRightMouseDown() || KeyControl::IsKeyPress(KeyControl::KEY_COLUM_CASTING)) && pGame->m_csStatus.m_lpQuickSelected )
						{
							const Skill::ProtoType *pSkillType = g_SkillMgr.GetSkillProtoType(pGame->m_csStatus.m_lpQuickSelected->GetSkillID());
							if (pSkillType == NULL)
							{
								CRYLChattingDlg::Instance()->AddMessage(CRYLStringTable::m_strString[144], CRYLNetworkData::Instance()->m_dwMyChrID, 129);
								return S_OK ;
							}

							if ( pGame->m_csStatus.m_lpQuickSelected->GetSkillType() == Skill::Type::CHANT )
							{
								const unsigned char cSkillLockCount = pGame->m_csStatus.m_lpQuickSelected->GetSkillLockCount();
								const unsigned char cSkillLevel = pGame->m_csStatus.m_lpQuickSelected->GetNowDurability();

								// 마나 체크 부분
								if ((pSkillType[cSkillLockCount].m_StartMP + pSkillType[cSkillLockCount].m_LockMP + 
									(pSkillType[cSkillLockCount].m_LevelMP + pSkillType[cSkillLockCount].m_LockMP) * cSkillLevel) > 
									static_cast<unsigned short>(pGame->m_csStatus.GetCurrentMP()))
								{
									CRYLChattingDlg::Instance()->AddMessage(CRYLStringTable::m_strString[146], CRYLNetworkData::Instance()->m_dwMyChrID, 129);
									return S_OK ;
								}
								CRYLNetworkData::Instance()->SetChant( pGame->m_csStatus.m_lpQuickSelected ) ;
								pGame->m_csStatus.SetActivateSkill( pGame->m_csStatus.m_lpQuickSelected ) ;
								return S_OK ;
							}
						}
					}
				}
			}
		}

        //=============================================================================
        if ( !m_bKeyPress )
		{
			// 움직일수 있는지 체크
            if ( !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Hold )
             && !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stun ) 
			 && CRYLGameData::Instance()->GetEnableMove() )
			{                    
                if( ( KeyPressed( 'W' ) && KeyPressed( 'A' ) ) ||
					KeyControl::IsKeyState(KeyControl::KEY_COLUM_FORWARD) && KeyControl::IsKeyState(KeyControl::KEY_COLUM_LEFT) )
				{
					m_bKeyPress = true;
                    pSelfCreature->Walk( RYLCreature::MOVE_FOWARD, fRunFactor );

					vecChrToward = vecChrToward + vecChrLeft;
					vecChrToward.Normalize();

                    pSelfCreature->SetFixAngle( -FLOAT_PHI / 4.0f );
					fAngle += -FLOAT_PHI / 4.0f;

					pSelfModel->SetDirection( fAngle );
				} 
				else if ( KeyPressed( 'W' ) && KeyPressed( 'D' ) ||
					KeyControl::IsKeyState(KeyControl::KEY_COLUM_FORWARD) && KeyControl::IsKeyState(KeyControl::KEY_COLUM_RIGHT) )
				{
					m_bKeyPress = true;
                    pSelfCreature->Walk( RYLCreature::MOVE_FOWARD, fRunFactor );

					vecChrToward = vecChrToward - vecChrLeft;  
					vecChrToward.Normalize();

                    pSelfCreature->SetFixAngle( FLOAT_PHI / 4.0f );
					fAngle += FLOAT_PHI / 4.0f;

					pSelfModel->SetDirection( fAngle );
				} 
				else if ( KeyPressed( 'W' ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_FORWARD) )
				{
                    m_bKeyPress = true;
                    pSelfCreature->Walk( RYLCreature::MOVE_FOWARD, fRunFactor );
				}
				else if ( KeyPressed( 'S' ) && KeyPressed( 'A' ) ||
					KeyControl::IsKeyState(KeyControl::KEY_COLUM_BACKWARD) && KeyControl::IsKeyState(KeyControl::KEY_COLUM_LEFT) )
				{
					m_bKeyPress		= true;

					vecChrToward	= vecChrToward - vecChrLeft;
					vecChrToward.Normalize();

                    pSelfCreature->SetFixAngle( FLOAT_PHI / 4.0f );
					fAngle += FLOAT_PHI / 4.0f;

					pSelfModel->SetDirection( fAngle );

                    pSelfCreature->Walk( RYLCreature::MOVE_BACK, 0.0f );
				}
				else if ( KeyPressed( 'S' ) && KeyPressed( 'D' ) ||
					( KeyControl::IsKeyState(KeyControl::KEY_COLUM_BACKWARD) && KeyControl::IsKeyState(KeyControl::KEY_COLUM_RIGHT) ))
				{
					m_bKeyPress		= true;

					vecChrToward	= vecChrToward + vecChrLeft;
					vecChrToward.Normalize();

                    pSelfCreature->SetFixAngle( -FLOAT_PHI / 4.0f );
					fAngle += -FLOAT_PHI / 4.0f;

					pSelfModel->SetDirection( fAngle );			
                    pSelfCreature->Walk( RYLCreature::MOVE_BACK, 0.0f );
				}
				else if ( KeyPressed( 'S' ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_BACKWARD) )
				{
					m_bKeyPress = true;
                    pSelfCreature->Walk( RYLCreature::MOVE_BACK, 0.0f );
				}
				else if ( KeyPressed( 'A' ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_LEFT) )
				{
					m_bKeyPress = true;
                    pSelfCreature->Walk( RYLCreature::MOVE_LEFT, 0.0f );
				}
				else if ( KeyPressed( 'D' ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_RIGHT) )
				{
					m_bKeyPress = true;
                    pSelfCreature->Walk( RYLCreature::MOVE_RIGHT, 0.0f );
				}

                // by ichabod
				// 슬로우 상태일때는 Dash 불가능.
                if ( !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Slow ) )
				{
                    if ( !CSceneManager::m_CollisionDetection.GetHeightGradient() && ( KeyPressed( VK_SPACE ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_JUMP) ) && 
						( KeyPressed( 'A' ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_LEFT) ) && !bSpace && CT!=CT_WATER && CheckNode.m_DelayTimer == 0 )
					{
						m_bKeyPress = true;
						bSpace		= true;
						
                        if ( pSelfCreature->Dash( RYLCreature::MOVE_LEFT ) )
						{
							const char *strMotionSheet = pSelfModel->GetCurrentMotionSheetName();

							if ( strMotionSheet &&
								( !strcmp( strMotionSheet, "CLASS_ROGUE" )  || !strcmp( strMotionSheet, "WEAPON_SHADOW_OFF") ||
								  !strcmp( strMotionSheet, "WEAPON_2CLAW" ) || !strcmp( strMotionSheet, "WEAPON_2DA" ) ) ) 
							{
								m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
													                  pSelfCreature->GetLowerActionID(),
													                  pSelfCreature->GetDirection(),
													                  vecChrLeft * ( 1365.0f / 4.0f ) + *( pSelfCreature->GetPosition() ) );
							} 
							else
							{
								m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
													                  pSelfCreature->GetLowerActionID(),
													                  pSelfCreature->GetDirection(),
													                  vecChrLeft * ( 1050.0f / 4.0f ) + *( pSelfCreature->GetPosition() ) );
							}
						}
					}

                    if ( !CSceneManager::m_CollisionDetection.GetHeightGradient() && ( KeyPressed( VK_SPACE ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_JUMP) ) &&
						( KeyPressed( 'D' ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_RIGHT) ) && !bSpace && CT!=CT_WATER && CheckNode.m_DelayTimer == 0 )
					{
						m_bKeyPress = true;
						bSpace		= true;
						
                        if ( pSelfCreature->Dash( RYLCreature::MOVE_RIGHT ) )
						{
							const char *strMotionSheet = pSelfModel->GetCurrentMotionSheetName();

							if ( strMotionSheet &&
								 ( !strcmp( strMotionSheet, "CLASS_ROGUE" )  || !strcmp( strMotionSheet, "WEAPON_SHADOW_OFF" ) ||
								   !strcmp( strMotionSheet, "WEAPON_2CLAW" ) || !strcmp( strMotionSheet, "WEAPON_2DA" ) ) )
							{
								m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
													                  pSelfCreature->GetLowerActionID(),
													                  pSelfCreature->GetDirection(),
													                  vecChrLeft * -( 1365.0f / 4.0f ) + *( pSelfCreature->GetPosition() ) );
							} 
							else
							{
								m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
													                  pSelfCreature->GetLowerActionID(),
													                  pSelfCreature->GetDirection(),
													                  vecChrLeft * -( 1050.0f / 4.0f ) + *( pSelfCreature->GetPosition() ) );
							}
						}
					}

					if ( !CSceneManager::m_CollisionDetection.GetHeightGradient() && ( KeyPressed( VK_SPACE ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_JUMP) ) &&
						( KeyPressed( 'S' ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_BACKWARD) ) && !bSpace && CT != CT_WATER && CheckNode.m_DelayTimer == 0 )
					{
						const char *strMotionSheet = pSelfModel->GetCurrentMotionSheetName();

						if ( strMotionSheet &&
							 ( !strcmp( strMotionSheet, "CLASS_ROGUE" )  || !strcmp( strMotionSheet, "WEAPON_SHADOW_OFF" ) ||
							   !strcmp( strMotionSheet, "WEAPON_2CLAW" ) || !strcmp( strMotionSheet, "WEAPON_2DA" ) ) )
						{
							m_bKeyPress = true;
							bSpace		= true;

                            if ( pSelfCreature->Dash( RYLCreature::MOVE_BACK ) )
							{
								m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
													                  pSelfCreature->GetLowerActionID(),
													                  pSelfCreature->GetDirection(),
													                  vecChrToward * -( 1365.0f / 4.0f ) + *( pSelfCreature->GetPosition() ) );
							}
						}
					}
                }

                // by ichabod
				// 슬로우 상태일때는 Dash 불가능.
                if ( !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Slow ) )
				{
					if ( !CSceneManager::m_CollisionDetection.GetHeightGradient() && (KeyPressed( VK_SPACE ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_JUMP))
						&& !bSpace && CT != CT_WATER && CheckNode.m_DelayTimer == 0 )
					{
						if ( CT != CT_NONE && CT != CT_CHRSIDE )
						{
							m_bKeyPress = true;

							if ( pSelfCreature->Jump() )
							{
								if ( g_DeviceInput.KeyHold( 'W' ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_FORWARD))
                                {
                                    pSelfCreature->SetJumpAttackVector( vecChrToward * ( m_lpObjControl->m_fRunSpeed / 35.0f ) );
                                }
								else if ( g_DeviceInput.KeyHold( 'S' ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_BACKWARD))
                                {
                                    pSelfCreature->SetJumpAttackVector( vecChrToward * -( 150.0f / 35.0f ) );
                                }
								else
                                {
                                    pSelfCreature->SetJumpAttackVector( vector3( 0.0f, 0.0f, 0.0f ) );
                                }
							}

							m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
												                  pSelfCreature->GetLowerActionID(),
                                                                  pSelfCreature->GetDirection(),
                                                                  *( pSelfCreature->GetPosition() ) );
						}				
					}
				}
            }

            if ( !m_lpObjControl->m_bSummonCmd )
			{
                if ( !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stun ) )
				{
					if ( m_lpObjControl->m_bClientUsed == false && (KeyPressed( VK_LBUTTON ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_ATTACK))
						&& CT != CT_WATER )
					{
						m_bKeyPress = true;

						if ( pSelfCreature->Attack() )
						{
							m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
												                  pSelfCreature->GetLowerActionID(),
												                  pSelfCreature->GetDirection(),
                                                                  *( pSelfCreature->GetPosition() ) );
						}
					}

					if ( m_lpObjControl->CheckAttackable() && m_bLeftClickDown && m_lpObjControl->m_bClientUsed == true && m_lpObjControl->m_bAttackable &&
						(g_DeviceInput.GetIsLeftMousePress() || KeyControl::IsKeyState(KeyControl::KEY_COLUM_ATTACK)) && CT != CT_WATER )
					{
						bool bChk = false;

						char strText[MAX_PATH];

						// 장거리 무기일때 //
						switch(CRYLGameData::Instance()->m_csStatus.GetWeaponType())
						{
						case Item::ItemType::ARROW:
						case Item::ItemType::BOLT:
						case Item::ItemType::CROSSBOW:
						case Item::ItemType::BOW:
							{
								unsigned char cIndex = Item::EquipmentPos::WEAPON_HAND1;

								if(CRYLGameData::Instance()->m_csStatus.m_cWeaponPos!=0)
								{
									cIndex = Item::EquipmentPos::WEAPON_HAND2;
								}

								const Item::ItemInfo *pItemInfo;

								pItemInfo = g_ItemMgr.GetItemInfo(CRYLGameData::Instance()->m_csStatus.m_lstEquipment[cIndex]->GetProtoTypeID());

								if(pItemInfo)
								{
									switch(pItemInfo->m_UseLimit.m_cLimitStatus)
									{
									case Item::StatusLimit::STR:
										if(CRYLGameData::Instance()->m_csStatus.GetSTR()<pItemInfo->m_UseLimit.m_wLimitValue)
										{
											sprintf(strText, CRYLStringTable::m_strString[37], pItemInfo->m_UseLimit.m_wLimitValue-CRYLGameData::Instance()->m_csStatus.GetSTR());
											bChk = true;
										}
										break;
									case Item::StatusLimit::DEX:
										if(CRYLGameData::Instance()->m_csStatus.GetDEX()<pItemInfo->m_UseLimit.m_wLimitValue)
										{
											sprintf(strText, CRYLStringTable::m_strString[38], pItemInfo->m_UseLimit.m_wLimitValue-CRYLGameData::Instance()->m_csStatus.GetDEX());
											bChk = true;
										}
										break;
									case Item::StatusLimit::CON:
										if(CRYLGameData::Instance()->m_csStatus.GetINT()<pItemInfo->m_UseLimit.m_wLimitValue)				
										{
											sprintf(strText, CRYLStringTable::m_strString[39], pItemInfo->m_UseLimit.m_wLimitValue-CRYLGameData::Instance()->m_csStatus.GetINT());
											bChk = true;
										}
										break;
									case Item::StatusLimit::INT:
										if(CRYLGameData::Instance()->m_csStatus.GetCON()<pItemInfo->m_UseLimit.m_wLimitValue)
										{		
											sprintf(strText, CRYLStringTable::m_strString[40], pItemInfo->m_UseLimit.m_wLimitValue-CRYLGameData::Instance()->m_csStatus.GetCON());
											bChk = true;
										}
										break;
									case Item::StatusLimit::WIS:
										if(CRYLGameData::Instance()->m_csStatus.GetWIS()<pItemInfo->m_UseLimit.m_wLimitValue)						
										{		
											sprintf(strText, CRYLStringTable::m_strString[41], pItemInfo->m_UseLimit.m_wLimitValue-CRYLGameData::Instance()->m_csStatus.GetWIS());
											bChk = true;
										}
										break;
									}
								}
							}
							break;
						}

						if(bChk)
						{
							CRYLChattingDlg::Instance()->AddMessage(strText, CRYLNetworkData::Instance()->m_dwMyChrID, 129);
						}

						if ( m_lpObjControl->m_bAttackable && !bChk)
						{
							m_bKeyPress		= true;
							m_lpObjControl->m_bAttackResult = TRUE;

							if ( pSelfCreature->Attack() )
							{
								m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
													                  pSelfCreature->GetLowerActionID(),
													                  pSelfCreature->GetDirection(),
                                                                      *( pSelfCreature->GetPosition() ) );
							}
						} 
						else if(!bChk)
						{
							m_lpObjControl->m_bAttackResult = FALSE;
						}
					}
				}
			}
		}
	}

	// 움직일수 없는 상황인지 체크
    if ( pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Hold ) ||
		pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stun ) ||
		!CRYLGameData::Instance()->GetEnableMove() )
	{
		if ( m_lpObjControl->SetAutoRun ) 
			m_lpObjControl->SetAutoRun( FALSE );

		m_lpObjControl->m_bAutoFollowMode	= FALSE;
		m_lpObjControl->m_dwAutoFollowChrID = 0xFFFFFFFF;
	}

	if ( !m_bKeyPress )
	{
		m_lpObjControl->PlayerProcessOperation( bKeyAble );
		
		if ( m_lpObjControl->GetAutoRun && m_lpObjControl->GetAutoRun() )
		{
			m_bKeyPress = true;
            pSelfCreature->Walk( RYLCreature::MOVE_FOWARD, fRunFactor );
			m_lpObjControl->SetKillCounter( 0 );
			m_lpObjControl->SetClientExitCounter( 0 );
			m_lpObjControl->SetLogoutCounter( 0 );
			m_lpObjControl->SetPortalCounter( 0 );
			m_lpObjControl->SetSkillUseCounter( 0 );
		}

		if ( m_lpObjControl->m_bAutoFollowMode )
		{
            RYLCreature* pFollowCreature = RYLCreatureManager::Instance()->GetCreature( m_lpObjControl->m_dwAutoFollowChrID );

            if ( pFollowCreature )
            {
                vector3 vecLen = *( pFollowCreature->GetPosition() ) - *( pSelfCreature->GetPosition() );
				vecLen.y	   = 0.0f;

				if ( vecLen.GetLens() > 700.0f )
				{
					m_lpObjControl->SetDirectionforMouse( vecLen );

                    pSelfCreature->SetDirection( pSelfModel->GetDirection() );
					fChrDir = pSelfModel->GetDirection();

					m_bKeyPress = true;
                    pSelfCreature->Walk( RYLCreature::MOVE_FOWARD, fRunFactor );
				}
			} 
			else
			{
				m_lpObjControl->m_bAutoFollowMode	= FALSE;
				m_lpObjControl->m_dwAutoFollowChrID = 0xFFFFFFFF;
			}
		}
	}
	else
	{
		m_lpObjControl->SetKillCounter( 0 );
		m_lpObjControl->SetClientExitCounter( 0 );
		m_lpObjControl->SetLogoutCounter( 0 );
		m_lpObjControl->SetPortalCounter( 0 );
		m_lpObjControl->SetSkillUseCounter( 0 );
		
		if ( m_lpObjControl->SetAutoRun ) 
			m_lpObjControl->SetAutoRun( FALSE );

		m_lpObjControl->m_bAutoFollowMode	= FALSE;
		m_lpObjControl->m_dwAutoFollowChrID = 0xFFFFFFFF;
	}

    // 스킬 해제
	unsigned long dwFunction;
    if ( pSelfCreature->GetStillCasting() && !m_lpObjControl->CheckStillCasting( dwFunction ) )
	{
		m_lpObjControl->m_bSkillExecuted = TRUE;
	}

	if ( !m_bKeyPress )
	{
        if ( !pSelfCreature->GetIsDead() )
		{
			pSelfCreature->Wait();
		}
	}

	if ( !m_lpObjControl->GetAttacking() )
	{
		if ( m_lpObjControl->SetChangeWeapon )
			m_lpObjControl->SetChangeWeapon( FALSE );
	}

    switch ( pSelfCreature->GetLowerActionID() )
	{
		case CA_WALK:
            {
                CSceneManager::CharacterMovementVector( pSelfModel, vecChrToward * ( m_lpObjControl->m_fWalkSpeed / 35.0f ) );
            }
            break;

        case CA_FORWARDJUMP:
		case CA_FRONTDASH:
		case CA_RUN:
		    {
                CSceneManager::CharacterMovementVector( pSelfModel, vecChrToward * ( m_lpObjControl->m_fRunSpeed / 35.0f ) );
            }
			break;
        
        case CA_WALKBACK:
        case CA_BACKJUMP:
		    {
			    CSceneManager::CharacterMovementVector( pSelfModel, vecChrToward * -( m_lpObjControl->m_fWalkSpeed / 35.0f ) );
            }
			break;

		case CA_WALKLEFT:
            {
			    CSceneManager::CharacterMovementVector( pSelfModel, vecChrLeft * ( m_lpObjControl->m_fWalkSpeed / 35.0f ) );
            }
			break;

		case CA_WALKRIGHT:
            {
			    CSceneManager::CharacterMovementVector( pSelfModel, vecChrLeft * -( m_lpObjControl->m_fWalkSpeed / 35.0f ) );
            }
			break;

		case CA_ATTACK2:
		case CA_ATTACK3:
		case CA_ATTACK4:
		case CA_ATTACK5:
		case CA_ATTACK6:
            {
			    CSceneManager::CharacterMovementVector( pSelfModel, vector3( 0.0f,0.0f,0.0f ) );
            }
			break;

		case CA_JUMPATTACK2:
		case CA_JUMPATTACK:
            {
                CSceneManager::CharacterMovementVector( pSelfModel, pSelfCreature->GetJumpAttackVector() );
            }
			break;

		case CA_LEFTDASH:
			{
                string strMotionSheet = pSelfModel->GetCurrentMotionSheetName();

                if ( strMotionSheet.size() 
                    && ( !strMotionSheet.compare( "CLASS_ROGUE" ) || !strMotionSheet.compare( "WEAPON_SHADOW_OFF" )
                      || !strMotionSheet.compare( "WEAPON_2CLOW" ) || !strMotionSheet.compare( "WEAPON_2DA" ) ) )
                {
					CSceneManager::CharacterMovementVector( pSelfModel, vecChrLeft * ( 1365.0f / 35.0f ) , -1.1f );
				} 
				else
				{
					CSceneManager::CharacterMovementVector( pSelfModel, vecChrLeft * ( 1050.0f / 35.0f ) , -1.1f );
				}
			}
			break;

		case CA_RIGHTDASH:
			{
				string strMotionSheet = pSelfModel->GetCurrentMotionSheetName();

                if ( strMotionSheet.size() 
                    && ( !strMotionSheet.compare( "CLASS_ROGUE" ) || !strMotionSheet.compare( "WEAPON_SHADOW_OFF" )
                      || !strMotionSheet.compare( "WEAPON_2CLOW" ) || !strMotionSheet.compare( "WEAPON_2DA" ) ) )
				{
					CSceneManager::CharacterMovementVector( pSelfModel, vecChrLeft * -( 1365.0f / 35.0f ) ,-1.1f );
				} 
				else
				{
					CSceneManager::CharacterMovementVector( pSelfModel, vecChrLeft * -( 1050.0f / 35.0f ) ,-1.1f );
				}
			}
			break;

		case CA_BACKDASH:
            {
			    CSceneManager::CharacterMovementVector( pSelfModel, vecChrToward * -( 1365.0f / 35.0f ) ,-1.1f);
            }
			break;

		default:
            {
			    CSceneManager::CharacterMovementVector( pSelfModel, vector3( 0.0f, 0.0f, 0.0f ) );
            }
			break;
	}

    if ( CFrameTimer::GetUpdateTimer( m_lpObjControl->m_nCharMovingPacketTimerID ) >= 1.0f ) // && pSelfCreature->IsCancelAction(2))
	{
		if ( pSelfCreature->GetLowerActionID() == CA_WALK
		 || pSelfCreature->GetLowerActionID() == CA_RUN
		 || pSelfCreature->GetLowerActionID() == CA_WALKBACK
		 || pSelfCreature->GetLowerActionID() == CA_WALKLEFT
		 || pSelfCreature->GetLowerActionID() == CA_WALKRIGHT
		 || pSelfCreature->GetLowerActionID() == CA_REST
		 || pSelfCreature->GetLowerActionID() == CA_WAIT
		 || pSelfCreature->GetLowerActionID() == CA_FIRE1
		 || pSelfCreature->GetLowerActionID() == CA_FIRE2

		 
		 || (CA_GATHERSTART <= pSelfCreature->GetLowerActionID() && pSelfCreature->GetLowerActionID() <= CA_GATHEREND)
		 || (CA_SOCIALSTART <= pSelfCreature->GetLowerActionID() && pSelfCreature->GetLowerActionID() <= CA_SOCIALEND)

		 || pSelfCreature->GetLowerActionID() == CA_SHOT
		 || pSelfCreature->GetLowerActionID() == CA_JUMPSHOT
		 || pSelfCreature->GetLowerActionID() == CA_FORWARDJUMP
		 || pSelfCreature->GetLowerActionID() == CA_BACKJUMP
		 || pSelfCreature->GetLowerActionID() == CA_JUMP
		 || pSelfCreature->GetLowerActionID() == CA_FRONTDASH
		 || pSelfCreature->GetLowerActionID() == CA_JUMPATTACK
		 || pSelfCreature->GetLowerActionID() == CA_JUMPATTACK2
		 || pSelfCreature->GetLowerActionID() == CA_FALLDOWN )
		{
			vecChrToward.Normalize();
			vecChrLeft.Normalize();

			vector3 vecMove;

			switch( pSelfCreature->GetLowerActionID() )
			{
				case CA_FORWARDJUMP:
				case CA_FRONTDASH:
				case CA_RUN:
                    {
					    vecMove = vecChrToward * ( m_lpObjControl->m_fRunSpeed / 4.0f );
                    }
					break;

				case CA_WALKBACK:
                    {
					    vecMove = vecChrToward * -( m_lpObjControl->m_fWalkSpeed / 4.0f );
                    }
					break;

				case CA_WALKLEFT:
                    {
					    vecMove = vecChrLeft * ( m_lpObjControl->m_fWalkSpeed / 4.0f );
                    }
					break;

				case CA_WALKRIGHT:
                    {
					    vecMove = vecChrLeft * -( m_lpObjControl->m_fWalkSpeed / 4.0f );
                    }
					break;

				case CA_WALK:
                    {
					    vecMove = vecChrToward * ( m_lpObjControl->m_fWalkSpeed / 4.0f );
                    }
					break;

				case CA_JUMPATTACK:
				case CA_JUMPATTACK2:
                    {
                        vecMove = pSelfCreature->GetJumpAttackVector();
                    }
					break;

				case CA_BACKJUMP:
                    {
					    vecMove = vecChrToward * -( m_lpObjControl->m_fWalkSpeed / 4.0f );
                    }
					break;

				default:
                    {
					    vecMove = vector3( 0.0f, 0.0f, 0.0f );
                    }
					break;
			}

            vecMove += *( pSelfCreature->GetPosition() );

			CollisionType CT = CSceneManager::GetCharacterCollisionType( pSelfModel );

			if ( CT == CT_WATER )
			{
                m_lpObjControl->WrapSendMovingPacket( CA_SWIM, CA_SWIM, pSelfCreature->GetDirection(), vecMove );
			} 
			else
			{
				// edith 점프
                m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
									                  pSelfCreature->GetLowerActionID(),
                                                      pSelfCreature->GetDirection(), vecMove );
			}
		}
	}
    return S_OK;
}

//--------------------------------------------------------------------------------------------------------------------------
// CRYLCharacterControl::MouseClickModeSelfCharacterUpdate()
static BOOL bEdit = TRUE;

static float           fChrToBack    = -100.0f;
static vector3         vecEndBack    = vector3( 0.0f, 0.0f, 0.0f );

void CRYLCharacterControl::MouseClickModeSelfCharacterUpdate( BOOL bKeyAble, BOOL bEdge, int nMouseX, int nMouseY )
{

	vector3 vecMovePos;

	RYLCreature* pSelfCreature = RYLCreatureManager::Instance()->GetSelfCreature();
    if ( !pSelfCreature )
        return;

    CZ3DGeneralChrModel* pSelfModel = pSelfCreature->GetCharModel();
    if ( !pSelfModel )
        return;

    CharDataNode* pSelfData = RYLCharacterDataManager::Instance()->GetCharData( pSelfCreature->GetCharID() );
    if ( !pSelfData )
        return;

	if ( bEdit )
	{
        if ( !RYLCreatureManager::Instance()->GetCreatureListHead() )
            return;

		if ( m_lpObjControl->m_bClientUsed )
		{
			nMouseX = g_DeviceInput.GetMouseLocal()->x;
			nMouseY = g_DeviceInput.GetMouseLocal()->y;
		}

		CCharacterLightShadowManager::CCharacterDataNode CheckNode;
		CSceneManager::CharacterCollisionDetectType( pSelfModel, CDT_FULL );

        pSelfCreature->SetDirection( pSelfModel->GetDirection() );
        float fAngle = pSelfModel->GetDirection();

        pSelfCreature->SetPosition( pSelfModel->GetPosition() );
		vector3 vecChrPos	 = pSelfModel->GetPosition();
		vector3 vecChrToward = vector3( 0.0f, 0.0f, 0.0f );
		vector3 vecChrLeft	 = vector3( 0.0f, 0.0f, 0.0f );

		m_lpObjControl->CheckAttackedTiming();
		m_lpObjControl->CheckCameraShakeTiming();

		m_bKeyPress = false;

		float fRunFactor = CFrameTimer::GetUpdateTimer( m_lpObjControl->m_dwRunFactorTimer );

		BOOL bAvailableTarget = FALSE;

		if ( bKeyAble && g_DeviceInput.GetIsLeftMouseDown() )
		{
			m_bLeftClickDown = TRUE;
		} 
		else if ( g_DeviceInput.GetIsLeftMouseUp() )
		{
			m_bLeftClickDown = FALSE;
		}

		unsigned long dwFunction;
		if ( pSelfCreature->GetStillCasting() && !m_lpObjControl->CheckStillCasting( dwFunction ) )
		{
			// edith 
			// 스킬을 캐스팅하다 RButton을 up했을대 여기가 걸린다.
			// m_bSkillExecuted 이함수가 스킬의 실행과 관련된 변수같다.
			m_lpObjControl->m_bSkillExecuted = TRUE;
		}

		if ( !bEdge && bKeyAble && m_lpObjControl->m_lAutoRunCounter && g_DeviceInput.GetIsLeftMousePress() )
		{
			m_lpObjControl->m_lAutoRunCounter -= fRunFactor * 5;
			
			if ( m_lpObjControl->m_lAutoRunCounter < 0 )
			{
				m_lpObjControl->m_lAutoRunCounter   = 0;
				m_lpObjControl->SetAutoRun( TRUE );
				m_lpObjControl->m_bAutoFollowMode   = FALSE;
				m_lpObjControl->m_dwAutoFollowChrID = 0xFFFFFFFF;
			}
		}

		if ( true == pSelfCreature->Landing( m_lpObjControl ) )
		{
			pSelfCreature->InitCharAction();

			if ( m_lpObjControl->ChangeWeapon ) 
            {
				m_lpObjControl->ChangeWeapon();
            }
		}

		// 스킬 유지
		if ( pSelfCreature->GetStillCasting() && m_lpObjControl->CheckStillCasting( dwFunction ) )
		{
			m_bKeyPress = true;
			m_lpObjControl->SetKillCounter( 0 );
			m_lpObjControl->SetClientExitCounter( 0 );
			m_lpObjControl->SetLogoutCounter( 0 );
			m_lpObjControl->SetPortalCounter( 0 );
			m_lpObjControl->SetSkillUseCounter( 0 );
			bKeyAble = FALSE;

			if ( pSelfCreature->Casting() )
			{
				//캐스팅이 완료시 다른 클라이언트에 동작을 바꾸라고 전달.
				m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
									                  pSelfCreature->GetLowerActionID(),
									                  pSelfCreature->GetDirection(),
									                  *( pSelfCreature->GetPosition() ) );
			}
		}

        
        RYLCreature* pTempCreature = NULL;
        RYLCreature* pSpecialCreature = NULL;

		if ( !bEdge && bKeyAble && !pSelfCreature->GetIsDead() )
		{
			CollisionType CT = CSceneManager::GetCharacterCollisionType( pSelfModel );
			CheckNode = CSceneManager::GetCharacterDataNode( pSelfModel );

			// 타겟 처리
			CZ3DGeneralChrModel *pSelectChr = NULL;
			pSelectChr = CSceneManager::PickingCharacter( nMouseX, nMouseY );

			if ( pSelectChr ) 
                pTempCreature = RYLCreatureManager::Instance()->GetCreature( pSelectChr );

			if ( m_lpObjControl->CheckSkillStart && m_lpObjControl->CheckSkillStart( TRUE ) )
			{
				unsigned long dwFunction;
				m_lpObjControl->CheckStillCasting( dwFunction );
				
				if ( dwFunction != 0xFFFFFFFF )
				{
                    pTempCreature = RYLCreatureManager::Instance()->GetCreature( dwFunction );
				}
			}
			
			if ( pTempCreature )
			{
				bAvailableTarget = TRUE;

				if ( m_dwSpecialTargetID == 0xFFFFFFFF )
				{
					m_lpObjControl->m_dwSpecialTargetID = pTempCreature->GetCharID();
					
					if ( m_lpObjControl->CheckTargetforSkill )
					{
						m_lpObjControl->CheckTargetforSkill( m_lpObjControl->m_dwSpecialTargetID, TRUE );

						// 거리체크
						m_lpObjControl->CheckTargetLength(m_lpObjControl->m_dwSpecialTargetID);
					}
				}

				if ( m_bLeftMousePress && g_DeviceInput.GetIsLeftMousePress() )	// 왼쪽버튼 마우스 커서 스냅
				{
					if ( pTempCreature->GetCharID() != 0xFFFFFFFF && pSelfCreature->GetCharID() != pTempCreature->GetCharID() )
					{
						// edith 몬스터를 클릭했을때 커서를 고정하는 부분
						vector3 vecResult, vecTargetMonsterPos;
						float fW;

						vecTargetMonsterPos = *( pTempCreature->GetPosition() );
						vecTargetMonsterPos = vecTargetMonsterPos + vector3( 0.0f, 100.0f, 0.0f );
						BaseGraphicsLayer::TransformVector( vecTargetMonsterPos, vecResult, fW );
						g_DeviceInput.SetCursorPos( ( int )vecResult.x, ( int )vecResult.y );
					}
				}

				if ( m_bRightMousePress && g_DeviceInput.GetIsRightMousePress() )	// 오른쪽버튼 마우스 커서 스냅
				{
					if ( m_dwSpecialTargetID != 0xFFFFFFFF )
					{
						vector3 vecResult, vecTargetMonsterPos;
						float fW;

						pSpecialCreature = RYLCreatureManager::Instance()->GetCreature( m_dwSpecialTargetID );

						if ( pSpecialCreature )
						{
							// 타켓의 위치로 마우스를 고정한다.
							vecTargetMonsterPos = *( pSpecialCreature->GetPosition() );
							vecTargetMonsterPos = vecTargetMonsterPos + vector3( 0.0f, 100.0f, 0.0f );
							BaseGraphicsLayer::TransformVector( vecTargetMonsterPos, vecResult, fW );
							g_DeviceInput.SetCursorPos( ( int )vecResult.x, ( int )vecResult.y );
						}
					}
				}
			}
			else
			{
				// 마우스 오버에 캐릭터가 없을때.
				if ( m_dwSpecialTargetID == 0xFFFFFFFF )
				{
					m_lpObjControl->m_dwSpecialTargetID = 0xFFFFFFFF;
				}
				m_lpObjControl->CheckTargetLength(m_lpObjControl->m_dwSpecialTargetID);
			}

			// 마우스 오른쪽 버튼
			RYLCreature* pNewSpecialTarget = MouseClickModeSelectSpecialTarget( pTempCreature, nMouseX, nMouseY ) ;
			if (NULL != pNewSpecialTarget)
			{
				pSpecialCreature = pNewSpecialTarget;
			}

			// 마우스 왼쪽 버튼
			if ( g_DeviceInput.GetIsLeftMouseDown() )
			{
				m_bLeftMousePress	= TRUE;
				m_bKeyPress			= true;
				m_lpObjControl->m_bAutoFollowMode	= FALSE;
				m_lpObjControl->m_dwAutoFollowChrID = 0xFFFFFFFF;
				m_lpObjControl->SetAutoRun( FALSE );

				if ( pTempCreature )
				{
					if ( m_lpObjControl->m_bSummonCmd )
					{
						m_lpObjControl->m_bAutoTargetAttack = false;
						m_lpObjControl->m_dwAutoTargetID	= 0xFFFFFFFF;
					}
					else
					{
                        if ( !pTempCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stealth ) &&
							pSelfCreature->GetCharID() != pTempCreature->GetCharID() )
						{
							m_lpObjControl->m_bAutoTargetAttack = true;
							m_lpObjControl->m_dwAutoTargetID	= pTempCreature->GetCharID();
						}
						else
						{
							m_lpObjControl->m_bAutoTargetAttack = false;
							m_lpObjControl->m_dwAutoTargetID	= 0xFFFFFFFF;
						}
					}

					m_dwNormalTargetID	= pTempCreature->GetCharID();
					m_dwSpecialTargetID	= 0xFFFFFFFF;
				} 
				else
				{
					m_lpObjControl->m_nSelectItem		= CSceneManager::MouseOnCheckInstanceObject( nMouseX, nMouseY );
					m_dwNormalTargetID	= 0xFFFFFFFF;
					m_dwSpecialTargetID	= 0xFFFFFFFF;
					m_lpObjControl->m_bAutoTargetAttack = false;
					m_lpObjControl->m_dwAutoTargetID	= 0xFFFFFFFF;

					if ( m_lpObjControl->m_nSelectItem == 0xFFFFFFFF )
					{
						m_lpObjControl->m_lAutoRunCounter = 3000;
				
						//수정 05.06.22
						vector3 vecMovePos = CSceneManager::PickMousePos( nMouseX, nMouseY );

						vector3 vecSeePos = m_lpObjControl->m_Camera. m_vecCameraPos;
						vector3 vecTemp = vector3(  0.0f, 0.0f, 0.0f );
						vecTemp.x = vecSeePos.x - vecMovePos.x;
						vecTemp.z = vecSeePos.z - vecMovePos.z;
						vecTemp.y = vecTemp.z / vecTemp.x * 90;
						vecTemp.x = (cos(vecTemp.y))+(sin(vecTemp.y)); 
						vecTemp.z = (-sin(vecTemp.y)*25.0f)+(cos(vecTemp.y)*25.0f);

						CEffScript *pMousePointEff = new CEffScript;
						CSceneManager::AddEffectScript( pMousePointEff );

					
						pMousePointEff->GetScriptBinData( "point.esf" );
						pMousePointEff->SetStartPos( vecMovePos.x + vecTemp.x, vecMovePos.y, vecMovePos.z + vecTemp.z );			
						pMousePointEff->SetEndPos( vecMovePos.x + vecTemp.x, vecMovePos.y, vecMovePos.z + vecTemp.z );

						//수정전
						/*
						m_lpObjControl->m_lAutoRunCounter = 3000;
				
						vector3 vecMovePos = CSceneManager::PickMousePos( nMouseX, nMouseY );	
						pMousePointEff->GetScriptBinData( "point.esf" );
						pMousePointEff->SetStartPos( vecMovePos.x, vecMovePos.y + 30.0f, vecMovePos.z );			
						pMousePointEff->SetEndPos( vecMovePos.x , vecMovePos.y + 30.0f, vecMovePos.z );
						*/
					}
				}
			}
			else if ( g_DeviceInput.GetIsLeftMouseUp() )
			{
				m_bLeftMousePress = FALSE;
			}
			else if ( g_DeviceInput.GetIsLeftMousePress() )
			{
				if ( m_dwSpecialTargetID != 0xFFFFFFFF )
				{
					m_dwNormalTargetID = m_dwSpecialTargetID;
				}
			}

			if ( !pTempCreature && m_dwNormalTargetID != 0xFFFFFFFF )
			{
				pTempCreature = RYLCreatureManager::Instance()->GetCreature( m_dwNormalTargetID );
				
				if ( pTempCreature )
				{
					bAvailableTarget = TRUE;
				}
				else
				{
					m_dwNormalTargetID = 0xFFFFFFFF;
				}
			}

			if ( !pSpecialCreature && m_dwSpecialTargetID != 0xFFFFFFFF )
			{
				pSpecialCreature = RYLCreatureManager::Instance()->GetCreature( m_dwSpecialTargetID );
				
                if ( !pSpecialCreature )
				{
					m_dwSpecialTargetID = 0xFFFFFFFF;
				}
			}

			if ( pTempCreature )
			{
				if ( pTempCreature->GetIsDead() || pTempCreature->GetCharID() == pSelfCreature->GetCharID() )
				{
					m_dwNormalTargetID	= 0xFFFFFFFF;
					pTempCreature		= NULL;
					bAvailableTarget	= FALSE;
				}
			}

			if ( m_lpObjControl->m_bAutoTargetAttack )
			{
				RYLCreature* pTargetCreature = RYLCreatureManager::Instance()->GetCreature( m_lpObjControl->m_dwAutoTargetID );
				
                if ( pTargetCreature )
				{
					if ( pTargetCreature->GetIsDead() )
					{
						m_lpObjControl->m_bAutoTargetAttack = false;
						m_lpObjControl->m_dwAutoTargetID	= 0xFFFFFFFF;
					}
				}
				else
				{
					m_lpObjControl->m_bAutoTargetAttack = false;
					m_lpObjControl->m_dwAutoTargetID	= 0xFFFFFFFF;
				}

				if ( m_lpObjControl->m_dwAutoTargetID != 0xFFFFFFFF )
					m_dwNormalTargetID = m_lpObjControl->m_dwAutoTargetID;
			}

			// 움직임 체크
			unsigned long dwFunction;
			if ( ( m_bLeftClickDown && g_DeviceInput.GetIsLeftMousePress() && m_bLeftMousePress ) || 
				( g_DeviceInput.GetIsRightMousePress() && m_bRightMousePress ) || 
				( m_lpObjControl->CheckStillCasting && m_lpObjControl->CheckStillCasting( dwFunction ) ) )
			{
#ifdef _NEW_KEY_
				if ( m_bLeftClickDown && g_DeviceInput.GetIsLeftMousePress() && m_bLeftMousePress )
#else
				if ( ( m_bLeftClickDown && g_DeviceInput.GetIsLeftMousePress() && m_bLeftMousePress ) || 
					( g_DeviceInput.GetIsRightMousePress() && m_bRightMousePress ) )
#endif
				{
					/*
					CSceneManager::PickMakeRay( nMouseX, nMouseY, BaseGraphicsLayer::m_lScreenSx, BaseGraphicsLayer::m_lScreenSy );
					vector3 vecStart = CSceneManager::m_vecPickRayStart;
					vector3 vecEnd	 = CSceneManager::m_vecPickRayStart + 1000000.0f * CSceneManager::m_vecPickRayDir;

					vector3 vecPoly[ 4 ];
					float fSize = 1000000.0f;
					vecPoly[ 0 ] = vecChrPos + vector3( -fSize, 0.0f, -fSize );
					vecPoly[ 1 ] = vecChrPos + vector3( fSize, 0.0f, -fSize );
					vecPoly[ 2 ] = vecChrPos + vector3( -fSize, 0.0f, fSize );
					vecPoly[ 3 ] = vecChrPos + vector3( fSize, 0.0f, fSize );

					float fInter;
					if ( CIntersection::PolygonRay( vecStart, vecEnd, vecPoly, fInter ) )
					{
						m_lpObjControl->m_vecClickPos = vecStart + ( fInter * CSceneManager::m_vecPickRayDir );
					}
					else if ( CIntersection::PolygonRay( vecStart, vecEnd, &vecPoly[ 1 ], fInter ) )
					{
						m_lpObjControl->m_vecClickPos = vecStart + ( fInter * CSceneManager::m_vecPickRayDir );
					} 
					else*/
					{
						m_lpObjControl->m_vecClickPos = CSceneManager::PickMousePos( nMouseX, nMouseY );
					}
				}

				if ( pTempCreature )
				{
                    CZ3DGeneralChrModel* pTempModel = pTempCreature->GetCharModel();

					if ( pTempModel )
                    {
						m_lpObjControl->m_vecClickPos = pTempModel->GetPosition();
                    }
					else
                    {
                        m_lpObjControl->m_vecClickPos = *( pTempCreature->GetPosition() );
                    }

					vector3 vecLen = m_lpObjControl->m_vecClickPos - *( pSelfCreature->GetPosition() ); 
					vecLen.y = 0.0f;

					float fTargetLength = vecLen.GetLens();

					if ( g_DeviceInput.GetIsLeftMouseDown() && m_lpObjControl->m_bSummonCmd )
					{
						m_lpObjControl->m_bClickMove		= false;
						m_lpObjControl->m_bClickAttack		= false;
						m_bClickPlayerAndNpc	= FALSE;
						m_bLeftClickDown		= FALSE;

						if ( m_lpObjControl->GetClientEnemyType( pTempCreature->GetCharID() ) == EnemyCheck::EC_ENEMY )
						{
							m_lpObjControl->SendSummonAttack( pTempCreature->GetCharID() );
						}

						return;
					}

					if ( g_DeviceInput.GetIsLeftMousePress() )
					{
						switch( m_lpObjControl->GetClientIdentity( pTempCreature->GetCharID() ) )
						{
							case Creature::CT_PC:
                            case Creature::CT_STRUCT:
							case Creature::CT_SUMMON:
							case Creature::CT_SIEGE_OBJECT:
                            {
								// 이동 후 대기
							    if ( m_lpObjControl->GetClientEnemyType( pTempCreature->GetCharID() ) == EnemyCheck::EC_ENEMY )
							    {
								    if ( g_DeviceInput.GetIsLeftMouseDown() )
								    {
									    if ( m_lpObjControl->m_bAttackable )
										    m_lpObjControl->m_bAttackResult = TRUE ;
									    else
										    m_lpObjControl->m_bAttackResult = FALSE ;
								    }

								    // 이동 후 공격
								    if ( m_lpObjControl->m_bAttackable )
								    {
									    if ( m_dwNormalTargetID != 0xFFFFFFFF )
									    {
										    m_lpObjControl->m_bClickMove = false ;
										    if ( m_lpObjControl->CheckAttackable() )
										    {
											    m_lpObjControl->m_bClickAttack = true ;
										    }
										    else
										    {
											    m_lpObjControl->m_bClickAttack = false ;
										    }
										    m_bClickPlayerAndNpc = FALSE ;
									    }
									    else
									    {
										    m_lpObjControl->m_bClickMove		= true ;
										    m_lpObjControl->m_bClickAttack		= false ;
										    m_bClickPlayerAndNpc	= FALSE ;
									    }
								    }
								    else
								    {
									    m_lpObjControl->m_bClickMove		= false ;
									    m_lpObjControl->m_bClickAttack		= false ;
									    m_bClickPlayerAndNpc	= FALSE ;
								    }
							    }
							    else
							    {
								    if ( m_dwNormalTargetID != 0xFFFFFFFF )
								    {
									    m_lpObjControl->m_bClickMove		= false ;
									    m_lpObjControl->m_bClickAttack		= false ;
									    m_bClickPlayerAndNpc	= TRUE ;
								    }
								    else
								    {
									    m_lpObjControl->m_bClickMove		= true ;
									    m_lpObjControl->m_bClickAttack		= false ;
									    m_bClickPlayerAndNpc	= FALSE ;
								    }
							    }
								break;
                            }

							case Creature::CT_NPC:
                            {
								// 이동 후 대화
								if ( m_dwNormalTargetID != 0xFFFFFFFF )
								{
									m_lpObjControl->m_bClickMove		= false;
									m_lpObjControl->m_bClickAttack		= false;
									m_bClickPlayerAndNpc	= TRUE;
								}
								else
								{
									m_lpObjControl->m_bClickMove		= true;
									m_lpObjControl->m_bClickAttack		= false;
									m_bClickPlayerAndNpc	= FALSE;
								}
								break;
                            }

							case Creature::CT_MONSTER:
                            {
								// 적이 아닌 몬스터의 경우 무시
								if ( m_lpObjControl->GetClientEnemyType( pTempCreature->GetCharID() ) == EnemyCheck::EC_ENEMY )
								{
									if ( g_DeviceInput.GetIsLeftMouseDown() )
									{
										if ( m_lpObjControl->m_bAttackable )
										{
											m_lpObjControl->m_bAttackResult = TRUE;
										}
										else
										{
											m_lpObjControl->m_bAttackResult = FALSE;
										}
									}

									// 이동 후 공격
									if ( m_lpObjControl->m_bAttackable )
									{
										if ( m_dwNormalTargetID != 0xFFFFFFFF )
										{
											m_lpObjControl->m_bClickMove = false;
											
											if ( m_lpObjControl->CheckAttackable() )
											{
												m_lpObjControl->m_bClickAttack = true;
											}
											else
											{
												m_lpObjControl->m_bClickAttack = false;
											}

											m_bClickPlayerAndNpc = FALSE;
										}
										else
										{
											m_lpObjControl->m_bClickMove		= true;
											m_lpObjControl->m_bClickAttack		= false;
											m_bClickPlayerAndNpc	= FALSE;
										}
									}
									else
									{
										m_lpObjControl->m_bClickMove		= false;
										m_lpObjControl->m_bClickAttack		= false;
										m_bClickPlayerAndNpc	= FALSE;
									}
								}
								break;
                            }
						}
					}
				}

				if ( pSpecialCreature )
				{
					//==============================================================
					// 스킬 사용
					unsigned long dwFunction;

                    if ( m_lpObjControl->CheckStillCasting( dwFunction ) )
					{
						// 타겟 유무 체크
                        if ( m_lpObjControl->GetClientIdentity( pSpecialCreature->GetCharID() ) != Creature::CT_NPC )
						{
							// 이동 후 스킬
							if ( m_dwSpecialTargetID != 0xFFFFFFFF )
							{
								m_lpObjControl->m_bClickMove		= false;
								m_lpObjControl->m_bClickAttack		= false;
								m_bClickPlayerAndNpc	= FALSE;
								m_bClickSkill			= TRUE;
							} 
							else
							{
								m_lpObjControl->m_bClickMove		= true;
								m_lpObjControl->m_bClickAttack		= false;
								m_bClickPlayerAndNpc	= FALSE;
								m_bClickSkill			= FALSE;
							}
						}
					}
				}

#ifdef _NEW_KEY_
				if ( g_DeviceInput.GetIsLeftMousePress() && m_bLeftMousePress )
#else
				if ( ( g_DeviceInput.GetIsLeftMousePress() && m_bLeftMousePress ) || ( g_DeviceInput.GetIsRightMousePress() && m_bRightMousePress ) )
#endif
				{
					if ( !pTempCreature && !pSpecialCreature )
					{
						vector3 vecLen = m_lpObjControl->m_vecClickPos - *( pSelfCreature->GetPosition() );
						vecEndBack = vecLen;
						vecLen.y = 0.0f;

						if ( vecLen.GetLens() <= TARGETNEAR_RANGE/2 )
						{
							m_lpObjControl->m_bClickMove		= false;
							m_lpObjControl->m_bClickAttack		= false;
							m_bClickPlayerAndNpc	= FALSE;
						} 
						else
						{
							// 이동
							m_lpObjControl->m_bClickMove		= true;
							m_lpObjControl->m_bClickAttack		= false;
							m_bClickPlayerAndNpc	= FALSE;

							fChrToBack = -100.0f;

						}
					}
				}
			}
		}

		if ( m_dwNormalTargetID != 0xFFFFFFFF || m_dwSpecialTargetID != 0xFFFFFFFF )
		{
			m_lpObjControl->SetAutoRun( FALSE );
			m_lpObjControl->m_bAutoFollowMode	= FALSE;
			m_lpObjControl->m_dwAutoFollowChrID = 0xFFFFFFFF;
		}

		if ( !m_bKeyPress )
		{
			if ( m_lpObjControl->GetAutoRun && m_lpObjControl->GetAutoRun() )
			{
				m_lpObjControl->SetKillCounter( 0 );
				m_lpObjControl->SetClientExitCounter( 0 );
				m_lpObjControl->SetLogoutCounter( 0 );
				m_lpObjControl->SetPortalCounter( 0 );
				m_lpObjControl->SetSkillUseCounter( 0 );

				if ( bKeyAble && !bEdge && !g_DeviceInput.GetIsMiddleMousePress() )
				{
					//마우스 연속 누를때 들어 옴
					CSceneManager::PickMakeRay( nMouseX, nMouseY, BaseGraphicsLayer::m_lScreenSx, BaseGraphicsLayer::m_lScreenSy );
					vector3 vecStart = CSceneManager::m_vecPickRayStart;
					vector3 vecEnd	 = CSceneManager::m_vecPickRayStart + 1000000.0f * CSceneManager::m_vecPickRayDir;

					vector3 vecPoly[ 4 ];
					float fSize = 1000000.0f;
					vecPoly[ 0 ] = vecChrPos + vector3( -fSize, 0.0f, -fSize );
					vecPoly[ 1 ] = vecChrPos + vector3( fSize, 0.0f, -fSize );
					vecPoly[ 2 ] = vecChrPos + vector3( -fSize, 0.0f, fSize );
					vecPoly[ 3 ] = vecChrPos + vector3( fSize, 0.0f, fSize );

					float fInter;
					if ( CIntersection::PolygonRay( vecStart, vecEnd, vecPoly, fInter ) )
					{
						m_lpObjControl->m_vecClickPos = vecStart + ( fInter * CSceneManager::m_vecPickRayDir );
					}
					else if ( CIntersection::PolygonRay( vecStart, vecEnd, &vecPoly[ 1 ], fInter ) )
					{
						m_lpObjControl->m_vecClickPos = vecStart + ( fInter * CSceneManager::m_vecPickRayDir );
					}
					else
					{
						m_lpObjControl->m_vecClickPos = CSceneManager::PickMousePos( nMouseX, nMouseY );
					}
				}
				else
				{
					float fChrDir = pSelfCreature->GetDirection();
					fChrDir = -fChrDir;
					fChrDir -= FLOAT_PHI / 2.0f;

					vector3 vecToward( cosf( fChrDir ), 0.0f, sinf( fChrDir ) );
					vecToward.Normalize();
					
                    vecToward *= 100.0f;
					m_lpObjControl->m_vecClickPos = *( pSelfCreature->GetPosition() ) + vecToward;
				}

				m_lpObjControl->m_bClickMove		= true;
				m_lpObjControl->m_bClickAttack		= false;
				m_bClickPlayerAndNpc	= FALSE;
			}

			if ( m_lpObjControl->m_bAutoFollowMode )
			{
				RYLCreature* pTempCreature = RYLCreatureManager::Instance()->GetCreature( m_lpObjControl->m_dwAutoFollowChrID );
				
                if ( pTempCreature )
				{
					vector3 vecLen = *( pTempCreature->GetPosition() ) - *( pSelfCreature->GetPosition() );
					vecLen.y = 0.0f;

					if ( vecLen.GetLens() > 700.0f )
					{
						vecLen.Normalize();
						vecLen *= 490.0f;
						m_lpObjControl->m_vecClickPos = *( pSelfCreature->GetPosition() ) + vecLen;
					}

					m_lpObjControl->m_bClickMove		= true;
					m_lpObjControl->m_bClickAttack		= false;
					m_bClickPlayerAndNpc	= FALSE;
				} 
				else
				{
					m_lpObjControl->m_bAutoFollowMode	= FALSE;
					m_lpObjControl->m_dwAutoFollowChrID = 0xFFFFFFFF;

					m_lpObjControl->m_bClickMove		= false;
					m_lpObjControl->m_bClickAttack		= false;
					m_bClickPlayerAndNpc	= FALSE;
				}
			}
		} 
		else
		{
			m_lpObjControl->SetAutoRun( FALSE );
			m_lpObjControl->m_bAutoFollowMode = FALSE;
			m_lpObjControl->m_dwAutoFollowChrID = 0xFFFFFFFF;
		}

		if ( !pSelfCreature->GetIsDead() )
		{
			// edith 2008.06.25 말탔을때 공격 금지
			// 말을 타고있을땐 캐스팅이 무조건 안된다.
			CRYLGameData* pGame = CRYLGameData::Instance() ;
			if(pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::AVATA] != NULL)
			{
				unsigned char cType = pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::AVATA]->GetItemInfo().m_DetailData.m_cItemType;
				if(cType != Item::ItemType::TRANSFORM && pGame->m_csStatus.m_cRidePos == 1)
				{
					m_lpObjControl->m_bClickAttack = false;
					m_bClickSkill = FALSE;
				}
			}

			//====================================================================
			// 클릭해서 이동
			//====================================================================
			if ( m_lpObjControl->m_bClickMove )
			{
				m_lpObjControl->SetKillCounter( 0 );
				m_lpObjControl->SetClientExitCounter( 0 );
				m_lpObjControl->SetLogoutCounter( 0 );
				m_lpObjControl->SetPortalCounter( 0 );
				m_lpObjControl->SetSkillUseCounter( 0 );

                vecChrToward	= m_lpObjControl->m_vecClickPos - *( pSelfCreature->GetPosition() );

				if( fChrToBack == vecChrToward.y )
					vecChrToward.y =  0.0f;
				
				fChrToBack = vecChrToward.y;

				if ( m_lpObjControl->m_nSelectItem != 0xFFFFFFFF )
				{
					if ( vecChrToward.GetLens() <= 300.0f )
					{
						m_lpObjControl->m_bClickMove		= false;
						m_lpObjControl->m_bClickAttack		= false;
						m_bClickPlayerAndNpc	= FALSE;

						if ( m_lpObjControl->PickItem )
                        {
							m_lpObjControl->PickItem( m_lpObjControl->m_nSelectItem );
                        }

						m_lpObjControl->m_nSelectItem = 0xFFFFFFFF;
					} 
					else
					{
						// 움직일수 있는지 체크
                        if ( !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Hold ) && 
							!pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stun ) &&
							CRYLGameData::Instance()->GetEnableMove() )
						{   
							m_lpObjControl->SetDirectionforMouse( vecChrToward );
                            pSelfCreature->Walk( RYLCreature::MOVE_FOWARD, 0.0f );
						}
					}
				} 
				else
				{
					if ( m_lpObjControl->GetAutoRun && !m_lpObjControl->GetAutoRun() && vecChrToward.GetLens() <= TARGETNEAR_RANGE - 30 ) // 30 보정수치
					{
						m_lpObjControl->m_bClickMove		= false;
						m_lpObjControl->m_bClickAttack		= false;
						m_bClickPlayerAndNpc	= FALSE;
					} 
					else if ( !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Hold ) && 
							 !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stun ) )
					{
						if ( vecChrToward.GetLens() <= ATTACKABLE_RANGE )
						{    // 추가
							m_lpObjControl->SetDirectionforMouse( vecEndBack ) ;
							pSelfCreature->Walk( RYLCreature::MOVE_FOWARD, 0.0f );
						}
						else if( CRYLGameData::Instance()->GetEnableMove() )
						{
							// 움직일수 있는지 체크
							m_lpObjControl->SetDirectionforMouse( vecChrToward );
							pSelfCreature->Walk( RYLCreature::MOVE_FOWARD, 0.0f );
							vecEndBack = vecChrToward;
						}
					}
				}
			}

			//====================================================================
			// 클릭해서 공격 (몬스터 위치로 최대한 가까이 간다)
			//====================================================================
			if ( m_lpObjControl->m_bClickAttack )
			{
				bool bChk = false;

				char strText[MAX_PATH];

				// 장거리 무기일때 //
				switch(CRYLGameData::Instance()->m_csStatus.GetWeaponType())
				{
				case Item::ItemType::ARROW:
				case Item::ItemType::BOLT:
				case Item::ItemType::CROSSBOW:
				case Item::ItemType::BOW:
					{
						unsigned char cIndex = Item::EquipmentPos::WEAPON_HAND1;

						if(CRYLGameData::Instance()->m_csStatus.m_cWeaponPos!=0)
						{
							cIndex = Item::EquipmentPos::WEAPON_HAND2;
						}

						const Item::ItemInfo *pItemInfo;

						pItemInfo = g_ItemMgr.GetItemInfo(CRYLGameData::Instance()->m_csStatus.m_lstEquipment[cIndex]->GetProtoTypeID());

						if(pItemInfo)
						{
							switch(pItemInfo->m_UseLimit.m_cLimitStatus)
							{
							case Item::StatusLimit::STR:
								if(CRYLGameData::Instance()->m_csStatus.GetSTR()<pItemInfo->m_UseLimit.m_wLimitValue)
								{
									sprintf(strText, CRYLStringTable::m_strString[37], pItemInfo->m_UseLimit.m_wLimitValue-CRYLGameData::Instance()->m_csStatus.GetSTR());
									bChk = true;
								}
								break;
							case Item::StatusLimit::DEX:
								if(CRYLGameData::Instance()->m_csStatus.GetDEX()<pItemInfo->m_UseLimit.m_wLimitValue)
								{
									sprintf(strText, CRYLStringTable::m_strString[38], pItemInfo->m_UseLimit.m_wLimitValue-CRYLGameData::Instance()->m_csStatus.GetDEX());
									bChk = true;
								}
								break;
							case Item::StatusLimit::CON:
								if(CRYLGameData::Instance()->m_csStatus.GetINT()<pItemInfo->m_UseLimit.m_wLimitValue)				
								{
									sprintf(strText, CRYLStringTable::m_strString[39], pItemInfo->m_UseLimit.m_wLimitValue-CRYLGameData::Instance()->m_csStatus.GetINT());
									bChk = true;
								}
								break;
							case Item::StatusLimit::INT:
								if(CRYLGameData::Instance()->m_csStatus.GetCON()<pItemInfo->m_UseLimit.m_wLimitValue)
								{		
									sprintf(strText, CRYLStringTable::m_strString[40], pItemInfo->m_UseLimit.m_wLimitValue-CRYLGameData::Instance()->m_csStatus.GetCON());
									bChk = true;
								}
								break;
							case Item::StatusLimit::WIS:
								if(CRYLGameData::Instance()->m_csStatus.GetWIS()<pItemInfo->m_UseLimit.m_wLimitValue)						
								{		
									sprintf(strText, CRYLStringTable::m_strString[41], pItemInfo->m_UseLimit.m_wLimitValue-CRYLGameData::Instance()->m_csStatus.GetWIS());
									bChk = true;
								}
								break;
							}
						}
					}
					break;
				}

				if(bChk)
				{
					CRYLChattingDlg::Instance()->AddMessage(strText, CRYLNetworkData::Instance()->m_dwMyChrID, 129);
				}

				if(!bChk)
				{
					m_lpObjControl->SetKillCounter( 0 );
					m_lpObjControl->SetClientExitCounter( 0 );
					m_lpObjControl->SetLogoutCounter( 0 );
					m_lpObjControl->SetPortalCounter( 0 );
					m_lpObjControl->SetSkillUseCounter( 0 );

					RYLCreature* pTargetCreature = RYLCreatureManager::Instance()->GetCreature( m_dwNormalTargetID );

					if ( pTargetCreature )
					{
						vecChrToward	= *( pTargetCreature->GetPosition() ) - *( pSelfCreature->GetPosition() );
						vecChrToward.y	= 0.0f;

						const char** pstrSelfShape = pSelfCreature->GetShapeString();
						unsigned long dwType = m_lpObjControl->GetWeaponType( 
							const_cast<char*>( pstrSelfShape[ Item::EquipmentPos::WEAPON_HAND1 + pSelfCreature->GetWeaponPos() ] ) );

						float fLength = 1800.0f;

						// edith 2009.01.29 아처 사정거리 로직.
						if ( pSelfData->m_usClass == 8 ) 
						{
							if(dwType == Item::ItemType::BOW)
								fLength += 1200.0f;		// 아쳐 사정거리
							else if(dwType == Item::ItemType::CROSSBOW)
								fLength += 600.0f;		// 아쳐 사정거리
						}
						else if ( pSelfData->m_usClass == 10 ) 
						{
							if(dwType == Item::ItemType::BOW)
								fLength += 300.0f;		//  인챈터 사정거리
						}

						if ( m_lpObjControl->m_bAttackable && ( ( ( dwType == Item::ItemType::BOW || dwType == Item::ItemType::CROSSBOW ) && 
							vecChrToward.GetLens() <= fLength ) || vecChrToward.GetLens() <= ATTACKABLE_RANGE ) )
						{
							m_lpObjControl->m_dwNormalTargetMonsterID = pTargetCreature->GetCharID();

							if ( !m_lpObjControl->m_bAutoTargetAttack )
							{
								m_lpObjControl->m_bClickMove		= false;
								m_lpObjControl->m_bClickAttack		= false;
								m_bClickPlayerAndNpc	= FALSE;
							}

							if ( !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stun ) )
							{
								m_lpObjControl->SetDirectionforMouse( vecChrToward );

								if ( pSelfCreature->Attack() )
								{
									m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
																		pSelfCreature->GetLowerActionID(),
																		pSelfCreature->GetDirection(),
																		*( pSelfCreature->GetPosition() ) );
								}
							}
						} 
						else
						{
							// 움직일수 있는지 체크
							if ( !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Hold ) && 
								!pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stun ) &&
								CRYLGameData::Instance()->GetEnableMove() )
							{
								m_lpObjControl->SetDirectionforMouse( vecChrToward );
								pSelfCreature->Walk( RYLCreature::MOVE_FOWARD, 0.0f );
							}
						}
					}
					else
					{
						m_dwNormalTargetID	= 0xFFFFFFFF;
						m_lpObjControl->m_bClickMove		= false;
						m_lpObjControl->m_bClickAttack		= false;
						m_bClickPlayerAndNpc	= FALSE;
					}
				}
			}

			//====================================================================
			// 클릭해서 대화 / 대기
			//====================================================================
			if ( m_bClickPlayerAndNpc )
			{
				m_lpObjControl->SetKillCounter( 0 );
				m_lpObjControl->SetClientExitCounter( 0 );
				m_lpObjControl->SetLogoutCounter( 0 );
				m_lpObjControl->SetPortalCounter( 0 );
				m_lpObjControl->SetSkillUseCounter( 0 );

				RYLCreature* pTargetCreature = RYLCreatureManager::Instance()->GetCreature( m_dwNormalTargetID );

				if ( pTargetCreature )
				{
					vecChrToward	= *( pTargetCreature->GetPosition() ) - *( pSelfCreature->GetPosition() );
					vecChrToward.y	= 0.0f;

					if ( vecChrToward.GetLens() <= PLAYERTARGETNEAR_RANGE )
					{
						switch( m_lpObjControl->GetClientIdentity( pTargetCreature->GetCharID() ) )
						{
							case Creature::CT_PC:
                                {
								    if ( g_DeviceInput.KeyHold( VK_SHIFT ) )
								    {
                                        if ( m_lpObjControl->GetClientEnemyType( pTargetCreature->GetCharID() ) == EnemyCheck::EC_ENEMY )
										    break;
									    m_lpObjControl->m_bCharacterTargetNear	= TRUE;
									    m_lpObjControl->m_dwNormalTargetID		= m_dwNormalTargetID;
								    }
                                }
								break;

							case Creature::CT_NPC:
							case Creature::CT_STRUCT:
								{
                                    if ( m_lpObjControl->GetClientEnemyType( pTargetCreature->GetCharID() ) == EnemyCheck::EC_ENEMY )
										break;
									m_lpObjControl->m_bCharacterTargetNear	= TRUE;
									m_lpObjControl->m_dwNormalTargetID	 	= m_dwNormalTargetID;
								}
								break;
						}

						m_lpObjControl->m_bClickMove		= false;
						m_lpObjControl->m_bClickAttack		= false;
						m_bClickPlayerAndNpc	= FALSE;
						m_dwNormalTargetID	= 0xFFFFFFFF;
						m_lpObjControl->SetDirectionforMouse( vecChrToward );
					} 
					else
					{
						// 움직일수 있는지 체크
						if ( !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Hold ) &&
							!pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stun ) &&
							CRYLGameData::Instance()->GetEnableMove() )
						{
							m_lpObjControl->SetDirectionforMouse( vecChrToward );
							pSelfCreature->Walk( RYLCreature::MOVE_FOWARD, 0.0f );
						}
					}
				} 
				else
				{
					m_dwNormalTargetID	= 0xFFFFFFFF;
					m_lpObjControl->m_bClickMove		= false;
					m_lpObjControl->m_bClickAttack		= false;
					m_bClickPlayerAndNpc	= FALSE;
				}
			}

			//====================================================================
			// 클릭해서 스킬
			//====================================================================
			if ( m_bClickSkill )
			{
				m_lpObjControl->SetKillCounter( 0 );
				m_lpObjControl->SetClientExitCounter( 0 );
				m_lpObjControl->SetLogoutCounter( 0 );
				m_lpObjControl->SetPortalCounter( 0 );
				m_lpObjControl->SetSkillUseCounter( 0 );

				RYLCreature* pTargetCreature = RYLCreatureManager::Instance()->GetCreature( m_dwSpecialTargetID );

				if ( pTargetCreature )
				{
					vecChrToward   = *( pTargetCreature->GetPosition() ) - *( pSelfCreature->GetPosition() );
					vecChrToward.y = 0.0f;

					if ( vecChrToward.GetLens() <= m_lpObjControl->m_fSkillLength )
					{
						m_lpObjControl->m_bClickMove		= false;
						m_lpObjControl->m_bClickAttack		= false;
						m_bClickPlayerAndNpc	= FALSE;
						m_bClickSkill			= FALSE;

                        if ( !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stun ) )
						{
							m_lpObjControl->SetDirectionforMouse( vecChrToward );

							// 스킬 캐스팅
							CollisionType CT = CSceneManager::GetCharacterCollisionType( pSelfModel );
						
                            if ( CT != CT_WATER && !pSelfCreature->GetSitMode() )
							{
								if ( pSelfCreature->GetLowerActionID() != CA_LEFTDASH
							     && pSelfCreature->GetLowerActionID() != CA_RIGHTDASH
								 && pSelfCreature->GetLowerActionID() != CA_BACKDASH )
								{
									m_lpObjControl->m_dwSpecialTargetID = m_dwSpecialTargetID;

									// edith 2009.05.20 마우스 모드일때의 스킬발동검사
									if ( m_lpObjControl->CheckSkillStart( TRUE ) )
									{
										pSelfCreature->SetStillCasting( true );

										// 이 스킬이 인스턴트면 바로 발동 된다.
										m_lpObjControl->m_dwStartSkillTick    = timeGetTime();
										m_lpObjControl->m_dwCastingSkillTick  = 0;
										m_lpObjControl->m_dwCastingSkillGrade = 0;
									}
								}
							} 
							else
							{
								m_dwSpecialTargetID = 0xFFFFFFFF;
							}
						}
						else
						{
							m_dwSpecialTargetID = 0xFFFFFFFF;
						}
					}
					else
					{
						// 움직일수 있는지 체크
						if ( !pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Hold ) &&
							!pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Stun ) &&
							CRYLGameData::Instance()->GetEnableMove() )
						{
							m_lpObjControl->SetDirectionforMouse( vecChrToward );
							pSelfCreature->Walk( RYLCreature::MOVE_FOWARD, 0.0f );
						}
					}
				}
				else
				{
					m_dwSpecialTargetID	= 0xFFFFFFFF;
					m_lpObjControl->m_bClickMove		= false;
					m_lpObjControl->m_bClickAttack		= false;
					m_bClickPlayerAndNpc	= FALSE;
					m_bClickSkill			= FALSE;
				}
			}

			if ( !m_lpObjControl->m_bClickMove && !m_lpObjControl->m_bClickAttack && !m_bClickPlayerAndNpc && !m_bClickSkill )
			{
				pSelfCreature->Wait();
			}
		}

		if ( !m_lpObjControl->GetAttacking() )
		{
			if ( m_lpObjControl->SetChangeWeapon ) 
				m_lpObjControl->SetChangeWeapon( FALSE );
		}

		vecChrToward.Normalize();
		switch ( pSelfCreature->GetLowerActionID() )
		{
			case CA_RUN:
				CSceneManager::CharacterMovementVector( pSelfModel, 
														vecChrToward * ( m_lpObjControl->m_fRunSpeed / 35.0f ) );
				break;

			case CA_WALK:
				CSceneManager::CharacterMovementVector( pSelfModel, 
														vecChrToward * ( m_lpObjControl->m_fWalkSpeed / 35.0f ) );
				break;

			default:
				CSceneManager::CharacterMovementVector( pSelfModel, 
													    vector3( 0.0f, 0.0f, 0.0f ) );
				break;
		}

        // Minotaurs 자신 이외의 타겟은 빛이 번쩍
		if ( bAvailableTarget && pTempCreature )	
		{
            CZ3DGeneralChrModel* pTempModel = pTempCreature->GetCharModel();

            if ( pTempModel )
            {
			    vector3 vecLightChrPos = *pTempCreature->GetPosition();
			    CSceneManager::AddChrLight( pTempModel, vecLightChrPos + vector3( 0.0f, 200.0f, 0.0f ), D3DXCOLOR(0.f,1.f,0.f,1.f), 400.0f, 6 );
            }
		}

		m_lpObjControl->PlayerProcessOperation( bKeyAble );

		if ( CFrameTimer::GetUpdateTimer( m_lpObjControl->m_nCharMovingPacketTimerID ) >= 1.0f )// && pSelfCreature->IsCancelAction(2))
		{
			if ( pSelfCreature->GetLowerActionID() == CA_WALK
			 || pSelfCreature->GetLowerActionID() == CA_RUN
			 || pSelfCreature->GetLowerActionID() == CA_WALKBACK
			 || pSelfCreature->GetLowerActionID() == CA_WALKLEFT
			 || pSelfCreature->GetLowerActionID() == CA_WALKRIGHT
			 || pSelfCreature->GetLowerActionID() == CA_REST
			 || pSelfCreature->GetLowerActionID() == CA_WAIT
			 || pSelfCreature->GetLowerActionID() == CA_FIRE1
			 || pSelfCreature->GetLowerActionID() == CA_FIRE2

			 || (CA_GATHERSTART <= pSelfCreature->GetLowerActionID() && pSelfCreature->GetLowerActionID() <= CA_GATHEREND)
			 || (CA_SOCIALSTART <= pSelfCreature->GetLowerActionID() && pSelfCreature->GetLowerActionID() <= CA_SOCIALEND)

			 || pSelfCreature->GetLowerActionID() == CA_SHOT
			 || pSelfCreature->GetLowerActionID() == CA_JUMPSHOT
			 || pSelfCreature->GetLowerActionID() == CA_FORWARDJUMP
			 || pSelfCreature->GetLowerActionID() == CA_BACKJUMP
			 || pSelfCreature->GetLowerActionID() == CA_JUMP
			 || pSelfCreature->GetLowerActionID() == CA_FRONTDASH
			 || pSelfCreature->GetLowerActionID() == CA_JUMPATTACK
			 || pSelfCreature->GetLowerActionID() == CA_JUMPATTACK2
			 || pSelfCreature->GetLowerActionID() == CA_FALLDOWN )
			{
				vecChrToward.Normalize();
				vecChrLeft.Normalize();

				vector3 vecMove;
				switch ( pSelfCreature->GetLowerActionID() )
				{
					case CA_FORWARDJUMP:
					case CA_FRONTDASH:
					case CA_RUN:
					    vecMove = vecChrToward * ( m_lpObjControl->m_fRunSpeed / 4.0f );
						break;

					case CA_WALKBACK:	vecMove = vecChrToward * -( m_lpObjControl->m_fWalkSpeed / 4.0f );	break;
					case CA_WALKLEFT:	vecMove = vecChrLeft * ( m_lpObjControl->m_fWalkSpeed / 4.0f );		break;
					case CA_WALKRIGHT:	vecMove = vecChrLeft * -( m_lpObjControl->m_fWalkSpeed / 4.0f );	break;
					case CA_WALK:	    vecMove = vecChrToward * ( m_lpObjControl->m_fWalkSpeed / 4.0f );	break;

					case CA_JUMPATTACK:
					case CA_JUMPATTACK2:
						vecMove = pSelfCreature->GetJumpAttackVector();
						break;

					case CA_BACKJUMP:	vecMove = vecChrToward * -( m_lpObjControl->m_fWalkSpeed / 4.0f );	break;

					default:		    vecMove = vector3( 0.0f, 0.0f, 0.0f );								break;
				}

				vecMove += *( pSelfCreature->GetPosition() );

				CollisionType CT = CSceneManager::GetCharacterCollisionType( pSelfModel );

				if ( CT == CT_WATER )
				{
                    m_lpObjControl->WrapSendMovingPacket( CA_SWIM, CA_SWIM, pSelfCreature->GetDirection(), vecMove );
				} 
				else
				{
					m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
										                  pSelfCreature->GetLowerActionID(),
                                                          pSelfCreature->GetDirection(), 
                                                          vecMove );
				}
			}
		}
	} 
	else
	{
		if ( !RYLCreatureManager::Instance()->GetCreatureListHead() )
            return;

		CSceneManager::CharacterCollisionDetectType( pSelfModel, CDT_FULL );

		float	fAngle    = pSelfModel->GetDirection();
		vector3 vecChrPos = pSelfModel->GetPosition();

		//=====================================================================
		// 마우스 왼쪽 버튼 상태
		static bool bMouseLeftButtonStateChange  = false;
		static bool m_bMouseLeftButtonStatePress = false;
		static bool bMouseRightButtonStateChange = false;
		static bool m_bMouseRightButtonStatePress= false;
		
		//0 타격기 1 원거리 2 셀프
		static int  m_nSkillType = 0;

		if ( KeyPressed( VK_LBUTTON ) || KeyControl::IsKeyState(KeyControl::KEY_COLUM_ATTACK))
		{
			if ( m_bMouseLeftButtonStatePress )
            {
				bMouseLeftButtonStateChange = false;
            }
			else
            {
				bMouseLeftButtonStateChange = false;
            }
			
			m_bMouseLeftButtonStatePress = true;
		}
		else
		{
			if ( m_bMouseLeftButtonStatePress )
            {
				bMouseLeftButtonStateChange = true;
            }
			else
            {
				bMouseLeftButtonStateChange = false;
            }
			
			m_bMouseLeftButtonStatePress = false;
		}

		if ( KeyPressed( VK_RBUTTON ) )
		{
			if ( m_bMouseRightButtonStatePress )
            {
				bMouseRightButtonStateChange = true;
            }
			else
            {
				bMouseRightButtonStateChange = false;
            }

			m_bMouseRightButtonStatePress = false;
		}
		//=====================================================================

		if ( m_lpObjControl->m_bClientUsed )
		{
			nMouseX = g_DeviceInput.GetMouseLocal()->x;
			nMouseY = g_DeviceInput.GetMouseLocal()->y;
		}

		unsigned long dwFunction;
		if ( pSelfCreature->GetStillCasting() && !m_lpObjControl->CheckStillCasting( dwFunction ) )
		{
			m_lpObjControl->m_bSkillExecuted = TRUE;		
		}

		pSelfCreature->InitCharAction();

		m_lpObjControl->CheckAttackedTiming();
		m_lpObjControl->CheckCameraShakeTiming();

		vector3 vecChrToward = vector3( 0.0f, 0.0f, 0.0f );
		
		if ( bKeyAble )
		{
			bool m_bKeyPress = false;
			CollisionType CT = CSceneManager::GetCharacterCollisionType( pSelfModel );

			// 스킬 유지
			
#ifdef _NEW_KEY_
			if ( pSelfCreature->GetStillCasting() && (g_DeviceInput.GetSkillHold() || KeyControl::IsKeyState(KeyControl::KEY_COLUM_CASTING)) )
#else
			if ( pSelfCreature->GetStillCasting() && (g_DeviceInput.GetIsRightMousePress() || KeyControl::IsKeyState(KeyControl::KEY_COLUM_CASTING)) )
#endif
			{
				if ( pSelfCreature->Casting() )
				{
					m_bKeyPress = true;

					m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
										  pSelfCreature->GetLowerActionID(),
										  pSelfCreature->GetDirection(),
										  *( pSelfCreature->GetPosition() ) );
				}
			}

			//===================================================================
			// 스킬 발동 - 마우스로 발동
            if ( CT != CT_WATER && !pSelfCreature->GetSitMode() )
			{
				if ( (g_DeviceInput.GetIsRightMouseDown() || KeyControl::IsKeyState(KeyControl::KEY_COLUM_CASTING))
				 && ( pSelfCreature->GetLowerActionID() != CA_LEFTDASH
				   && pSelfCreature->GetLowerActionID() != CA_RIGHTDASH
				   && pSelfCreature->GetLowerActionID() != CA_BACKDASH ) )
				{
					CRYLGameData* pGame = CRYLGameData::Instance() ;
					if ( pGame->GetAlterSkillEnable() )
					{
						if ( pGame->GetAlterSkillType() == Skill::Type::CHANT )
						{
							CRYLNetworkData::Instance()->SetChant( pGame->m_csStatus.m_lpQuickSelected ) ;
							pGame->m_csStatus.SetActivateSkill( pGame->m_csStatus.m_lpQuickSelected ) ;
							return ;
						}

						if ( pGame->GetAlterSkillType() == Skill::Type::PASSIVE )		
						{
							return ;
						}
					}
					//m_bKeyPress = true;

					// edith 2009.05.20 스킬발동검사 CheckSkillStart 함수추가
					if ( m_lpObjControl->CheckSkillStart( TRUE ) )
					{
						pSelfCreature->SetStillCasting( true );

						// 이 스킬이 인스턴트면 바로 발동 된다.
						m_lpObjControl->m_dwStartSkillTick = timeGetTime();
						m_lpObjControl->m_dwCastingSkillTick = 0;
						m_lpObjControl->m_dwCastingSkillGrade = 0;
					}
				}
			}
			//===================================================================

			if ( !m_bKeyPress )
			{
				//===================================================================
				// 마우스에 픽킹된 캐릭 찾기
				CZ3DGeneralChrModel *pSelectChr = NULL;
				pSelectChr = CSceneManager::PickingCharacter( nMouseX, nMouseY );

				if ( pSelectChr )
				{
					RYLCreature* pTempCreature = RYLCreatureManager::Instance()->GetCreature( pSelectChr );

					if ( pTempCreature )
					{
						if ( pTempCreature->GetIsDead() == TRUE )
						{
							pTempCreature = NULL;
						}
					}

					if ( pTempCreature )
					{
                        CZ3DGeneralChrModel* pTempModel = pTempCreature->GetCharModel();

						if ( pTempModel != pSelfModel )
						{
							vector3 vecLightChrPos;
							pTempModel->GetPosition( vecLightChrPos );
							CSceneManager::AddChrLight( pTempModel, vecLightChrPos + vector3( 0.0f, 200.0f, 0.0f ), 0xff0022ff, 400.0f, 6 );
							
                            if ( ( m_lpObjControl->GetClientEnemyType( pTempCreature->GetCharID() ) == EnemyCheck::EC_ENEMY
                               || m_lpObjControl->GetClientIdentity( pTempCreature->GetCharID() ) == Creature::CT_NPC )
							 && m_lpObjControl->m_bClientUsed 
                             && ( m_bMouseLeftButtonStatePress || KeyPressed( VK_RBUTTON ) ) )
							{	
								vector3		vecResult,vecTargetMonsterPos;
								float		fW;

								pTempModel->GetPosition( vecTargetMonsterPos );						
								vecTargetMonsterPos = vecTargetMonsterPos + vector3( 0.0f, 100.0f, 0.0f );
								BaseGraphicsLayer::TransformVector( vecTargetMonsterPos, vecResult,fW );
								g_DeviceInput.SetCursorPos( ( int )vecResult.x, ( int )vecResult.y );
							}
						}
					}
				}

				//============================================================
				// 컨트롤 키를 누르면 제자리 공격
				//============================================================
				if ( bMouseLeftButtonStateChange == false && KeyPressed( VK_LBUTTON ) && KeyPressed( VK_CONTROL ) )		
				{
					CSceneManager::PickMakeRay( nMouseX, nMouseY, BaseGraphicsLayer::m_lScreenSx, BaseGraphicsLayer::m_lScreenSy );
					vector3 vecStart = CSceneManager::m_vecPickRayStart;
					vector3 vecEnd   = CSceneManager::m_vecPickRayStart + 10000.0f * CSceneManager::m_vecPickRayDir;

					vector3 vecPoly[ 4 ];
					float   fSize = 10000.0f;
					vecPoly[ 0 ] = vecChrPos + vector3( -fSize, 0.0f, -fSize );
					vecPoly[ 1 ] = vecChrPos + vector3(  fSize, 0.0f, -fSize );
					vecPoly[ 2 ] = vecChrPos + vector3( -fSize, 0.0f,  fSize );
					vecPoly[ 3 ] = vecChrPos + vector3(  fSize, 0.0f,  fSize );

					float	fInter;
					vector3 vecMovePos;
					
					if ( CIntersection::PolygonRay( vecStart, vecEnd, vecPoly, fInter ) )
					{
						vecMovePos = vecStart + ( fInter * CSceneManager::m_vecPickRayDir );
					}
					else if ( CIntersection::PolygonRay( vecStart, vecEnd, &vecPoly[ 1 ], fInter ) )
					{
						vecMovePos = vecStart + ( fInter * CSceneManager::m_vecPickRayDir );
					}
					
					vector3 vecMoveDirection = vecMovePos - vecChrPos;
					m_lpObjControl->SetDirectionforMouse( vecMoveDirection );
					{
						if ( pSelfCreature->Attack() )
						{
							m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
												  pSelfCreature->GetLowerActionID(),
												  pSelfCreature->GetDirection(),
												  *( pSelfCreature->GetPosition() ) );
						}
					}

					m_lpObjControl->m_bClickMove = false;
				}

				//============================================================
				// 클릭해서 이동
				//============================================================
				if ( m_lpObjControl->m_bClickMove )
				{
					vecChrToward   = m_lpObjControl->m_vecClickPos - vecChrPos; 	
					vecChrToward.y = 0.0f;

					if ( vecChrToward.GetLens() <= TARGETNEAR_RANGE )
					{
						m_lpObjControl->m_bClickMove = false;

						if ( m_lpObjControl->m_nSelectItem != 0xffffffff )
						{
							if ( m_lpObjControl->PickItem )
								m_lpObjControl->PickItem( m_lpObjControl->m_nSelectItem );

							m_lpObjControl->m_nSelectItem = 0xffffffff;				 	
						}
						return;
					}

					m_lpObjControl->SetDirectionforMouse( vecChrToward );
                    pSelfCreature->Walk( RYLCreature::MOVE_FOWARD, 0.0f );

					if ( pSelfCreature->GetLowerActionID() == CA_RUN )
						CSceneManager::CharacterMovementVector( pSelfModel, vecChrToward * ( 14.0f ) );
				} 
				else if ( m_lpObjControl->m_bClickAttack )
				{
					//============================================================
					// 클릭해서 공격 ( 몬스터 위치로 최대한 가까이 간다
					//============================================================
                    RYLCreature* pTempCreature2 = RYLCreatureManager::Instance()->GetCreature( m_lpObjControl->m_dwClickModeAttackTargetID );

					if ( pTempCreature2 == NULL )
					{	
						m_lpObjControl->m_bClickMove	= false;
						m_lpObjControl->m_bClickAttack	= false;

						return;
					}	

                    CZ3DGeneralChrModel* pTempModel2 = pTempCreature2->GetCharModel();
					vector3 vecTargetPos;

					if ( pTempModel2 == NULL )		 	
					{
						m_lpObjControl->m_bClickMove	= false;
						m_lpObjControl->m_bClickAttack  = false;
					}
					else
					{
						vecTargetPos = pTempModel2->GetPosition();								
					}

					vecChrToward	= vecTargetPos - vecChrPos; 	
					vecChrToward.y	= 0.0f;

					if ( vecChrToward.GetLens() <= ATTACKABLE_RANGE )
					{
						m_lpObjControl->m_bClickMove   = false;
						m_lpObjControl->m_bClickAttack = false;				

                        if ( m_lpObjControl->GetClientEnemyType( pTempCreature2->GetCharID() ) != EnemyCheck::EC_ENEMY )
						{
                            switch ( m_lpObjControl->GetClientIdentity( pTempCreature2->GetCharID() ) )
                            {
							    case Creature::CT_PC:
                                    {
								        if ( g_DeviceInput.KeyHold( VK_SHIFT ) )
									        m_lpObjControl->m_bCharacterTargetNear = TRUE;
                                    }
								    break;

							    case Creature::CT_NPC:
                                    {
								        m_lpObjControl->m_bCharacterTargetNear = TRUE;
                                    }
								    break;
                            }
						}

						if ( m_nSkillType == 0 && KeyPressed( VK_RBUTTON ) && !pSelfCreature->GetStillCasting() )
						{
							// edith 2009.05.20 스킬발동검사 CheckSkillStart 함수추가
							if ( m_lpObjControl->CheckSkillStart( TRUE ) )
							{
								pSelfCreature->SetStillCasting( true );
								m_lpObjControl->m_dwStartSkillTick		= timeGetTime();
								m_lpObjControl->m_dwCastingSkillTick	= 0;
								m_lpObjControl->m_dwCastingSkillGrade	= 0;
							}
						}
					}

					m_lpObjControl->SetDirectionforMouse( vecChrToward );
					pSelfCreature->Walk( RYLCreature::MOVE_FOWARD, 0.0f );
					if ( pSelfCreature->GetLowerActionID() == CA_RUN )
                    {
						CSceneManager::CharacterMovementVector( pSelfModel, vecChrToward * ( 14.0f ) );
                    }
				} 
				else
				{
					if ( !pSelfCreature->GetIsDead() )
					{
						pSelfCreature->Wait();
					}
					CSceneManager::CharacterMovementVector( pSelfModel, vecChrToward );
				}


				//============================================================
				// 자동 공격
				//============================================================
				if ( m_lpObjControl->m_bAutoTargetAttack )
				{
					RYLCreature* pAutoTargetCreature = RYLCreatureManager::Instance()->GetCreature( m_lpObjControl->m_dwClickModeAttackTargetID );

					if ( pAutoTargetCreature && pAutoTargetCreature->GetIsDead() == TRUE )
					{
						m_lpObjControl->m_dwClickModeAttackTargetID = 0xFFFFFFFF;
						m_lpObjControl->m_bClickAttack				=false;
						m_lpObjControl->m_bClickMove				=false;		
					}
					
                    CZ3DGeneralChrModel* pAutoTargetModel = pAutoTargetCreature->GetCharModel();

					if ( pAutoTargetCreature && pAutoTargetModel && pAutoTargetModel != pSelfModel && pAutoTargetCreature->GetIsDead() == FALSE )
					{			
						vector3		vecTargetChrPos, vecLens;
						pAutoTargetModel->GetPosition( vecTargetChrPos );
						vecLens = vecTargetChrPos - vecChrPos;
						
						if ( vecLens.GetLens() < ATTACKABLE_RANGE )
						{
							vector3		vecMoveDirection;
							vecMoveDirection = vecTargetChrPos - vecChrPos;
							m_lpObjControl->SetDirectionforMouse( vecMoveDirection );
							{
								if ( pSelfCreature->Attack() )
								{
									m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
														  pSelfCreature->GetLowerActionID(),
														  pSelfCreature->GetDirection(),
														  *( pSelfCreature->GetPosition() ) );
								}
							}

							m_lpObjControl->m_bClickAttack	= false;
							m_lpObjControl->m_bClickMove	= false;
						}
						else
						{
							m_lpObjControl->m_dwClickModeAttackTargetID = pAutoTargetCreature->GetCharID();
							m_lpObjControl->m_bClickAttack	= true;
						}
					}
					else
					{
						m_lpObjControl->m_dwClickModeAttackTargetID = 0xFFFFFFFF;
						m_lpObjControl->m_bClickAttack				= false;
						m_lpObjControl->m_bClickMove				= false;	 	
					}
				}


				///////////////////////////////////////////////////
				///////////////
				///////////////////////////////////////////////////
				if ( !pSelfCreature->GetIsDead() && ( bMouseLeftButtonStateChange || KeyPressed( VK_LBUTTON ) || KeyPressed( VK_RBUTTON ) ) ) 
				{
					vector3		vecMovePos;
					if ( bMouseLeftButtonStateChange )
					{	
						// 마우스 왼쪽 버튼이 Click됐을 때
						vecMovePos = CSceneManager::PickMousePos( nMouseX, nMouseY );
						if ( vecMovePos.GetLens() <= 10.0f )
							return;

						vector3 vecMovePos = CSceneManager::PickMousePos( nMouseX, nMouseY );

						vector3 vecSeePos = m_lpObjControl->m_Camera. m_vecCameraPos;
						vector3 vecTemp = vector3(  0.0f, 0.0f, 0.0f );
						vecTemp.x = vecSeePos.x - vecMovePos.x;
						vecTemp.z = vecSeePos.z - vecMovePos.z;
						vecTemp.y = vecTemp.z / vecTemp.x * 90;
						vecTemp.x = (cos(vecTemp.y))+(sin(vecTemp.y)); 
						vecTemp.z = (-sin(vecTemp.y)*25.0f)+(cos(vecTemp.y)*25.0f);

						CEffScript *pMousePointEff = new CEffScript;
						CSceneManager::AddEffectScript( pMousePointEff );
					
						pMousePointEff->GetScriptBinData( "point.esf" );
						pMousePointEff->SetStartPos( vecMovePos.x + vecTemp.x, vecMovePos.y, vecMovePos.z + vecTemp.z );			
						pMousePointEff->SetEndPos( vecMovePos.x + vecTemp.x, vecMovePos.y, vecMovePos.z + vecTemp.z );
						
						/*CEffScript* pMousePointEff = new CEffScript;
						CSceneManager::AddEffectScript( pMousePointEff );

						pMousePointEff->GetScriptBinData( "point.esf" );
						pMousePointEff->SetStartPos( vecMovePos.x, vecMovePos.y + 30.0f, vecMovePos.z );			
						pMousePointEff->SetEndPos( vecMovePos.x, vecMovePos.y + 30.0f,vecMovePos.z );
						*/
						POINT *ptMousePos = g_DeviceInput.GetMouseLocal();
						m_lpObjControl->m_nSelectItem = CSceneManager::MouseOnCheckInstanceObject( ptMousePos->x, ptMousePos->y );				
					}
					else if ( KeyPressed( VK_LBUTTON ) )
					{	// 마우스 왼쪽 버튼이 눌리고 있을때
						CSceneManager::PickMakeRay( nMouseX, nMouseY, BaseGraphicsLayer::m_lScreenSx, BaseGraphicsLayer::m_lScreenSy );
						vector3 vecStart = CSceneManager::m_vecPickRayStart;
						vector3 vecEnd   = CSceneManager::m_vecPickRayStart + 10000.0f * CSceneManager::m_vecPickRayDir;

						vector3 vecPoly[ 4 ];
						float fSize  = 10000.0f;
						vecPoly[ 0 ] = vecChrPos + vector3( -fSize, 0.0f, -fSize );
						vecPoly[ 1 ] = vecChrPos + vector3(  fSize, 0.0f, -fSize );
						vecPoly[ 2 ] = vecChrPos + vector3( -fSize, 0.0f,  fSize );
						vecPoly[ 3 ] = vecChrPos + vector3(  fSize, 0.0f,  fSize );

						float fInter;
						if ( CIntersection::PolygonRay( vecStart, vecEnd, vecPoly, fInter ) )
						{
							vecMovePos = vecStart + ( fInter * CSceneManager::m_vecPickRayDir );
						}
						else if ( CIntersection::PolygonRay( vecStart, vecEnd, &vecPoly[ 1 ], fInter ) )
						{
							vecMovePos = vecStart + ( fInter * CSceneManager::m_vecPickRayDir );
						}
					}

					float	fMaxLens = 200.0f;
					m_lpObjControl->m_dwClickModeAttackTargetID = 0xFFFFFFFF;
					m_lpObjControl->m_vecClickPos = vecMovePos;		
					
					if ( pSelectChr )
					{
						RYLCreature* pTempCreature = RYLCreatureManager::Instance()->GetCreature( pSelectChr );				
                        
						// 나를 제외하고 픽킹이 되었을 때
						if ( pTempCreature && pTempCreature->GetIsDead() == FALSE ) 
						{			
                            CZ3DGeneralChrModel* pTempModel = pTempCreature->GetCharModel();

                            if ( pTempModel != pSelfModel )
                            {
							    vector3		vecTargetChrPos, vecLens;
							    pSelectChr->GetPosition( vecTargetChrPos );
							    vecLens = vecTargetChrPos - vecChrPos;
    							
							    if ( vecLens.GetLens() < ATTACKABLE_RANGE )
							    {
								    vector3		vecMoveDirection = vecTargetChrPos - vecChrPos;
								    m_lpObjControl->SetDirectionforMouse( vecMoveDirection );

								    if ( KeyPressed( VK_LBUTTON ) )						
								    {
									    if ( pSelfCreature->Attack() ) 
									    {
										    m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
															                      pSelfCreature->GetLowerActionID(),
                                                                                  pSelfCreature->GetDirection(),
															                      *( pSelfCreature->GetPosition() ) );
									    }
								    }
								    else if ( KeyPressed(VK_RBUTTON) && !pSelfCreature->GetStillCasting())
								    {							
									    if ( m_nSkillType == 0 || m_nSkillType == 1 )
									    {
											// edith 2009.05.20 스킬발동검사 CheckSkillStart 함수추가
											if ( m_lpObjControl->CheckSkillStart( TRUE ) )
											{
												pSelfCreature->SetStillCasting( true );
												m_lpObjControl->m_dwStartSkillTick = timeGetTime();
												m_lpObjControl->m_dwCastingSkillTick = 0;
												m_lpObjControl->m_dwCastingSkillGrade = 0;
											}
									    }
								    }

								    m_lpObjControl->m_bClickAttack = false;
								    m_lpObjControl->m_bClickMove   = false;
							    }
							    else
							    {
								    if ( KeyPressed( VK_RBUTTON ) && m_nSkillType == 1 && !pSelfCreature->GetStillCasting() )
								    {
										// edith 2009.05.20 스킬발동검사 CheckSkillStart 함수추가
										if ( m_lpObjControl->CheckSkillStart( TRUE ) )
										{
											pSelfCreature->SetStillCasting( true );
											m_lpObjControl->m_dwStartSkillTick		= timeGetTime();
											m_lpObjControl->m_dwCastingSkillTick	= 0;
											m_lpObjControl->m_dwCastingSkillGrade	= 0;
										}
								    }
								    else
								    {
									    m_lpObjControl->m_dwClickModeAttackTargetID = pTempCreature->GetCharID();
									    m_lpObjControl->m_bCharacterTargetNear = FALSE;
    									
									    if ( m_lpObjControl->CheckAttackable() )
									    {
										    m_lpObjControl->m_bClickAttack = true;
									    }
									    else
									    {
										    m_lpObjControl->m_bClickAttack = false;
									    }

									    m_lpObjControl->m_bClickMove = false;
								    } 
							    }

							    // 오토 타게팅
							    if ( KeyPressed( VK_CONTROL ) )		
							    {
								    m_lpObjControl->m_dwClickModeAttackTargetID = pTempCreature->GetCharID();
								    m_lpObjControl->m_bCharacterTargetNear		= FALSE;
								    m_lpObjControl->m_bAutoTargetAttack			= true;
								    m_lpObjControl->m_dwAutoTargetID			= m_lpObjControl->m_dwClickModeAttackTargetID;
							    }
							    else
							    {
								    m_lpObjControl->m_bAutoTargetAttack = false;
								    m_lpObjControl->m_dwAutoTargetID	= 0xFFFFFFFF;
							    }
                            }
						}
						else
						{
							m_lpObjControl->m_dwClickModeAttackTargetID = 0xFFFFFFFF;
							m_lpObjControl->m_bClickAttack				= false;
							m_lpObjControl->m_bClickMove				= true;
						}
					}
					else
					{
						m_lpObjControl->m_dwClickModeAttackTargetID = 0xFFFFFFFF;
						m_lpObjControl->m_bClickAttack				= false;

						if ( KeyPressed( VK_RBUTTON ) )
                        {
							m_lpObjControl->m_bClickMove = false;
                        }
						else
                        {
							m_lpObjControl->m_bClickMove = true;
                        }
					}
				}
				else
				{			
					if ( m_lpObjControl->m_bAutoTargetAttack == false )
					{
	//					m_pEnemyAttackEff->SetStartPos(0.0f,0.0f,0.0f);
	//					m_pEnemyAttackEff->SetEndPos(0.0f,0.0f,0.0f);
					}
				}
			}
		} 
		else
		{
			//===============================================================
			// 키입력이 먹지 않을때
			if ( !pSelfCreature->GetIsDead() )
			{
				pSelfCreature->Wait();
			}
			CSceneManager::CharacterMovementVector( pSelfModel, vecChrToward );

			m_lpObjControl->m_dwClickModeAttackTargetID = 0xFFFFFFFF;
			m_lpObjControl->m_bClickAttack				= false;
			m_lpObjControl->m_bClickMove				= false;
		}

		m_lpObjControl->PlayerProcessOperation( bKeyAble );

		if ( CFrameTimer::GetUpdateTimer( m_lpObjControl->m_nCharMovingPacketTimerID ) >= 1.0f && pSelfCreature->IsCancelAction( 2 ) )
		{
			vector3 vecMove;
			switch( pSelfCreature->GetLowerActionID() )
			{
				case CA_WALK:
                    {
					    vecMove = vecChrToward * ( m_lpObjControl->m_fWalkSpeed / 4.0f );
                    }
					break;

				case CA_RUN:
                    {
					    vecMove = vecChrToward * ( m_lpObjControl->m_fRunSpeed / 4.0f );
                    }
					break;

				default:
                    {
					    vecMove = vector3( 0.0f, 0.0f, 0.0f );
                    }
					break;
			}

			vecMove += *( pSelfCreature->GetPosition() );

			CollisionType CT = CSceneManager::GetCharacterCollisionType( pSelfModel );

			if ( CT == CT_WATER )
			{
				m_lpObjControl->WrapSendMovingPacket( CA_SWIM, CA_SWIM, pSelfCreature->GetDirection(), vecMove );
			}
			else
			{
				m_lpObjControl->WrapSendMovingPacket( pSelfCreature->GetUpperActionID(),
									  pSelfCreature->GetLowerActionID(),
                                      pSelfCreature->GetDirection(), 
                                      vecMove );
			}
		}
	}	
}

RYLCreature*	CRYLCharacterControl::MouseClickModeSelectSpecialTarget( RYLCreature* pCreature, int nMouseX, int nMouseY )
{
	RYLCreature* pSpecialCreature = NULL;

	if (m_lpObjControl->CheckSkillStart)
	{
		// edith 2009.05.20 마우스 모드에서 스킬사용.
		if (TRUE == m_lpObjControl->CheckSkillStart(TRUE))
		{
			m_bRightMousePress = TRUE;
			m_lpObjControl->SetAutoRun(FALSE);
			m_lpObjControl->m_bAutoFollowMode = FALSE;
			m_lpObjControl->m_dwAutoFollowChrID = 0xFFFFFFFF;

			if (NULL != pCreature)
			{
				m_bKeyPress = true;
				m_dwNormalTargetID	= 0xFFFFFFFF;
				m_dwSpecialTargetID	= pCreature->GetCharID();

				if (m_dwSpecialTargetID != m_lpObjControl->m_dwAutoTargetID)
				{
					m_lpObjControl->m_bAutoTargetAttack = false;
					m_lpObjControl->m_dwAutoTargetID	= 0xFFFFFFFF;
				}

				if (m_lpObjControl->CheckTargetforSkill)
				{
					m_lpObjControl->CheckTargetforSkill(m_dwSpecialTargetID, FALSE);
				}

				if (m_dwSpecialTargetID != 0xFFFFFFFF)
				{
					pSpecialCreature = RYLCreatureManager::Instance()->GetCreature(m_dwSpecialTargetID);

					if (!pSpecialCreature)
					{
						m_dwSpecialTargetID = 0xFFFFFFFF;
					}
				}
				// 거리체크
				m_lpObjControl->CheckTargetLength(m_dwSpecialTargetID);
			} 
			else
			{
				// edith 수정 (맵에 마우스 클릭 포인터 제거)
//				m_dwNormalTargetID	= 0xFFFFFFFF;
//				m_dwSpecialTargetID	= 0xFFFFFFFF;
//				m_lpObjControl->m_bAutoTargetAttack = false;
//				m_lpObjControl->m_dwAutoTargetID	= 0xFFFFFFFF;

//				vector3 vecMovePos = CSceneManager::PickMousePos( nMouseX, nMouseY );

//				vector3 vecSeePos = m_lpObjControl->m_Camera. m_vecCameraPos;
//				vector3 vecTemp = vector3(  0.0f, 0.0f, 0.0f );
//				vecTemp.x = vecSeePos.x - vecMovePos.x;
//				vecTemp.z = vecSeePos.z - vecMovePos.z;
//				vecTemp.y = vecTemp.z / vecTemp.x * 90;
//				vecTemp.x = (cos(vecTemp.y))+(sin(vecTemp.y)); 
//				vecTemp.z = (-sin(vecTemp.y)*25.0f)+(cos(vecTemp.y)*25.0f);

//				CEffScript *pMousePointEff = new CEffScript;
//				CSceneManager::AddEffectScript( pMousePointEff );

					
//				pMousePointEff->GetScriptBinData( "point.esf" );
//				pMousePointEff->SetStartPos( vecMovePos.x + vecTemp.x, vecMovePos.y, vecMovePos.z + vecTemp.z );			
//				pMousePointEff->SetEndPos( vecMovePos.x + vecTemp.x, vecMovePos.y, vecMovePos.z + vecTemp.z );

//				CEffScript *pMousePointEff = new CEffScript;
//				CSceneManager::AddEffectScript(pMousePointEff);

//				vector3 vecMovePos = CSceneManager::PickMousePos(nMouseX, nMouseY);
//				pMousePointEff->GetScriptBinData("point.esf");
//				pMousePointEff->SetStartPos(vecMovePos.x, vecMovePos.y + 30.0f, vecMovePos.z);			
//				pMousePointEff->SetEndPos(vecMovePos.x, vecMovePos.y + 30.0f, vecMovePos.z);
			}

			return pSpecialCreature;
		}

		if (TRUE == m_lpObjControl->CheckSkillStart(FALSE))
		{
			m_bRightMousePress	= FALSE;
			m_lpObjControl->SetAutoRun(FALSE);
			m_lpObjControl->m_bAutoFollowMode	= FALSE;
			m_lpObjControl->m_dwAutoFollowChrID = 0xFFFFFFFF;
			m_bKeyPress = true;

			if (m_lpObjControl->m_bAutoTargetAttack)
			{
				if (m_dwSpecialTargetID != 0xFFFFFFFF)
				{
					m_dwNormalTargetID = m_dwSpecialTargetID;
				}

				m_lpObjControl->m_bClickMove = false;
				if (m_lpObjControl->CheckAttackable())
				{
					m_lpObjControl->m_bClickAttack = true;
				}
				else
				{
					m_lpObjControl->m_bClickAttack = false;
				}
				m_bClickPlayerAndNpc = FALSE;
			}

			m_dwSpecialTargetID = 0xFFFFFFFF;
			return pSpecialCreature;
		}
	}

	return pSpecialCreature;
}

VOID		CRYLCharacterControl::RestoreCamera() 
{
	SetCamera(g_DeviceInput.m_ptMovePos.x, g_DeviceInput.m_ptMovePos.y, -1);
}

VOID		CRYLCharacterControl::SetCamera( int dx, int dy, int dz )
{	
    RYLCreature* pSelfCreature = RYLCreatureManager::Instance()->GetSelfCreature();
    if ( !pSelfCreature )
        return;

    CZ3DGeneralChrModel* pSelfModel = pSelfCreature->GetCharModel();
    if ( !pSelfModel )
        return;

    CharDataNode* pSelfData = RYLCharacterDataManager::Instance()->GetCharData( pSelfCreature->GetCharID() );
    if ( !pSelfData )
        return;

	CRYLGameData* pGame = CRYLGameData::Instance() ;
	if(!pGame)
		return;


	// 카메라 각도
	if ( m_lpObjControl->m_nCameraAnimation != 0 )
	{
		// 카메라 애니메이션이 있을 때 (없을땐 그냥 캐릭터 조종모드로 전환)
		return ;
	}

	if ( m_lpObjControl->m_bFreelook)  
	{
		D3DXMATRIX *matPosition = ( D3DXMATRIX* )CSceneManager::GetCamera()->GetMatPosition() ;
		vector3 vecBeforePos    = vector3( matPosition->_41, matPosition->_42, matPosition->_43 ) ;
		float	fDeltaY = dx / 300.0f ;
		float	fDeltaX = dy / 300.0f ;

		if(fDeltaX != 0 || fDeltaY != 0)
		{
			int i =0;
			i++;
		}


		D3DXVECTOR3 vecT( 0.0f, 0.0f, 0.0f ) ;
		D3DXVECTOR3 vecR( 0.0f, 0.0f, 0.0f ) ;

		D3DXMATRIX		matT, matR ;
		D3DXQUATERNION	qR ;	 	

		if ( KeyPressed( VK_MENU ) )
			return ;
		
		if ( KeyPressed( VK_SHIFT ) )
		{
			if ( KeyPressed( 'A' ) || KeyPressed( VK_LEFT ) )	vecT.x -= m_lpObjControl->m_fCameraSpeed ; // Slide Left			
			if ( KeyPressed( 'D' ) || KeyPressed( VK_RIGHT ) )	vecT.x += m_lpObjControl->m_fCameraSpeed ; // Slide Right
			if ( g_DeviceInput.GetIsRightMousePress() )			vecT.y += m_lpObjControl->m_fCameraSpeed ; // Slide Down
			if ( g_DeviceInput.GetIsLeftMousePress() )			vecT.y -= m_lpObjControl->m_fCameraSpeed ; // Slide Up
			if ( KeyPressed( 'W' ) )							vecT.z += m_lpObjControl->m_fCameraSpeed ; // Move Forward
			if ( KeyPressed( 'S' ) )							vecT.z -= m_lpObjControl->m_fCameraSpeed ; // Move Backward	
			if ( KeyPressed( VK_NUMPAD4 ) )						fDeltaY += 0.008f ;
			if ( KeyPressed( VK_NUMPAD6 ) )						fDeltaY -= 0.008f ;
			if ( KeyPressed( VK_NUMPAD8 ) )						
				fDeltaX -= 0.008f ;
			if ( KeyPressed( VK_NUMPAD2 ) )						
				fDeltaX += 0.008f ;
		}
		else
		{
			if ( KeyPressed( 'A' ) || KeyPressed( VK_LEFT ) )	vecT.x -= 10.0f ;			// Slide Left
			if ( KeyPressed( 'D' ) || KeyPressed( VK_RIGHT ) )	vecT.x += 10.0f ;			// Slide Right
			if ( g_DeviceInput.GetIsRightMousePress() )			vecT.y += 10.0f ;			// Slide Down
			if ( g_DeviceInput.GetIsLeftMousePress() )			vecT.y -= 10.0f ;			// Slide Up
			if ( KeyPressed( 'W' ) )
				vecT.z += 10.0f ; // Move Forward
			if ( KeyPressed( 'S' ) )
				vecT.z -= 10.0f ; // Move Backward	

			// Shift 를 눌러야만 회전할수 있게 한다.
			if ( KeyPressed( VK_NUMPAD4 ) )						fDeltaY += 0.008f ; 
			if ( KeyPressed( VK_NUMPAD6 ) )						fDeltaY -= 0.008f ;
			if ( KeyPressed( VK_NUMPAD8 ) )						fDeltaX -= 0.008f ;
			if ( KeyPressed( VK_NUMPAD2 ) )						fDeltaX += 0.008f ;
		}	
		
		
		float	fRotationX = CSceneManager::GetCamera()->GetRotationX() ;
		float	fRotationY = CSceneManager::GetCamera()->GetRotationY() ;
		
		D3DXVECTOR3		vecPos ;

		vecPos.x = matPosition->_41 ;
		vecPos.y = matPosition->_42 ;
		vecPos.z = matPosition->_43 ;
		
		D3DXMatrixIdentity( matPosition ) ;

		fRotationX -= fDeltaX ;
		fRotationY -= fDeltaY ;

		D3DXQuaternionRotationYawPitchRoll( &qR,fRotationY, fRotationX, 0.0f ) ;
		D3DXMatrixRotationQuaternion( &matR, &qR ) ;
		D3DXMatrixMultiply( matPosition, &matR, matPosition ) ;	
		
		matPosition->_41 = vecPos.x ;
		matPosition->_42 = vecPos.y ;
		matPosition->_43 = vecPos.z ;

		D3DXMatrixTranslation( &matT, vecT.x, vecT.y, vecT.z ) ;
		D3DXMatrixMultiply( matPosition, &matT, matPosition ) ;		

		CSceneManager::GetCamera()->SetRotaitionX( fRotationX ) ;
		CSceneManager::GetCamera()->SetRotaitionY( fRotationY ) ;

		vector3 vecCharPos ;	
		CollisionType CT ;
		vector3 vecOldPos	= vector3( matPosition->_41, matPosition->_42, matPosition->_43 ) ;
		vecCharPos			= vecOldPos ;
		vector3 vecDir		= vecOldPos - vecCharPos ;

		vecCharPos = CSceneManager::m_CollisionDetection.GetPosition( vecOldPos, vecDir, vecBeforePos, CT, true ) ;			
			
		matPosition->_41 = vecCharPos.x ;
		matPosition->_42 = vecCharPos.y ;
		matPosition->_43 = vecCharPos.z ;

        vector3 vecTempPos( CSceneManager::GetCamera()->GetMatPosition()->_41,
							CSceneManager::GetCamera()->GetMatPosition()->_42,
							CSceneManager::GetCamera()->GetMatPosition()->_43 );

        pSelfCreature->SetPosition( vecTempPos );

		CSceneManager::GetCamera()->SetVecPosition( vector3( CSceneManager::GetCamera()->GetMatPosition()->_41,
															 CSceneManager::GetCamera()->GetMatPosition()->_42,
															 CSceneManager::GetCamera()->GetMatPosition()->_43 ) ) ;		

		matrix *matPos=CSceneManager::GetCamera()->GetMatPosition() ;
		matrix matInv ;
		matInv.Inverse( *matPos ) ;
		
		CSceneManager::GetCamera()->SetMatView( matInv ) ;		
		CSceneManager::GetCamera()->MoveFrustum() ;
		return ;
	}

	// m_MouseClickMode가 1일때는 전투모드
	if ( m_lpObjControl->m_bClientUsed )
	{
		if( KeyControl::IsKeyState(KeyControl::KEY_COLUM_DIRECTION_LEFT))
		{
			dx = 12;
		}

		if( KeyControl::IsKeyState(KeyControl::KEY_COLUM_DIRECTION_RIGHT))
		{
			dx = -12;
		}

		if( KeyControl::IsKeyState(KeyControl::KEY_COLUM_DIRECTION_UP))
		{
			dy = 12;
		}

		if( KeyControl::IsKeyState(KeyControl::KEY_COLUM_DIRECTION_DOWN))
		{
			dy = -12;
		}

		dx =- dx ;
		dy =- dy ;

		if ( dx )
		{
			if ( m_lpObjControl->m_MouseClickMode == 1 )
			{
				// edith RButton 카메라 회전 전투모드
#ifdef _NEW_KEY_
				if ( g_DeviceInput.GetIsRightMousePress() && pGame->m_bShowCursor == FALSE)
#else
				if ( KeyPressed( VK_MBUTTON ) )
#endif
				{
					// 키보드 모드에서 여기로 들어온다.
					m_lpObjControl->m_Camera.m_fCameraRotX += ( -dx ) / 300.0f ;
				} 
				else
				{
					float	fMoveDir = dx / 300.0f ;
                    float   fChrDir  = pSelfCreature->GetDirection();

					m_lpObjControl->m_Camera.m_fCameraRotX -= fMoveDir ;					
					fChrDir += fMoveDir ;
					
                    if ( !pSelfCreature->GetIsDead() )
                    {
                        pSelfModel->SetDirection( fChrDir );
                    }
				}
			} 
#ifdef _NEW_KEY_
			else if(g_DeviceInput.GetIsRightMousePress() && pGame->m_bShowCursor == FALSE)
#else
			else
#endif
			{
				// 마우스 모드에서 여기로 들어온다. 전투모드
				m_lpObjControl->m_Camera.m_fCameraRotX += ( -dx ) / 300.0f ;
			}
		}

		if ( dy )
		{
			if ( m_lpObjControl->m_MouseClickMode == 1 )
			{
				m_lpObjControl->m_Camera.m_fBattleCameraRotY += dy / 300.0f ;
			}
			else
			{
				m_lpObjControl->m_Camera.m_fCameraRotY += dy / 300.0f ;
			}
		}

		if ( dz )
		{
			if ( m_lpObjControl->m_MouseClickMode == 1 )
			{
				m_lpObjControl->m_Camera.m_fBattleInterCharacterCamera += dz * 20.0f ;
				
				if ( m_lpObjControl->m_Camera.m_fBattleInterCharacterCamera < 100.0f )  
					m_lpObjControl->m_Camera.m_fBattleInterCharacterCamera = 100.0f ;

				if ( m_lpObjControl->m_Camera.m_fBattleInterCharacterCamera > 1000.0f )
					m_lpObjControl->m_Camera.m_fBattleInterCharacterCamera = 1000.0f ;
			}
			else
			{
				m_lpObjControl->m_Camera.m_fMouseClickInterCharacterCamera += dz * 20.0f ;
				
				if ( m_lpObjControl->m_Camera.m_fMouseClickInterCharacterCamera < 100.0f ) 
					m_lpObjControl->m_Camera.m_fMouseClickInterCharacterCamera = 100.0f ;

				if ( m_lpObjControl->m_Camera.m_fMouseClickInterCharacterCamera > 1000.0f ) 
					m_lpObjControl->m_Camera.m_fMouseClickInterCharacterCamera = 1000.0f ;
			}
		}
	}
	else
	{
		if ( dx )
		{
			if ( m_lpObjControl->m_MouseClickMode == 1 )
			{
				// edith 마우스 오른쪽 버튼 눌렀을때 카메라 처리
#ifdef _NEW_KEY_
				if ( g_DeviceInput.GetIsRightMousePress() && pGame->m_bShowCursor == FALSE)
#else
				if ( KeyPressed( VK_MBUTTON ) )
#endif
				{
					// 키보드 모드에서 여기로 들어온다. 미전투모드
					m_lpObjControl->m_Camera.m_fCameraRotX += ( -dx ) / 300.0f ;
				} 
				else
				{
					float	fMoveDir = dx / 300.0f ;
                    float   fChrDir  = pSelfCreature->GetDirection();

					m_lpObjControl->m_Camera.m_fCameraRotX -= fMoveDir ;					
					fChrDir += fMoveDir ;

                    if ( !pSelfCreature->GetIsDead() )
                    {
                        pSelfModel->SetDirection( fChrDir );
                    }
				}
			}
#ifdef _NEW_KEY_
			else if(g_DeviceInput.GetIsRightMousePress() && pGame->m_bShowCursor == FALSE)
#else
			else
#endif
			{
				// 마우스 모드에서 여기로 들어온다. 미전투모드
				m_lpObjControl->m_Camera.m_fCameraRotX += ( -dx ) / 300.0f ;
			}
		}
		
		if ( dy )  
		{
			if ( m_lpObjControl->m_MouseClickMode == 1 )
			{
				m_lpObjControl->m_Camera.m_fBattleCameraRotY += ( -dy ) / 300.0f ;
			}
			else
			{
				m_lpObjControl->m_Camera.m_fCameraRotY += (-dy) / 300.0f;
			}			
		}

		if (m_lpObjControl->m_Camera.m_nCameraMode==0)
			m_lpObjControl->m_Camera.m_fBattleInterCharacterCamera += g_DeviceInput.m_lMMoveZ * 10.0f;
		else
			m_lpObjControl->m_Camera.m_fMouseClickInterCharacterCamera += g_DeviceInput.m_lMMoveZ * 10.0f;

		if (dx)
		{
			float fMoveDir = dx/ 300.0f;
            float   fChrDir  = pSelfCreature->GetDirection();

			fChrDir += fMoveDir;
			if (m_lpObjControl->m_MouseClickMode==1)
			{
                if ( !pSelfCreature->GetIsDead() )
                {
                    pSelfModel->SetDirection( fChrDir );
                }
			}
		}
	}	

    vector3 vecChrPos;
	pSelfModel->GetPosition( vecChrPos );

	float fChrDir = pSelfModel->GetDirection();
    fChrDir -= pSelfCreature->GetFixAngle();

	float fInitY = 150.0f;

	if ( pSelfCreature->GetLowerActionID() == CA_SITDOWN || pSelfCreature->GetLowerActionID() == CA_STANDUP )
	{
		if ( pSelfModel )
		{
			CZ3DObject *lpObject = pSelfModel->GetSkeletonPartObject( "CHEST" );

			if ( lpObject != NULL )
			{
				matrix *lpMatrix = lpObject->GetTM();

				if ( lpMatrix != NULL )
				{
					fInitY = lpMatrix->_42; //무조건 ch
				}
			}

            switch ( pSelfData->m_cRace )
			{
				case 0:	// 인간
                    {
					    switch ( pSelfData->m_cSex )
					    {
						    case 1:
                                {
                                    fInitY = fInitY + 34.0f;
                                }
							    break;

						    case 2:
                                {
							        fInitY = fInitY + 30.0f;
                                }
							    break;
					    }
                    }
					break;

				case 1:	// 아칸
                    {
					    switch ( pSelfData->m_cSex )
					    {
						    case 1:
                                {
							        fInitY = fInitY + 6.0f;
                                }
							    break;

						    case 2:
                                {
							        fInitY = fInitY + 19.0f;
                                }
							    break;
					    }
                    }
					break;
			}
		}
	}
	else if ( pSelfCreature->GetLowerActionID() == CA_REST )
	{
        switch ( pSelfData->m_cRace )
		{
			case 0:	// 인간
                {
				    switch ( pSelfData->m_cSex )
				    {
					    case 1:
                            {
						        fInitY = 54.0f;
                            }
						    break;

					    case 2:
                            {
						        fInitY = 62.0f;
                            }
						    break;
				    }
                }
				break;

			case 1:	// 아칸
                {
				    switch ( pSelfData->m_cSex )
				    {
					    case 1:
                            {
						        fInitY = 89.0f;
                            }
						    break;

					    case 2:
                            {
						        fInitY = 176.0f;
                            }
						    break;
				    }
                }
				break;
		}
	}

	if  (m_lpObjControl->m_MouseClickMode == 1 )
	{
		// edith 마우스 RButton을 누른후 카메라를 실제로 회전하는 회전값
#ifdef _NEW_KEY_
		if ( g_DeviceInput.GetIsRightMousePress() && pGame->m_bShowCursor == FALSE)
#else
		if ( KeyPressed( VK_MBUTTON ) )
#endif
		{
			fChrDir = m_lpObjControl->m_Camera.m_fDirection-m_lpObjControl->m_Camera.m_fCameraRotX;
//			fChrDir -= m_lpObjControl->m_Camera.m_fCameraRotX ;
			m_lpObjControl->m_Camera.m_fCameraRotX = 0;
			m_lpObjControl->m_Camera.UpdateBattleCharacter( vecChrPos, fChrDir, fInitY ) ;
		} 
		else
		{
			// 이건 키보드 모드일때 마우스가 이동할때 사용된다.
			fChrDir -= FLOAT_PHI / 2.0f ;
			m_lpObjControl->m_Camera.UpdateBattleCharacter( vecChrPos, fChrDir, fInitY ) ;
		}
	}
	else
		m_lpObjControl->m_Camera.UpdateClickMode( vecChrPos, fInitY ) ;
}

//--------------------------------------------------------------------------------------------------------------------------
// CRYLCharacterControl::MakeAttackInfo()
bool CRYLCharacterControl::MakeAttackInfo( unsigned short wSkill, unsigned char cSkillLock, unsigned char cSkillLevel, 
											   unsigned char cAtCount, float fAttackAngle, float fAttackRate, BOOL bAngle, BOOL bGod )
{
	// 밀리계열의 스킬만 온다.
    RYLCreature* pSelfCreature = RYLCreatureManager::Instance()->GetSelfCreature();
    if ( !pSelfCreature )
        return false;

    CharDataNode* pSelfData = RYLCharacterDataManager::Instance()->GetCharData( pSelfCreature->GetCharID() );
    if ( !pSelfData )
        return false;

    CZ3DGeneralChrModel* pSelfModel = pSelfCreature->GetCharModel();
    if ( !pSelfModel )
        return false;
	
	// Hide 캐릭터가 공격하지 못하도록
	if ( pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Hide ) || pSelfCreature->GetRideEnable() ) 
		return false;	

	// edith 2008.06.25 말탔을때 공격 금지
	// 말을 타고 있으면 공격못하게 
	CRYLGameData* pGame = CRYLGameData::Instance() ;
	if(pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::AVATA] != NULL)
	{
		unsigned char cType = pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::AVATA]->GetItemInfo().m_DetailData.m_cItemType;
		if(cType != Item::ItemType::TRANSFORM && pGame->m_csStatus.m_cRidePos == 1)
			return false;
	}



    float		fAngle = ( -pSelfModel->GetDirection() ) - FLOAT_PHI / 2.0f;
    vector3		vecSelfDir( cosf( fAngle ), 0.0f, sinf( fAngle ) );
    vector3     vecEnemyDir;

	vector3		vecLens;
	float		fInterLens; 	

	matrix		matSelfChrPos;
	matSelfChrPos.MakeIdent();
    matSelfChrPos._41 = pSelfCreature->GetPosition()->x;
    matSelfChrPos._43 = pSelfCreature->GetPosition()->z;

	vector3			vecFirstAttackPos;
	unsigned long	dwDefenser[ 10 ];
	int				nAttackCount = 0;
	unsigned long	dwType, dwNationType;
	ZeroMemory( &dwDefenser, sizeof( unsigned long ) * 10 );
    
    vector<unsigned long> vecCharID;
    RYLCreatureManager::Instance()->GetCharIDArray( vecCharID );

    if ( vecCharID.size() < 2 )
        return false;

	std::vector<unsigned long>::iterator pos = vecCharID.begin();
	std::vector<unsigned long>::iterator end = vecCharID.end();

	for(; pos != end; ++pos)
	{
		RYLCreature* pTempCreature = RYLCreatureManager::Instance()->GetCreature(*pos); 
        if ( NULL == pTempCreature )
            continue;

        if ( pTempCreature == pSelfCreature )
            continue;

        if ( pTempCreature->GetIsDead() )
            continue;

        // Hide 캐릭터 때리지 못하게
		if ( pTempCreature->GetEffect()->GetChantStatus( Skill::SpellID::Hide ) || pTempCreature->GetRideEnable() )
            continue;
        
        CZ3DGeneralChrModel* pTempModel = pTempCreature->GetCharModel();
        if ( NULL == pTempModel )
            continue;

        CharDataNode* pTempData = RYLCharacterDataManager::Instance()->GetCharData( pTempCreature->GetCharID() );
        if ( NULL == pTempData )
            continue;
		/*
		// 공성 진지 OBB Collision
        bool bOBBEnable = pTempCreature->GetOBBEnable();
		if ( bOBBEnable && EnemyCheck::EC_ENEMY == m_lpObjControl->GetClientEnemyType( pTempCreature->GetCharID() ) )
		{
			vector3 m_vExtent( 30, 85, 30 );

            pSelfCreature->GetOBB()->SetStaticParameter( m_vExtent );
			
			vector3 vecPos;
			matrix	matRot;

			matRot.MakeIdent();
			float fAngle = ( -pSelfModel->GetDirection() ) - FLOAT_PHI / 2.0f;
			matRot.YRotation( fAngle );

			vecPos = *( pSelfCreature->GetPosition() );
			vecPos.y += 85;

            pSelfCreature->GetOBB()->SetDynamicParameter( vecPos, matRot );
			
			CZ3DBoundingVolumeObject::_SetRenderFlag( true );

            if ( pTempModel->CheckCollision( *( pSelfCreature->GetOBB() ) ) )
			{
				if ( nAttackCount == 0 ) 	
				{
					pTempModel->GetPosition( vecFirstAttackPos );
				}

				float fAttackActionRate = rand() % 6;
				fAttackActionRate += 0.3f;
				fAttackActionRate *= 0.1f;

				if ( fAttackActionRate > 0.5f) 
					fAttackActionRate = 0.5f;

				if ( pTempModel->IsAddonActionFinished( 1 ) )
					pTempModel->AddMotion( "ATTACKED", 3, fAttackActionRate );

				if ( pTempData->m_nChrPattern != 8 )		// 석상 체크
				{
					CSceneManager::CharacterRandomPos( pTempModel, 10, vecSelfDir * 50.0f );
				}

                vector3 vecTargetLightPos = *( pTempCreature->GetPosition() ) - *( pSelfCreature->GetPosition() );
				vecTargetLightPos.Normalize();
                vecTargetLightPos = *( pSelfCreature->GetPosition() ) + vecTargetLightPos * 60.0f;				
				CSceneManager::AddChrLight( pTempModel, vecTargetLightPos + vector3( 0.0f, 100.0f, 0.0f ), 0xffff2200, 400.0f, 6 );

				vector3 vecEffectPos = vecTargetLightPos + vector3( 0.0f, 120.0f, 0.0f );

				CEffScript *test_script = new CEffScript;
				CSceneManager::AddEffectScript( test_script );
				if ( fAttackRate >= 1.0f || m_lpObjControl->m_SkillAttackValue ) 
				{
					test_script->GetScriptBinData( "attack2.esf" );	
				}
				else 
				{
					test_script->GetScriptBinData( "attack1.esf" );
				}

                test_script->SetStartPos( pSelfCreature->GetPosition()->x, 
										  pSelfCreature->GetPosition()->y,
										  pSelfCreature->GetPosition()->z );
				test_script->SetEndPos( vecEffectPos.x, vecEffectPos.y, vecEffectPos.z );
				test_script->SetChr( pSelfModel, pTempModel );

				//particle
				CX3DEffect* pEffect = new CX3DEffect;
				CSceneManager::AddEffect( pEffect );
				if ( fAttackRate >= 1.0f || m_lpObjControl->m_SkillAttackValue )
				{
					pEffect->Load( EFFECTPATH, "attack2_particle.eff" );
					
					if ( m_lpObjControl->m_SkillAttackValue > 0 )
						m_lpObjControl->m_SkillAttackValue --;
				}
				else
				{
					pEffect->Load( EFFECTPATH, "attack1_particle.eff" );
				}
				pEffect->SetLoop( FALSE );				
				pEffect->SetAxis( 0.0f,-( ( 180.0f * fAngle ) / 3.14159f ) + 90.0f + fAttackAngle, 0.0f );
				pEffect->SetCenter(vecEffectPos.x,vecEffectPos.y,vecEffectPos.z );				
                dwDefenser[ nAttackCount++ ] = pTempCreature->GetCharID();
				if ( nAttackCount >= 10 ) 
					break;
			}
		}
		*/

        dwType = m_lpObjControl->GetClientIdentity( pTempCreature->GetCharID() );

		if ( dwType == Creature::CT_MONSTER )
		{
			dwType = m_lpObjControl->GetClientSummon( pTempCreature->GetCharID(), TRUE );

			if ( dwType != Creature::CT_MONSTER ) 
				continue;
		}

		dwNationType = m_lpObjControl->GetClientEnemyType( pTempCreature->GetCharID() );

		if ( dwType == Creature::CT_NPC || dwNationType != EnemyCheck::EC_ENEMY )
			continue;

		if ( bGod && dwType == Creature::CT_PC && pTempCreature->GetEffect()->GetChantStatus( Skill::SpellID::Invincible ) )
			continue;
		
		vecLens    = *( pTempCreature->GetPosition() ) - *( pSelfCreature->GetPosition() );	
		vecLens.y  = 0.0f;
		fInterLens = vecLens.GetLens();

		vector3 vecMin  = vector3( -35.0f, 0.0f, -55.0f );
		vector3 vecMax  = vector3(  35.0f, 0.0f,  65.0f );
		vector3 vecMin2 = vector3( -35.0f, 0.0f,  65.0f );
		vector3 vecMax2 = vector3(  35.0f, 0.0f, -55.0f );

		unsigned long dwCID = 0;
		if (dwType == Creature::CT_MONSTER)
		{
			dwCID = pTempCreature->GetCharID();

			// 해당 몬스터는 게더링 타입으로 고정된 캐릭이기 때문에 검사하지 않느다.
			unsigned long dwMonID = dwCID & 0x0000FFFF;
			if(3100 < dwMonID && dwMonID < 3500)
			{
				continue;
			}
		}
		else if (dwType == Creature::CT_SIEGE_OBJECT)
		{
			dwCID = pTempCreature->GetObjectType();
		}
		
		if (0 != dwCID)
		{
			if ( m_lpObjControl->GetMonsterMinBox ) 
			{
				vecMax = m_lpObjControl->GetMonsterMinBox( dwCID );
				vecMax *= -1.0f;
			}

			if ( m_lpObjControl->GetMonsterMaxBox ) 
			{
				vecMin = m_lpObjControl->GetMonsterMaxBox( dwCID );
				vecMin *= -1.0f;
			}

			if ( m_lpObjControl->GetMonsterMaxBox || m_lpObjControl->GetMonsterMinBox )
			{
				vecMin2 = vector3( vecMin.x, 0.0f, vecMax.z );
				vecMax2 = vector3( vecMax.x, 0.0f, vecMin.z );
			}
		}

		float fEnemyAngle = pTempModel->GetDirection() - FLOAT_PHI;

		matrix	matPos, matInv, matResult;
		matPos.YRotation( fEnemyAngle );
		
		matPos._41 = pTempCreature->GetPosition()->x;
		matPos._43 = pTempCreature->GetPosition()->z;

		matInv.Inverse( matPos );			
		matResult = matSelfChrPos * matInv;

		vector3 vecBoxRad1	= matResult.GetLoc() - vecMax;
		float	fBoxRad1	= vecBoxRad1.GetLens();

		vector3 vecBoxRad2	= matResult.GetLoc() - vecMin;
		float	fBoxRad2	= vecBoxRad2.GetLens();
			
		vector3 vecBoxRad3  = matResult.GetLoc() - vecMax2;
		float	fBoxRad3	= vecBoxRad3.GetLens();

		vector3 vecBoxRad4  = matResult.GetLoc() - vecMin2;
		float	fBoxRad4	= vecBoxRad4.GetLens();
			
		vector3 vecResult	= matResult.GetLoc();

		if(abs(pSelfCreature->GetPosition()->y-pTempCreature->GetPosition()->y)<300)
		{

			if ( ( fBoxRad1 <= 210.0f ) || ( fBoxRad2 <= 210.0f ) || ( fBoxRad3 <= 210.0f ) || ( fBoxRad4 <= 210.0f ) 
			|| ( ( vecMin.x <= vecResult.x ) && ( vecMin.z <= vecResult.z ) && ( vecMax.x >= vecResult.x ) && ( vecMax.z >= vecResult.z ) ) )
			{
				vector3 vecDir( cosf( fEnemyAngle ), 0.0f, sinf( fEnemyAngle ) );

				vecDir.x = cosf( fAngle );
				vecDir.z = sinf( fAngle );
				vecDir.y = 0.0f;

				matrix matDirChr = matSelfChrPos;
				matDirChr._41 += vecDir.x;
				matDirChr._43 += vecDir.z;			

				matDirChr = matDirChr * matInv;

				vecDir = matDirChr.GetLoc() - matResult.GetLoc();		

				if ( !bAngle || ( bAngle && CylinderOBBIntersection( matResult.GetLoc(), vecDir, FLOAT_PHI / 3.0f, vecMax, vecMin, 1210.0f ) ) )
				{	
					if ( nAttackCount == 0 ) 	
					{
						pTempModel->GetPosition( vecFirstAttackPos );					
					}

					vecEnemyDir		  = vecLens.Normalized();						
					float fInterAngle = vecEnemyDir * vecSelfDir;
					fInterAngle		  = acosf( fInterAngle );

					if ( bAngle && fInterAngle > FLOAT_PHI / 3.0f )
						continue;

					float fAttackActionRate = rand() % 6;
					fAttackActionRate += 0.3f;
					fAttackActionRate *= 0.1f;

					if ( fAttackActionRate > 0.5f) 
						fAttackActionRate = 0.5f;

					if ( pTempModel->IsAddonActionFinished( 1 ) )
						pTempModel->AddMotion( "ATTACKED", 3, fAttackActionRate );

					if ( pTempData->m_nChrPattern != 8 )		// 석상 체크
					{
						// By Minbobo (공성관련 오브젝트는 랜덤좌표를 안한다).
						unsigned short wObjectType = pTempCreature->GetObjectType();

						if(!EnemyCheck::IsCastleArms(wObjectType) && !EnemyCheck::IsSiegeArms(wObjectType) && !EnemyCheck::IsCastleNPC(wObjectType))
						{
							CSceneManager::CharacterRandomPos( pTempModel, 10, vecSelfDir * 50.0f );
						}
					}

					vector3 vecTargetLightPos = *( pTempCreature->GetPosition() ) - *( pSelfCreature->GetPosition() );
					vecTargetLightPos.Normalize();
					vecTargetLightPos = *( pSelfCreature->GetPosition() ) + vecTargetLightPos * 60.0f;				
					CSceneManager::AddChrLight( pTempModel, vecTargetLightPos + vector3( 0.0f, 100.0f, 0.0f ), 0xffff2200, 400.0f, 6 );

					vector3 vecEffectPos = vecTargetLightPos + vector3( 0.0f, 120.0f, 0.0f );

					CEffScript *test_script = new CEffScript;
					CSceneManager::AddEffectScript( test_script );

					if ( fAttackRate >= 1.0f || m_lpObjControl->m_SkillAttackValue ) 
					{
						test_script->GetScriptBinData( "attack2.esf" );	
					}
					else 
					{
						test_script->GetScriptBinData( "attack1.esf" );
					}

					test_script->SetStartPos( pSelfCreature->GetPosition()->x, 
											pSelfCreature->GetPosition()->y,
											pSelfCreature->GetPosition()->z );
					test_script->SetEndPos( vecEffectPos.x, vecEffectPos.y, vecEffectPos.z );

					test_script->SetChr( pSelfModel, pTempModel );

					//particle
					CX3DEffect *pEffect = new CX3DEffect;
					CSceneManager::AddEffect( pEffect );
					if ( fAttackRate >= 1.0f || m_lpObjControl->m_SkillAttackValue )
					{
						pEffect->Load( EFFECTPATH, "attack2_particle.eff" );
						
						if ( m_lpObjControl->m_SkillAttackValue > 0 )
							m_lpObjControl->m_SkillAttackValue--;
					}
					else
					{
						pEffect->Load( EFFECTPATH, "attack1_particle.eff" );
					}

					pEffect->SetLoop( FALSE );				
					pEffect->SetAxis( 0.0f,-( ( 180.0f * fAngle ) / 3.14159f ) + 90.0f + fAttackAngle, 0.0f );
					pEffect->SetCenter(vecEffectPos.x,vecEffectPos.y,vecEffectPos.z );				
					dwDefenser[ nAttackCount++ ] = pTempCreature->GetCharID();

					if ( nAttackCount >= 10 ) 
						break;
				}
			}
		}
	}

	if ( nAttackCount > 0 || wSkill & 0x8000 )
	{
		// 일반공격으로 인한 공격
		m_lpObjControl->PlaySound( wSkill, vecFirstAttackPos );
		m_lpObjControl->WrapSendCharAttack( *( pSelfCreature->GetPosition() ),
							                pSelfCreature->GetDirection() - FLOAT_PHI / 2.0f,
							                wSkill, false, cSkillLock, cSkillLevel, cAtCount, nAttackCount, dwDefenser );
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------------------------
// CRYLCharacterControl::MakeSkillAttackInfo()
bool CRYLCharacterControl::MakeSkillAttackInfo( unsigned long dwTargetID, unsigned short wSkill, 
											 unsigned char cSkillLock, unsigned char cSkillLevel, 
											 unsigned char cAtCount, BOOL bGod, BOOL bDead )
{
	// 캐스팅 계열의 스킬만온다 (예를들면 니들스팟, 파볼등등)
    RYLCreature* pSelfCreature = RYLCreatureManager::Instance()->GetSelfCreature();
    if ( !pSelfCreature )
        return false;

    CZ3DGeneralChrModel* pSelfModel = pSelfCreature->GetCharModel();
    if ( !pSelfModel )
        return false;

	if ( pSelfCreature->GetEffect()->GetChantStatus( Skill::SpellID::Hide ) || pSelfCreature->GetRideEnable() )
		return false;

	// edith 2008.06.25 말탔을때 공격 금지
	// 말을 타고 있으면 공격못하게 
	CRYLGameData* pGame = CRYLGameData::Instance() ;
	if(pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::AVATA] != NULL)
	{
		unsigned char cType = pGame->m_csStatus.m_lstEquipment[Item::EquipmentPos::AVATA]->GetItemInfo().m_DetailData.m_cItemType;
		if(cType != Item::ItemType::TRANSFORM && pGame->m_csStatus.m_cRidePos == 1)
			return false;
	}

	unsigned long   dwDefenser[ 10 ];
	int				nAttackCount = 0;
	unsigned long dwType;
	ZeroMemory( dwDefenser, sizeof( unsigned long ) * 10 );

    vector<unsigned long> vecCharID;
    RYLCreatureManager::Instance()->GetCharIDArray( vecCharID );

	std::vector<unsigned long>::iterator pos = vecCharID.begin();
	std::vector<unsigned long>::iterator end = vecCharID.end();

	for(; pos != end; ++pos)
	{
		RYLCreature* pTempCreature = RYLCreatureManager::Instance()->GetCreature(*pos); 
        if ( NULL == pTempCreature )
            continue;

        CZ3DGeneralChrModel* pTempModel = pTempCreature->GetCharModel();
		if ( NULL == pTempModel ) 
			continue;

        if ( !bDead && pTempCreature->GetIsDead() ) 
			continue;

		dwType = m_lpObjControl->GetClientIdentity( pTempCreature->GetCharID() );

        if ( bGod && dwType == Creature::CT_PC && pTempCreature->GetEffect()->GetChantStatus( Skill::SpellID::Invincible ) ) 
			continue;

		if ( pTempCreature->GetCharID() == dwTargetID )
		{
			if ( wSkill == 3 || wSkill == 4 )		// 활
			{
				vector3 vecChrPos;

				float fAttackActionRate = rand() % 6;
				fAttackActionRate *= 0.1f;
				fAttackActionRate += 0.3f;

				if ( fAttackActionRate > 0.5f) 
					fAttackActionRate = 0.5f;

				if ( pTempModel->IsAddonActionFinished( 1 ) )
					pTempModel->AddMotion( "ATTACKED", 3, fAttackActionRate );

				pTempModel->GetPosition( vecChrPos );
				CSceneManager::AddChrLight( pTempModel, vecChrPos + vector3( 0.0f, 200.0f, 0.0f ), 0xffff2200, 400.0f, 6 );

				vector3		vecEffectPos;
				vecEffectPos	= vecChrPos;
				vecEffectPos.y += 120.0f;

				CEffScript *test_script = new CEffScript;
				CSceneManager::AddEffectScript( test_script );
				test_script->GetScriptBinData( "attack1.esf" );
				test_script->SetStartPos( pSelfCreature->GetPosition()->x,
										  pSelfCreature->GetPosition()->y,
										  pSelfCreature->GetPosition()->z );
				test_script->SetEndPos( vecEffectPos.x, vecEffectPos.y, vecEffectPos.z );

				test_script->SetChr( pSelfModel, pTempModel );

				//particle
				CX3DEffect *pEffect = new CX3DEffect;
				CSceneManager::AddEffect( pEffect );
				pEffect->Load( EFFECTPATH,"attack1_particle.eff" );
				pEffect->SetLoop( FALSE );				
				pEffect->SetCenter( vecEffectPos.x, vecEffectPos.y, vecEffectPos.z );
				m_lpObjControl->PlaySound( wSkill, vecEffectPos );
			}

			if(0x9E00 < wSkill && wSkill <= 0x9EFF)
			{
				if(dwTargetID & Creature::MONSTER_BIT)
				{
					// 몬스터 ID를 집어넣은 이유는 몬스터 메니져에서 
					// 모든 몬스터의 프로토타입을 구하는 로직은 최종적으로 클라에 부하를 줄것이기 때문에
					// 1차로 CID로 분석을 해서 검색시의 부담을 덜어준다.
					unsigned long dwMonID = dwTargetID & 0x0000FFFF;
					if(3100 < dwMonID && dwMonID < 3500)
					{
						// 몬스터일때
						const CMonsterMgr::MonsterProtoType *lpMonster = g_MonsterMgr.GetMonsterProtoType( dwMonID );

						if(NULL != lpMonster && MonsterInfo::PATTERN_GATHER == lpMonster->m_MonsterInfo.m_cSkillPattern)
						{
							;
						}
						else
							continue;
					}
					else // 게더링 스킬을 일반 몹에 썻을때는 무시한다.
						continue;
				}
				else // 게더링 스킬을 일반 몹에 썻을때는 무시한다.
					continue;
			}

			dwDefenser[ nAttackCount++ ] = pTempCreature->GetCharID();
		}

		if ( nAttackCount >= 10 )
			break;
	}

	if ( nAttackCount > 0 || (wSkill & 0x8000 && 0x4000 < wSkill && wSkill < 0x5000) )
	{
		// 스킬을 쐈을때 적이 맞고 쓰러질때의 패킷
		// 이패킷을 빼면 몬스터가 스킬을 맞아도 죽지않는다.
		m_lpObjControl->WrapSendCharAttack( *( pSelfCreature->GetPosition() ),
	                                        pSelfCreature->GetDirection() - FLOAT_PHI / 2.0f,
							                wSkill, false, cSkillLock, cSkillLevel, cAtCount, nAttackCount, dwDefenser );
		return true;
	}
	return false;
}
