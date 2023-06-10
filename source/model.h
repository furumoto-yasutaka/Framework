/*******************************************************************************
*
*	タイトル：	モデルクラス
*	ファイル：	model.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "renderer.h"
#include "texture.h"

// マテリアル構造体
struct MODEL_MATERIAL
{
	char			Name[256];
	MATERIAL		Material;
	Texture*		Tex;
};

// 描画サブセット構造体
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};

// モデル構造体
struct MODEL
{
	VERTEX_3D*		VertexArray;
	unsigned int	VertexNum;

	unsigned int*	IndexArray;
	unsigned int	IndexNum;

	SUBSET*			SubsetArray;
	unsigned int	SubsetNum;
};

class Model
{
private:
	char			m_Path[256];			// 相対パス
	float			m_Scale = 1.0f;			// スケール

	ID3D11Buffer*	m_VertexBuffer = NULL;	// 頂点バッファ
	ID3D11Buffer*	m_IndexBuffer = NULL;	// インデックスバッファ
	SUBSET*			m_SubsetArray = NULL;	// サブセット
	unsigned int	m_SubsetNum = NULL;		// サブセット数

public:
	Model(const char* Path, float Scale);
	~Model();

	void Draw();

	// ロード
	void Load();
	// アンロード
	void Unload();
	
	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	char* GetPath() { return m_Path; }

	float GetScale() { return m_Scale; }
	/////////////////////////////
private:
	// objファイル読み込み
	void LoadObj(const char* fileName, MODEL* model);
	// mtlファイル読み込み
	void LoadMaterial(const char* fileName, MODEL_MATERIAL** materialArray, unsigned int* materialNum);
};
