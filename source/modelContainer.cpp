/*******************************************************************************
*
*	タイトル：	モデル管理用静的クラス
*	ファイル：	modelContainer.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "modelContainer.h"
#include "manager.h"
#include "scene.h"

#include <algorithm>

void ModelContainer::Init()
{
	// デモ用アセット
	{
		// プレイヤー
		AddModel("Coconut", "Game\\Coconut\\coconut.obj", 1.0f);
		AddModel("Player_Sphere", "Game\\Player\\PlayerSphere.obj", 1.0f);
		AddModel("Player_Cover", "Game\\Player\\PlayerCover.obj", 1.0f);
		AddModel("Player_WeaponConnection", "Game\\Player\\PlayerRodConnection.obj", 1.0f);
		AddModel("Player_Weapon", "Game\\Player\\PlayerWeapon.obj", 1.0f);

		// エネミー
		AddModel("Enemy_Egg", "Game\\Egg\\egg.obj", 0.5f);

		// プラットフォーム
		AddModel("Platform_Cube", "Game\\Platform\\Cube.obj", 1.0f);
	}

	// スカイドーム用
	AddModel("SkyDome", "Common\\SkyDome\\SkyDome.obj", 1.0f);
}

void ModelContainer::Uninit()
{
	// すべてのモデルを解放
	auto itr = m_ModelInfo.begin();
	while (itr != m_ModelInfo.end())
	{
		delete itr->second;
		itr = m_ModelInfo.erase(itr);
	}
}

/*******************************************************************************
*	モデル取得(名称で検索)
*******************************************************************************/
Model* ModelContainer::GetModel_InName(string name)
{
	auto itr = m_ModelInfo.find(name);

	if (itr != m_ModelInfo.end())
	{
		return itr->second;
	}
	else
	{
		string str = string("存在しない名称のモデルの取得が要求されました。\n") + name;
		MessageBox(GetWindow(),
			TEXT(str.c_str()),
			TEXT("modelContainer:Error"),
			MB_OK | MB_ICONERROR);
		return NULL;
	}
}

/*******************************************************************************
*	モデル追加
*******************************************************************************/
void ModelContainer::AddModel(string name, string path, float scale)
{
	string fullPath = "asset\\model\\" + path;

	// 要素がマップに存在するか確認
	if (m_ModelInfo.find(name) != m_ModelInfo.end())
	{
		string str = string("すでに存在する名称でモデルの追加が要求されました。\n") + fullPath + "\nこのモデルは読み込まれません。";
		MessageBox(GetWindow(),
			TEXT(str.c_str()),
			TEXT("modelContainer:Error"),
			MB_OK | MB_ICONERROR);
	}
	else
	{
		m_ModelInfo[name] = DBG_NEW Model(fullPath.c_str(), scale);
	}
}
