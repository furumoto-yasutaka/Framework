/*******************************************************************************
*
*	タイトル：	たまごエネミー引っ付きステートクラス
*	ファイル：	eggEnemyState_Stick.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "eggEnemyStateBase.h"

class EggEnemyState_Stick : public EggEnemyStateBase
{
private:


public:
	EggEnemyState_Stick(EggEnemyManager* Manager)
		: EggEnemyStateBase(Manager, EggEnemyManager::AiStateId::Stick)
	{}
	~EggEnemyState_Stick() {}

	void Update() override {}
	void LateUpdate() override {}
};
