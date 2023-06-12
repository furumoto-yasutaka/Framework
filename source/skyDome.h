/*******************************************************************************
*
*	�^�C�g���F	�X�J�C�h�[���p�ÓI�N���X
*	�t�@�C���F	skyDome.h
*	�쐬�ҁF	�Ö{ �ח�
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
	float				m_Size = 0.0f;	// �h�[���̃T�C�Y

private:
	SkyDome(GameObject* attachObject)
		: ModelRenderer(attachObject)
	{}
	~SkyDome() {}
public:
	void Init(const char* modelName,
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
