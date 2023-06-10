/*******************************************************************************
*
*	タイトル：	テクスチャ管理用静的クラス　※モデルに付属しているテクスチャは管理に含みません
*	ファイル：	textureContainer.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "textureContainer.h"
#include "manager.h"
#include "scene.h"

#include <algorithm>

void TextureContainer::Init()
{
	// デモ用アセット
	{
		// テロップ
		AddTexture("Telop_StreamingAudio", "Telop\\telop1.png", 1, 1);
		AddTexture("Telop_CollisionSplit", "Telop\\telop2.png", 1, 1);
		AddTexture("Telop_GpuParticle", "Telop\\telop3.png", 1, 1);
		AddTexture("Telop_Invoke", "Telop\\telop4.png", 1, 1);
	}

	// モデルにテクスチャ設定が無かった時用テクスチャ
	AddModelTexture("asset\\model\\Default\\modelTexDefault.png");

	// 汎用アセット
	AddTexture("Black", "Default\\black.png", 1, 1);
	AddTexture("White", "Default\\white.png", 1, 1);

	AddTexture("DevelopLogo", "Logo\\Logo.png", 1, 1);
	//AddTexture("GameLogo", "Game\\GameLogo.png", 1, 1);

	//AddTexture("Title_PromptText", "Title\\PromptText.png", 1, 1);
}

void TextureContainer::Uninit()
{
	// すべてのテクスチャを解放
	auto itr = m_TextureInfo.begin();
	while (itr != m_TextureInfo.end())
	{
		delete itr->second;
		itr = m_TextureInfo.erase(itr);
	}
}

/*******************************************************************************
*	モデルテクスチャ追加
*******************************************************************************/
Texture* TextureContainer::AddModelTexture(string path)
{
	Texture* tex = GetTexture_InPath(path);

	if (tex == NULL)
	{
		m_ModelTextureCount++;
		string name = "ModelTex_" + to_string(m_ModelTextureCount);

		tex = DBG_NEW Texture(path.c_str());
		tex->SetWidthDiv(1);
		tex->SetHeightDiv(1);
		m_TextureInfo[name] = tex;
	}

	return tex;
}

/*******************************************************************************
*	テクスチャ取得(名称で検索)
*******************************************************************************/
Texture* TextureContainer::GetTexture_InName(string name)
{
	auto itr = m_TextureInfo.find(name);

	if (itr != m_TextureInfo.end())
	{
		return itr->second;
	}
	else
	{
		string str = string("存在しない名称のテクスチャの取得が要求されました。\n") + name;
		MessageBox(GetWindow(),
			TEXT(str.c_str()),
			TEXT("textureContainer:Error"),
			MB_OK | MB_ICONERROR);
		return NULL;
	}
}

/*******************************************************************************
*	テクスチャ取得(パスで検索)
*******************************************************************************/
Texture* TextureContainer::GetTexture_InPath(string path)
{
	auto itr = find_if(m_TextureInfo.begin(), m_TextureInfo.end(),
		[&](pair<string, Texture*> pair) { return strcmp(pair.second->GetPath(), path.c_str()) == 0; });

	if (itr != m_TextureInfo.end())
	{
		return itr->second;
	}
	else
	{
		return NULL;
	}
}

/*******************************************************************************
*	テクスチャ追加
*******************************************************************************/
void TextureContainer::AddTexture(string name, string path, int widthDiv, int heightDiv)
{
	string fullPath = "asset\\texture\\" + path;

	// 要素がマップに存在するか確認
	if (m_TextureInfo.count(string(name)))
	{
		string str = string("すでに存在する名称でテクスチャの追加が要求されました。\n") + fullPath + "\nこのテクスチャは読み込まれません。";
		MessageBox(GetWindow(),
			TEXT(str.c_str()),
			TEXT("textureContainer:Warning"),
			MB_OK | MB_ICONERROR);
	}
	else
	{
		Texture* tex = DBG_NEW Texture(fullPath.c_str());
		tex->SetWidthDiv(widthDiv);
		tex->SetHeightDiv(heightDiv);
		m_TextureInfo[name] = tex;
	}
}
