/*******************************************************************************
*
*	タイトル：	スカイドーム用静的クラス
*	ファイル：	skyDome.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "modelRenderer.h"

#include <string>

class Model;

class SkyDome : public ModelRenderer
{
private:
	float				m_Size = 0.0f;	// ドームのサイズ

public:
	SkyDome(GameObject* attachObject)
		: ModelRenderer(attachObject)
	{}
	~SkyDome() {}

	void Init(const char* modelName = "SkyDome",
		string vertexShaderName = "vertexLightingVS",
		string pixelShaderName = "vertexLightingPS",
		float size = 1000.0f * 0.9f);
	void Update() override;
	void Draw3d() override;

#ifdef _DEBUG
public:
	void DrawInspector() override;
#endif
};
