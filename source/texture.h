/*******************************************************************************
*
*	タイトル：	テクスチャクラス
*	ファイル：	texture.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"

class Texture
{
private:
	char						m_Path[256];				// 相対パス
	ID3D11ShaderResourceView*	m_TextureResource = NULL;	// テクスチャリソース
	int							m_WidthDiv = 1;				// 横分割数
	int							m_HeightDiv = 1;			// 縦分割数

public:
	Texture(const char* path);
	~Texture();
	
	// ロード
	void Load();
	// アンロード
	void Unload();

	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	char* GetPath() { return m_Path; }

	ID3D11ShaderResourceView* GetResource() { return m_TextureResource; }

	int GetWidthDiv() { return m_WidthDiv; }
	void SetWidthDiv(int widthDiv) { m_WidthDiv = widthDiv; }

	int GetHeightDiv() { return m_HeightDiv; }
	void SetHeightDiv(int heightDiv) { m_HeightDiv = heightDiv; }
	/////////////////////////////
};
