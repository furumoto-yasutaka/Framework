/*******************************************************************************
*
*	タイトル：	たまごエネミー統括スクリプト
*	ファイル：	eggEnemyManager.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "eggEnemyManager.h"
#include "gameObject.h"
#include "collisionContactRecorder.h"
#include "playerManager.h"

// Aiステート用
#include "eggEnemyState_Search.h"
#include "eggEnemyState_Approach.h"
#include "eggEnemyState_Find.h"
#include "eggEnemyState_HeadAttack.h"
#include "eggEnemyState_KnockBack.h"
#include "eggEnemyState_Wince.h"
#include "eggEnemyState_Stick.h"
#include "eggEnemyState_Death.h"

// アニメーションステート用
#include "eggEnemyAnimationState_None.h"
#include "eggEnemyAnimationState_Find.h"
#include "eggEnemyAnimationState_Approach.h"
#include "eggEnemyAnimationState_HeadAttack.h"
#include "eggEnemyAnimationState_KnockBack.h"
#include "eggEnemyAnimationState_Wince.h"

void EggEnemyManager::Init()
{
	m_CommonParameter = DBG_NEW EnemyCommonParameter();
	m_AnimationState = DBG_NEW EggEnemyAnimationState_None(this,
		EnemyAnimationStateBase::AnimationBaseParam(true, 0.0f, 0.0f));
}

void EggEnemyManager::LateInit()
{
	m_GroundCollision = m_AttachObject->GetChild(0)->GetComponent<CollisionContactRecorder<AABBCollider3D>>();
	m_SearchCollision = m_AttachObject->GetChild(1)->GetComponent<CollisionContactRecorder<OBBCollider3D>>();
	m_AttackCollision = m_AttachObject->GetChild(2)->GetComponent<CollisionContactRecorder<OBBCollider3D>>();
	m_JumpCollision = m_AttachObject->GetChild(3)->GetComponent<CollisionContactRecorder<OBBCollider3D>>();

	m_AiState = DBG_NEW EggEnemyState_Search(this);

	m_AttachObject->GetComponent<AABBCollider3D>()->SetVelocityRef(&m_CommonParameter->m_Velocity, 0.0f);
}

void EggEnemyManager::Uninit()
{
	delete m_CommonParameter;
	delete m_AiState;
	delete m_AnimationState;
}

void EggEnemyManager::Update()
{
#ifdef _DEBUG
	printf("%f                     \n", m_CommonParameter->m_WinceValue);

	switch ((AiStateId)m_AiState->GetStateId())
	{
		using enum AiStateId;
	case Search:		printf("Search              \n");	break;
	case Approach:		printf("Approach            \n");	break;
	case Find:			printf("Find                \n");	break;
	case Attack:		printf("Attack              \n");	break;
	case KnockBack:		printf("KnockBack           \n");	break;
	case Wince:			printf("Wince               \n");	break;
	case Stick:			printf("Stick               \n");	break;
	case Death:			printf("Death               \n");	break;
	default: break;
	}

	// インスペクターから停止が有効になっている場合処理をしない
	if (m_IsStop) { return; }
#endif
	ChangeNextGeneralState();
	ChangeNextAiState();
	ChangeNextAnimationState();

	m_AiState->Update();
	m_AnimationState->Update();
}

void EggEnemyManager::LateUpdate()
{
#ifdef _DEBUG
	// インスペクターから停止が有効になっている場合処理をしない
	if (m_IsStop) { return; }
#endif

	m_AiState->LateUpdate();
	m_AnimationState->LateUpdate();
}

void EggEnemyManager::ChangeNextGeneralState()
{
	using enum GeneralStateId;

	D3DXVECTOR3 v = m_CommonParameter->m_Velocity;
	float nowFrameGravity = m_CommonParameter->m_NowFrameGravity;

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

void EggEnemyManager::RequestNextAiState(int Next)
{
	m_NextAiStateId = (AiStateId)Next;
}

void EggEnemyManager::ChangeNextAiState()
{
	if ((AiStateId)m_AiState->GetStateId() == m_NextAiStateId)
	{
		return;
	}

	EggEnemyStateBase* temp = NULL;

	switch (m_NextAiStateId)
	{
		using enum AiStateId;
	case Search:	temp = DBG_NEW EggEnemyState_Search(this);		break;
	case Approach:	temp = DBG_NEW EggEnemyState_Approach(this);	break;
	case Find:		temp = DBG_NEW EggEnemyState_Find(this);		break;
	case Attack:	temp = DBG_NEW EggEnemyState_HeadAttack(this);	break;
	case KnockBack:	temp = DBG_NEW EggEnemyState_KnockBack(this);	break;
	case Wince:		temp = DBG_NEW EggEnemyState_Wince(this);		break;
	case Stick:		temp = DBG_NEW EggEnemyState_Stick(this);		break;
	case Death:		temp = DBG_NEW EggEnemyState_Death(this);		break;
	default: break;
	}

	if (temp)
	{
		m_RecvAiStateId = (AiStateId)m_AiState->GetStateId();
		delete m_AiState;
		m_AiState = temp;
	}
}

void EggEnemyManager::RequestNextAnimationState(int Next)
{
	m_NextAnimationStateId = (AnimationStateId)Next;
}

void EggEnemyManager::ChangeNextAnimationState()
{
	if (m_AnimationState->GetAnimationStepId() == EnemyAnimationStateBase::AnimationStepId::End)
	{
		return;
	}

	EnemyAnimationStateBase::AnimationBaseParam param = m_AnimationState->GetAnimationBaseParam();

	EnemyAnimationStateBase* temp = NULL;

	switch (m_NextAnimationStateId)
	{
		using enum AnimationStateId;
	case None:			temp = DBG_NEW EggEnemyAnimationState_None(this, param);		break;
	case Find:			temp = DBG_NEW EggEnemyAnimationState_Find(this, param);		break;
	case Approach:		temp = DBG_NEW EggEnemyAnimationState_Approach(this, param);	break;
	case HeadAttack:	temp = DBG_NEW EggEnemyAnimationState_HeadAttack(this, param);	break;
	case KnockBack:		temp = DBG_NEW EggEnemyAnimationState_KnockBack(this, param);	break;
	case Wince:			temp = DBG_NEW EggEnemyAnimationState_Wince(this, param);		break;
	default: break;
	}

	if (temp)
	{
		delete m_AnimationState;
		m_AnimationState = temp;
	}
}

void EggEnemyManager::ShortRollDamage()
{
	if (m_AiState->GetStateId() == (int)AiStateId::Wince)
	{
		ChangeStickState();
	}
	else
	{
		m_CommonParameter->m_WinceValue += 1.0f;

		if (m_CommonParameter->m_WinceValue >= m_WINCE_VALUE_MAX)
		{
			m_CommonParameter->m_WinceValue = 0.0f;
			RequestNextAiState((int)AiStateId::Wince);
		}
		else
		{
			RequestNextAiState((int)AiStateId::KnockBack);
		}
	}
}

void EggEnemyManager::RollDamage(float DistanceRate)
{
	//if (m_AiState->GetStateId() == (int)AiStateId::Wince)
	//{
	//	ChangeStickState();
	//}
	//else
	//{
	//	if (DistanceRate >= m_ROLL_DISTANCE_RATE_STICK_THRESHOLD)
	//	{
	//		ChangeStickState();
	//	}
	//	else
	//	{
	//		m_CommonParameter->m_WinceValue += 1.0f + 2.0f * DistanceRate;
	//		if (m_CommonParameter->m_WinceValue >= m_WINCE_VALUE_MAX)
	//		{
	//			m_CommonParameter->m_WinceValue = 0.0f;
	//			RequestNextAiState((int)AiStateId::Wince);
	//		}
	//		else
	//		{
	//			RequestNextAiState((int)AiStateId::KnockBack);
	//		}
	//	}
	//}

	ChangeStickState();
}

void EggEnemyManager::LongRollDamage()
{

}

void EggEnemyManager::ChangeStickState()
{
	RequestNextAiState((int)AiStateId::Stick);

	GameObject* stickEnemyList = Manager::GetScene()->GetGameObject("Player")->GetComponent<PlayerManager>()->GetStickEnemyList();
	m_AttachObject->SetParent(stickEnemyList, 0, (int)GameObject::SetParentOption::KeepPos);
	if (m_AttachObject->m_LocalPosition.x < 0.0f)
	{
		m_AttachObject->m_LocalPosition.x = -0.25f;
	}
	else
	{
		m_AttachObject->m_LocalPosition.x = 0.25f;
	}
	m_AttachObject->m_LocalPosition.y = -0.3f;

	m_AttachObject->GetComponent<AABBCollider3D>()->SetIsActive(false);
}

#ifdef _DEBUG
/*******************************************************************************
*	インスペクター表示処理
*******************************************************************************/
void EggEnemyManager::DrawInspector()
{
	if (ImGui::CollapsingHeader("EggEnemyManager"))
	{
		ImGui::Checkbox("IsStop", &m_IsStop);
	}
}
#endif
