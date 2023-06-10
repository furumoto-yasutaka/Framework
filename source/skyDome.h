/*******************************************************************************
*
*	タイトル：	スカイドーム用静的クラス
*	ファイル：	skyDome.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"

#include <string>

class Model;

class SkyDome
{
private:
	static inline string				m_ModelName = "SkyDome";	// モデル名称(コンテナ上の名前)
	static inline Model*				m_Model = NULL;			// モデルの詳細情報
	static inline ID3D11VertexShader*	m_VertexShader = NULL;		// 頂点シェーダー
	static inline ID3D11PixelShader*	m_PixelShader = NULL;		// ピクセルシェーダー
	static inline ID3D11InputLayout*	m_VertexLayout = NULL;		// 入力レイアウト
	static inline float					m_Size;						// サイズ

private:
	~SkyDome() {}
public:
	static void Init();
	static void Uninit();
	static void Draw();

	static void SetModel(string name);
};
