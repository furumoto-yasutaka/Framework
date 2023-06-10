/*******************************************************************************
*
*	タイトル：	プレイヤーアニメーションステート基底クラス
*	ファイル：	playerAnimationStateBase.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "playerAnimationStateBase.h"
#include "playerManager.h"
#include "gameObject.h"
#include "camera.h"

void PlayerAnimationStateBase::Update()
{
	switch (m_AnimationStepId)
	{
		using enum AnimationStepId;
	case Main:
		MainAnimation();
		break;
	case PreEnd:
		PreEndAnimation();
		break;
	default: break;
	}

	UpdateParentRotateLock();
	if (m_IsCameraRotate)
	{
		UpdateCameraRotate();
	}
}

void PlayerAnimationStateBase::LateUpdate()
{
	D3DXVECTOR3 vel = m_Manager->GetParameter()->m_Velocity;
	vel.y = 0.0f;
	float moveSpeed = D3DXVec3Length(&vel);

	UpdateSphereRotate(moveSpeed);
	UpdateCoverRotate(moveSpeed);
}

void PlayerAnimationStateBase::PreEnd()
{
	if (m_AnimationStepId != AnimationStepId::End)
	{
		m_AnimationStepId = AnimationStepId::PreEnd;
	}
}

void PlayerAnimationStateBase::End()
{
	m_AnimationStepId = AnimationStepId::End;
	m_IsCameraRotate = true;
}

void PlayerAnimationStateBase::UpdateParentRotateLock()
{
	D3DXQUATERNION q = Math::GetQuaternionIdentity();
	D3DXQuaternionInverse(&q, &m_Manager->GetParameter()->m_NowFrameRotateAngle);

	GameObject* rod = m_Manager->GetModelParts(PlayerManager::ModelPartsId::WeaponConnection);
	rod->m_LocalRotation *= q;
}

void PlayerAnimationStateBase::UpdateCameraRotate()
{
	// 現在の棒の位置から元の棒の位置まで割合補間で移動する
	D3DXVECTOR3 cameraForward = Manager::GetScene()->GetActiveCamera()->GetAttachObject()->GetForward();
	cameraForward = { cameraForward.x, 0.0f, cameraForward.z };
	cameraForward = Math::Normalize(cameraForward);

	GameObject* weaponConnection = m_Manager->GetModelParts(PlayerManager::ModelPartsId::WeaponConnection);
	D3DXVECTOR3 weaponForward = weaponConnection->GetForward();
	weaponForward = { weaponForward.x, 0.0f, weaponForward.z };
	weaponForward = Math::Normalize(weaponForward);

	D3DXVECTOR3 y = { 0.0f, 1.0f, 0.0f };
	D3DXVECTOR3 axis;
	D3DXQUATERNION q = Math::GetQuaternionIdentity();

	// 振り向きに使う回転軸を決定
	if (weaponForward == -cameraForward)
	{// z軸と水平な場合
		// 外積の結果がおかしくなるので手動で指定
		axis = y;
	}
	else
	{// z軸と水平ではない場合
		// 外積を使うことで近い方から回れる回転軸を求められる
		D3DXVec3Cross(&axis, &weaponForward, &cameraForward);
	}

	// 現在の向きと向きたい方向とのなす角を求める
	float angle = Math::VectorAngle(weaponForward, cameraForward);
	angle *= m_CAMERA_FOLLOW_ROTATE_INTERPOLATION_RATE;
	// 回転クオータニオンを作る
	q *= Math::AxisRotation_Radian(axis, angle);
	// 回転の反映
	weaponConnection->m_LocalRotation *= q;
}

void PlayerAnimationStateBase::UpdateSphereRotate(float MoveSpeed)
{
	if (MoveSpeed != 0.0f)
	{
		float rotateSpeed = MoveSpeed * m_SPHERE_ROTATE_SPEED;
		m_Manager->GetModelParts(PlayerManager::ModelPartsId::Sphere)->m_LocalRotation *=
			Math::AxisRotation_Degree({ 1.0f, 0.0f, 0.0f }, rotateSpeed);
	}
}

void PlayerAnimationStateBase::UpdateCoverRotate(float MoveSpeed)
{
	m_AnimationBaseParam.CoverRotateTargetAngle = MoveSpeed * m_COVER_ROTATE_ANGLE_RATE;
	if (m_AnimationBaseParam.CoverRotateTargetAngle > m_COVER_ROTATE_ANGLE_MAX)
	{
		m_AnimationBaseParam.CoverRotateTargetAngle = m_COVER_ROTATE_ANGLE_MAX;
	}

	m_AnimationBaseParam.CoverRotateAngle += 
		(m_AnimationBaseParam.CoverRotateTargetAngle - m_AnimationBaseParam.CoverRotateAngle) * m_COVER_ROTATE_INTERPOLATION_RATE;

	m_Manager->GetModelParts(PlayerManager::ModelPartsId::Cover)->m_LocalRotation =
		Math::AxisRotation_Degree({ 1.0f, 0.0f, 0.0f }, -m_AnimationBaseParam.CoverRotateAngle);
}
