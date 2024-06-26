/*******************************************************************************
*
*	タイトル：	モデル描画コンポーネント
*	ファイル：	modelRenderer.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "draw3dComponent.h"

#include <string>

class Model;

class ModelRenderer : public Draw3DComponent
{
private:
	string				m_ModelName;			// モデルの名称(コンテナ上の名前)
	string				m_VertexShaderName;		// 頂点シェーダー名
	string				m_PixelShaderName;		// ピクセルシェーダー名
	Model*				m_Model = NULL;			// モデルの詳細情報
	ID3D11VertexShader* m_VertexShader = NULL;	// 頂点シェーダー
	ID3D11PixelShader*	m_PixelShader = NULL;	// ピクセルシェーダー
	ID3D11InputLayout*	m_VertexLayout = NULL;	// 入力レイアウト

public:
	ModelRenderer(GameObject* attachObject)
		: Draw3DComponent(attachObject)
	{}
	~ModelRenderer() {}

	void Init(const char* modelName, string vertexShaderName = "vertexLightingVS", string pixelShaderName = "vertexLightingPS");
	void Uninit() override;
	void Update() override;
	void Draw3d() override;

	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	string GetModelName() { return m_ModelName; }

	void SetModel(string modelName);
	/////////////////////////////

#ifdef _DEBUG
public:
	void DrawInspector() override;
#endif
};
