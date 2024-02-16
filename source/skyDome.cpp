/*******************************************************************************
*
*	�^�C�g���F	�X�J�C�h�[���ÓI�N���X
*	�t�@�C���F	skyDome.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "skyDome.h"
#include "renderer.h"
#include "modelContainer.h"
#include "manager.h"
#include "camera.h"
#include "gameObject.h"

void SkyDome::Init(const char* modelName,
	string vertexShaderName, string pixelShaderName,
	float size)
{
	ModelRenderer::Init(modelName, vertexShaderName, pixelShaderName);

	// �h�[���̃T�C�Y�ݒ�(�ő�`�拗�� * 0.9)
	m_Size = 1000.0f * 0.9f;
}

void SkyDome::Update()
{
	// ���ݗL���ɂȂ��Ă���J�������擾
	Camera* camera = Manager::GetScene()->GetActiveCamera();
	if (camera)
	{
		// �J�����ɒǏ]������
		D3DXVECTOR3 pos = camera->GetAttachObject()->m_LocalPosition;
		pos.y = -3.0f;
		m_AttachObject->SetWorldPosition(pos);
	}
}

void SkyDome::Draw3d()
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
	D3DXMatrixScaling(&scale, m_Size, m_Size, m_Size);
	world = scale * m_AttachObject->GetWorldMatrix();
	Renderer::SetWorldMatrix(&world);

	// ���f���`��
	m_Model->Draw();
}

#ifdef _DEBUG
/*******************************************************************************
*	�C���X�y�N�^�[�\������
*******************************************************************************/
void SkyDome::DrawInspector()
{
	if (ImGui::CollapsingHeader("SkyDome"))
	{
		ImGui::Text("ModelName : %s", m_ModelName.c_str());

		ImGui::Text("VertexShaderName : %s", m_VertexShaderName.c_str());

		ImGui::Text("PixelShaderName : %s", m_PixelShaderName.c_str());

		ImGui::Text("PixelShaderName : %f", m_Size);
	}
}
#endif
