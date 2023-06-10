/*******************************************************************************
*
*	�^�C�g���F	���f���`��R���|�[�l���g
*	�t�@�C���F	modelRenderer.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "modelRenderer.h"
#include "renderer.h"
#include "modelContainer.h"
#include "gameObject.h"
#include "input.h"

void ModelRenderer::Init(const char* modelName, string vertexShaderName, string pixelShaderName)
{
	m_ModelName = modelName;
	m_VertexShaderName = vertexShaderName + ".cso";
	m_PixelShaderName = pixelShaderName + ".cso";

	// ���f���擾
	m_Model = ModelContainer::GetModel_InName(m_ModelName);

	// �V�F�[�_�[�ݒ�
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, m_VertexShaderName.c_str());
	Renderer::CreatePixelShader(&m_PixelShader, m_PixelShaderName.c_str());
}

void ModelRenderer::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void ModelRenderer::Update()
{

}

void ModelRenderer::Draw3d()
{
	if (!m_Model)
	{
		return;
	}

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ���[���h�}�g���N�X�ݒ�
	D3DXMATRIX world, scale;
	float s = m_Model->GetScale();
	D3DXMatrixScaling(&scale, s, s, s);
	world = scale * m_AttachObject->GetWorldMatrix();
	Renderer::SetWorldMatrix(&world);

	// ���f���`��
	m_Model->Draw();
}

/*******************************************************************************
*	���f���ύX
*******************************************************************************/
void ModelRenderer::SetModel(string modelName)
{
	m_ModelName = modelName;

	m_Model = ModelContainer::GetModel_InName(m_ModelName);
}

#ifdef _DEBUG
/*******************************************************************************
*	�C���X�y�N�^�[�\������
*******************************************************************************/
void ModelRenderer::DrawInspector()
{
	if (ImGui::CollapsingHeader("ModelRenderer"))
	{
		ImGui::Text("ModelName : %s", m_ModelName.c_str());

		ImGui::Text("VertexShaderName : %s", m_VertexShaderName.c_str());

		ImGui::Text("PixelShaderName : %s", m_PixelShaderName.c_str());
	}
}
#endif
