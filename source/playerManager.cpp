/*******************************************************************************
*
*	タイトル：	プレイヤー制御スクリプト
*	ファイル：	playerManager.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "playerManager.h"
#include "manager.h"
#include "camera.h"
#include "gameObject.h"
#include "math.h"
#include "collisionContactRecorder_PlayerGround.h"
#include "collisionContactRecorder_PlayerAttack.h"
#include "playerInputListener_Controller.h"
#include "playerInputListener_MouseKeyboard.h"
#include "playerHpManager.h"
#include "input.h"

// アクションステート用
#include "playerActionState_None.h"
#include "playerActionState_Dash.h"
#include "playerActionState_Turn.h"
#include "playerActionState_Avoid.h"
#include "playerActionState_Roll_Left.h"
#include "playerActionState_Roll_Right.h"
#include "playerActionState_WeaponDeformation.h"

// アニメーションステート用
#include "playerAnimationState_None_Gun.h"
#include "playerAnimationState_None_Rod.h"
#include "playerAnimationState_Roll_Left.h"
#include "playerAnimationState_Roll_Right.h"
#include "playerAnimationState_WeaponDeformation_Gun.h"
#include "playerAnimationState_WeaponDeformation_Rod.h"

void PlayerManager::Init(InputModeId InputMode)
{
	m_Parameter = DBG_NEW PlayerParameter();
	m_AnimationState = DBG_NEW PlayerAnimationState_None_Rod(this,
		PlayerAnimationStateBase::AnimationBaseParam(0.0f, 0.0f));

	switch (InputMode)
	{
		using enum InputModeId;
	case MouseKeyboard:
		m_InputListener = DBG_NEW PlayerInputListener_MouseKeyboard(this);
		break;
	case Controller:
		m_InputListener = DBG_NEW PlayerInputListener_Controller(this);
		break;
	}
}

void PlayerManager::LateInit()
{
	using enum ModelPartsId;

	GameObject* weapon = m_AttachObject->GetChild(1)->GetChild(1)->GetChild(0)->GetChild(0)->GetChild(0);
	m_StickEnemyList = weapon->GetChild(0);

	m_GroundCollision = m_AttachObject->GetChild(0)->GetComponent<CollisionContactRecorder_PlayerGround<AABBCollider3D>>();
	m_AttackCollision = weapon->GetComponent<CollisionContactRecorder_PlayerAttack<OBBCollider3D>>();

	m_ModelParts[(int)Sphere] = m_AttachObject->GetChild(1)->GetChild(0);
	m_ModelParts[(int)Cover] = m_AttachObject->GetChild(1)->GetChild(1);
	m_ModelParts[(int)WeaponConnection] = m_ModelParts[(int)Cover]->GetChild(0);
	m_ModelParts[(int)WeaponCenter] = m_ModelParts[(int)WeaponConnection]->GetChild(0);
	m_ModelParts[(int)Weapon] = m_ModelParts[(int)WeaponCenter]->GetChild(0);

	m_ActionState = DBG_NEW PlayerActionState_None(this);
	m_NextActionStateId = m_ActionState->GetStateId();

	m_AttachObject->GetComponent<AABBCollider3D>()->SetVelocityRef(&m_Parameter->m_Velocity, 0.0f);

	GameObject* obj = Manager::GetScene()->GetGameObject(PlayerHpManager::m_PLAYER_HP_MANAGER_OBJ_NAME);
	obj->GetComponent<PlayerHpManager>()->CreateUi(m_Parameter);
}

void PlayerManager::Uninit()
{
	m_InputListener->DeleteElementAll();
	delete m_AnimationState;
	delete m_ActionState;
	delete m_Parameter;
	delete m_InputListener;
}

void PlayerManager::Update()
{
#ifdef _DEBUG
	//if (Input::GetKeyboardTrigger(DIK_1))
	//{
	//	m_Parameter->m_Hp--;
	//}
	//if (Input::GetKeyboardTrigger(DIK_2))
	//{
	//	m_Parameter->m_Hp++;
	//}

	//printf("%d                 \n", m_Parameter->m_Hp);

	//// アクションステートを表示
	//printf("ActionState : ");
	//switch (m_NextActionStateId)
	//{
	//	using enum ActionStateId;
	//case None:				printf("None                   \n\n");	break;
	//case Dash:				printf("Dash                   \n\n");	break;
	//case Turn:				printf("Turn                   \n\n");	break;
	//case Avoid:				printf("Avoid                  \n\n");	break;
	//case ShortRoll_Left:	printf("ShortRoll_Left         \n\n");	break;
	//case ShortRoll_Right:	printf("ShortRoll_Right        \n\n");	break;
	//case Roll_Left:			printf("Roll_Left              \n\n");	break;
	//case Roll_Right:		printf("Roll_Right             \n\n");	break;
	//case LongRoll_Left:		printf("LongRoll_Left          \n\n");	break;
	//case LongRoll_Right:	printf("LongRoll_Right         \n\n");	break;
	//case Knock:				printf("Knock                  \n\n");	break;
	//case DashKnock:			printf("DashKnock              \n\n");	break;
	//case ChargeKnock:		printf("ChargeKnock            \n\n");	break;
	//case DropKnock:			printf("DropKnock              \n\n");	break;
	//case RodScaling:		printf("RodScaling             \n\n");	break;
	//case RodOpen:			printf("RodOpen                \n\n");	break;
	//default: return;
	//}

	//switch (m_AnimationState->GetStateId())
	//{
	//	using enum AnimationStateId;
	//case None:				printf("None                   \n");	break;
	//case None_Extend:		printf("None_Extend            \n");	break;
	//case ShortRoll_Left:	printf("ShortRoll_Left         \n");	break;
	//case ShortRoll_Right:	printf("ShortRoll_Right        \n");	break;
	//case Roll_Left:			printf("Roll_Left              \n");	break;
	//case Roll_Right:		printf("Roll_Right             \n");	break;
	//case RodScaling_Extend:	printf("RodScaling_Extend      \n");	break;
	//case RodScaling_Put:	printf("RodScaling_Put         \n");	break;
	//default: return;
	//}

	// インスペクターから停止が有効になっている場合処理をしない
	if (m_IsStop) { return; }
#endif

	// 入力をキューに記録
	m_InputListener->UpdateQueue();

	// キューを確認してステートに応じた処理を実行
	m_ActionState->Update();

	m_AnimationState->Update();
}

void PlayerManager::LateUpdate()
{
#ifdef _DEBUG
	// インスペクターから停止が有効になっている場合処理をしない
	if (m_IsStop) { return; }
#endif

	m_ActionState->LateUpdate();
	m_AnimationState->LateUpdate();

	// ステート更新
	ChangeNextGeneralState();
	ChangeNextActionState();
	ChangeNextAnimationState();
}

void PlayerManager::ChangeNextGeneralState()
{
	using enum GeneralStateId;

	D3DXVECTOR3 v = m_Parameter->m_Velocity;
	float nowFrameGravity = m_Parameter->m_NowFrameGravity;

	if (m_GroundCollision->GetIsContact())
	{// 接地している
		if (D3DXVec3LengthSq(&v) <= nowFrameGravity * nowFrameGravity)
		{// 動いていない
			m_GeneralStateId = Idle;
		}
		else
		{// 動いている
			m_GeneralStateId = Run;
		}
	}
	else
	{// 接地していない
		if (v.y > 0.0f)
		{// 上昇している
			m_GeneralStateId = Jump;
		}
		else
		{// 降下しているor動きなし
			m_GeneralStateId = Float;
		}
	}
}

void PlayerManager::RequestNextActionState(ActionStateId Next)
{
	m_NextActionStateId = Next;
}

void PlayerManager::ChangeNextActionState()
{
	if (m_NextActionStateId == ActionStateId::Null)
	{
		return;
	}

	PlayerActionStateBase* temp = NULL;

	switch (m_NextActionStateId)
	{
		using enum ActionStateId;
	case None:				temp = DBG_NEW PlayerActionState_None(this);				break;
	case Dash:				temp = DBG_NEW PlayerActionState_Dash(this);				break;
	case Turn:				temp = DBG_NEW PlayerActionState_Turn(this);				break;
	case Avoid:				temp = DBG_NEW PlayerActionState_Avoid(this);				break;
	case Roll_Left:			temp = DBG_NEW PlayerActionState_Roll_Left(this);			break;
	case Roll_Right:		temp = DBG_NEW PlayerActionState_Roll_Right(this);			break;
	case WeaponDeformation:	temp = DBG_NEW PlayerActionState_WeaponDeformation(this);	break;
	}

	if (temp)
	{
		m_RecvActionStateId = m_ActionState->GetStateId();
		delete m_ActionState;
		m_ActionState = temp;
	}

	m_NextActionStateId = ActionStateId::Null;
}

void PlayerManager::RequestNextAnimationState(AnimationStateId Next)
{
	m_NextAnimationState = Next;
	m_AnimationState->PreEnd();
}

void PlayerManager::ChangeNextAnimationState()
{
	if (m_NextAnimationState == AnimationStateId::Null ||
		m_AnimationState->GetAnimationStepId() != PlayerAnimationStateBase::AnimationStepId::End)
	{
		return;
	}

	PlayerAnimationStateBase::AnimationBaseParam param = m_AnimationState->GetAnimationBaseParam();

	PlayerAnimationStateBase* temp = NULL;

	switch (m_NextAnimationState)
	{
		using enum AnimationStateId;
	case None_Gun:				temp = DBG_NEW PlayerAnimationState_None_Gun(this, param);				break;
	case None_Rod:				temp = DBG_NEW PlayerAnimationState_None_Rod(this, param);				break;
	case Roll_Left:				temp = DBG_NEW PlayerAnimationState_Roll_Left(this, param);				break;
	case Roll_Right:			temp = DBG_NEW PlayerAnimationState_Roll_Right(this, param);			break;
	case WeaponDeformation_Gun:	temp = DBG_NEW PlayerAnimationState_WeaponDeformation_Gun(this, param);	break;
	case WeaponDeformation_Rod:	temp = DBG_NEW PlayerAnimationState_WeaponDeformation_Rod(this, param);	break;
	}

	if (temp)
	{
		delete m_AnimationState;
		m_AnimationState = temp;
	}

	m_NextAnimationState = AnimationStateId::Null;
}

void PlayerManager::Damage(int Value)
{
	// 無敵状態でない場合ダメージを与える
	if (m_Parameter->m_InvincibleTime <= 0.0f)
	{
		m_Parameter->m_Hp -= Value;

		if (m_Parameter->m_Hp < 0)
		{
			m_Parameter->m_Hp = 0;
		}

		m_Parameter->m_InvincibleTime = 2.0f;
	}
}

#ifdef _DEBUG
/*******************************************************************************
*	インスペクター表示処理
*******************************************************************************/
void PlayerManager::DrawInspector()
{
	if (ImGui::CollapsingHeader("PlayerManager"))
	{
		ImGui::Checkbox("IsStop", &m_IsStop);
	}
}
#endif
