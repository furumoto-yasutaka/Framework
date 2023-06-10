/*******************************************************************************
*
*	タイトル：	テクスチャ管理用静的クラス　※モデルに付属しているテクスチャは管理に含みません
*	ファイル：	textureContainer.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "texture.h"

#include <unordered_map>
#include <memory>

class TextureContainer
{
private:
	// ▼要素
	// 引数1	string型	テクスチャ名称(キー)
	// 引数2	Texture*型	テクスチャ情報
	static inline unordered_map<string, Texture*> m_TextureInfo;	// テクスチャリスト
	
	static inline int m_ModelTextureCount = 0;
	static inline string m_ModelTexInitial = "ModelTex_";

private:
	TextureContainer() {}
public:
	static void Init();
	static void Uninit();

	// モデルテクスチャ追加
	static Texture* AddModelTexture(string path);

	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//

	// テクスチャ取得(名称で検索)
	static Texture* GetTexture_InName(string name);

	// テクスチャ取得(パスで検索)
	static Texture* GetTexture_InPath(string path);

	static inline string GetModelTexInitial() { return m_ModelTexInitial; }
	/////////////////////////////

private:
	// テクスチャ追加
	static void AddTexture(string name, string path,
		int widthDiv, int heightDiv);
};
