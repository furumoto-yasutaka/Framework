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

void SkyDome::Init()
{
	// ���f���擾
	m_Model = ModelContainer::GetModel_InName(m_ModelName);

	// �V�F�[�_�[�ݒ�
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	// �h�[���̃T�C�Y�ݒ�(�ő�`�拗�� * 0.9)
	m_Size = 1000.0f * 0.9f;
}

void SkyDome::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void SkyDome::Draw()
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

	//------------------------
	// ���[���h�}�g���N�X�ݒ�
	//------------------------
	D3DXVECTOR3 p;
	D3DXVECTOR3 s = D3DXVECTOR3(m_Size, m_Size, m_Size);
	// ���ݗL���ɂȂ��Ă���J�������擾
	Camera* camera = Manager::GetScene()->GetActiveCamera();
	if (camera)
	{
		p = camera->GetAttachObject()->m_LocalPosition;	// �J�����ɒǏ]������
	}
	else
	{
		p = { 0.0f, 0.0f, 0.0f };
	}
	p.y = -3.0f;

	D3DXMATRIX world, scale, trans;
	D3DXMatrixScaling(&scale, s.x, s.y, s.z);
	D3DXMatrixTranslation(&trans, p.x, p.y, p.z);
	world = scale * trans;

	Renderer::SetWorldMatrix(&world);

	// �`��
	m_Model->Draw();
}

/*******************************************************************************
*	���f���ݒ�
*******************************************************************************/
void SkyDome::SetModel(string name)
{
	m_ModelName = name;
	m_Model = ModelContainer::GetModel_InName(name);
}
