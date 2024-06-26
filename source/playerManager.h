/*******************************************************************************
*
*	タイトル：	プレイヤー統括スクリプト
*	ファイル：	playerManager.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "notDrawComponent.h"
#include "aabbCollider3d.h"

#include <list>
#include <optional>
#include <string>

class PlayerActionStateBase;
template<class T>
class CollisionContactRecorder_PlayerGround;
template<class T>
class CollisionContactRecorder_PlayerAttack;
class PlayerInputListener;
class PlayerAnimationStateBase;

class PlayerParameter
{
public:
	// 現在値
	int				m_Hp = 5;							// 現在HP
	int				m_HpMax = 5;						// HP最大値
	float			m_InvincibleTime = 0.0f;			// 残り無敵時間
	bool			m_IsGun = false;					// 銃状態か
	bool			m_IsCanAirAvoid = true;				// 空中での回避ができる状態か
	D3DXVECTOR3		m_Velocity = { 0.0f, 0.0f, 0.0f };	// 加速度

	float			m_MoveMagnitude = 0.0f;				// 現在の移動速度(マグニチュード)
	D3DXVECTOR3		m_MoveInput = { 0.0f, 0.0f, 0.0f };	// 移動の入力方向
	float			m_MoveAngleDistance = 0.0f;			// オブジェクトの向きと入力方向との角度差
	float			m_JumpMagnitude = 0.0f;				// 現在のジャンプ力(マグニチュード)
	float			m_TurnSpeed = 0.0f;					// 振り向き速度
	float			m_NowFrameGravity = 0.0f;			// このフレームにかかっている重力
	D3DXQUATERNION	m_NowFrameRotateAngle = Math::GetQuaternionIdentity();	// このフレームに振り向いた量

	// 最大値
	float			m_MoveMagnitudeMax = 0.0f;			// 現在の移動の最大速度

	PlayerParameter() {}
};

class PlayerManager : public NotDrawComponent
{
public:
	enum class InputModeId
	{
		MouseKeyboard = 0,
		Controller,
	};
	enum class GeneralStateId
	{
		Idle = 0,
		Run,
		Float,
		Jump,
	};
	enum class ActionStateId
	{
		Null = 0,
		None,
		Dash,
		Turn,
		Avoid,
		Roll_Left,
		Roll_Right,
		WeaponDeformation,
	};
	enum class AnimationStateId
	{
		Null = 0,
		None_Gun,
		None_Rod,
		Roll_Left,
		Roll_Right,
		WeaponDeformation_Gun,
		WeaponDeformation_Rod,
	};
	enum class ModelPartsId
	{
		Sphere = 0,				// モデル有
		Cover,					// モデル有
		WeaponConnection,		// モデル有
		WeaponCenter,			// モデル無し
		Weapon,					// モデル有
		Length,
	};

private:
	// ステート関係
	GeneralStateId				m_GeneralStateId = GeneralStateId::Idle;	// 汎用ステートID
	PlayerActionStateBase*		m_ActionState = NULL;						// アクションステート
	ActionStateId				m_RecvActionStateId = ActionStateId::Null;	// 1つ前のアクションステート
	ActionStateId				m_NextActionStateId = ActionStateId::Null;	// 次のアクションステートID
	PlayerAnimationStateBase*	m_AnimationState = NULL;
	AnimationStateId			m_NextAnimationState = AnimationStateId::Null;

	// オブジェクト
	GameObject*				m_StickEnemyList = NULL;
	GameObject*				m_ModelParts[(int)ModelPartsId::Length] = {};

	// 連携クラス
	PlayerParameter*		m_Parameter = NULL;			// パラメータ
	PlayerInputListener*	m_InputListener = NULL;		// 入力制御

	// コンポーネント
	CollisionContactRecorder_PlayerGround<AABBCollider3D>*	m_GroundCollision = NULL;
	CollisionContactRecorder_PlayerAttack<OBBCollider3D>*	m_AttackCollision = NULL;

public:
	PlayerManager(GameObject* AttachObject)
		: NotDrawComponent(AttachObject)
	{}
	virtual ~PlayerManager() {}

	void Init(InputModeId InputMode);
	void LateInit() override;
	void Uninit() override;
	void Update() override;
	void LateUpdate() override;

	void RequestNextActionState(ActionStateId Next);		// アクションステートの遷移リクエスト
	void RequestNextAnimationState(AnimationStateId Next);	// アニメーションステートの遷移リクエスト

	void Damage(int Value);

private:
	void ChangeNextGeneralState();							// 汎用ステートの遷移処理
	void ChangeNextActionState();							// アクションステートの遷移処理
	void ChangeNextAnimationState();						// アニメーションステートの遷移処理

public:
	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	PlayerActionStateBase* GetActionState() { return m_ActionState; }
	PlayerAnimationStateBase* GetAnimationState() { return m_AnimationState; }
	ActionStateId GetRecvActionStateId() { return m_RecvActionStateId; }
	GameObject* GetStickEnemyList() { return m_StickEnemyList; }
	GameObject* GetModelParts(ModelPartsId Id) { return m_ModelParts[(int)Id]; }
	PlayerParameter* GetParameter() { return m_Parameter; }
	CollisionContactRecorder_PlayerGround<AABBCollider3D>* GetGroundCollision() { return m_GroundCollision; }
	CollisionContactRecorder_PlayerAttack<OBBCollider3D>* GetAttackCollision() { return m_AttackCollision; }
	PlayerInputListener* GetInputListener() { return m_InputListener; }
	/////////////////////////////

#ifdef _DEBUG
private:
	bool m_IsStop = false;

public:
	void DrawInspector() override;
#endif
};
