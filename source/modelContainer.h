/*******************************************************************************
*
*	タイトル：	モデル管理用静的クラス
*	ファイル：	modelContainer.h
*	作成者：	古本 泰隆
* 
*******************************************************************************/
#pragma once
#include "model.h"

#include <unordered_map>

class ModelContainer
{
private:
	// ▼要素
	// 引数1	string型	モデル名称(キー)
	// 引数2	Model*型	モデル情報
	static inline unordered_map<string, Model*> m_ModelInfo;	// モデルリスト

private:
	ModelContainer() {}
public:
	static void Init();
	static void Uninit();

	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//

	// モデル取得(名称で検索)
	static Model* GetModel_InName(string name);
	/////////////////////////////
private:
	// モデル追加
	static void AddModel(string name, string path, float scale);
};
