/*******************************************************************************
*
*	タイトル：	たまごエネミー攻撃ステートクラス
*	ファイル：	eggEnemyState_HeadAttack.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "eggEnemyStateBase.h"

class EggEnemyState_HeadAttack : public EggEnemyStateBase
{
private:

public:
	EggEnemyState_HeadAttack(EggEnemyManager* Manager);
	~EggEnemyState_HeadAttack() {}

	void Update() override;
	void LateUpdate() override;

	void Attack();	// 攻撃の当たり判定を確認する
};
