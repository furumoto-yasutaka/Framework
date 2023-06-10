/*******************************************************************************
*
*	�^�C�g���F	���f���`��R���|�[�l���g
*	�t�@�C���F	modelRenderer.h
*	�쐬�ҁF	�Ö{ �ח�
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
	string				m_ModelName;			// ���f���̖���(�R���e�i��̖��O)
	string				m_VertexShaderName;		// ���_�V�F�[�_�[��
	string				m_PixelShaderName;		// �s�N�Z���V�F�[�_�[��
	Model*				m_Model = NULL;			// ���f���̏ڍ׏��
	ID3D11VertexShader* m_VertexShader = NULL;	// ���_�V�F�[�_�[
	ID3D11PixelShader*	m_PixelShader = NULL;	// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout*	m_VertexLayout = NULL;	// ���̓��C�A�E�g

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
	//�@�����@�A�N�Z�T�@�����@//
	string GetModelName() { return m_ModelName; }

	void SetModel(string modelName);
	/////////////////////////////

#ifdef _DEBUG
public:
	void DrawInspector() override;
#endif
};
