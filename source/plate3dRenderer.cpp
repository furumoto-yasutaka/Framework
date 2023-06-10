/*******************************************************************************
*
*	�^�C�g���F	3D�|���S���`��R���|�[�l���g
*	�t�@�C���F	plate3dRenderer.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "plate3dRenderer.h"
#include "textureContainer.h"
#include "gameobject.h"
#include "textureAnimation.h"
#include "camera.h"

#ifdef _DEBUG
#include "debugManager.h"
#endif

void Plate3DRenderer::Init(const char* textureName, D3DXVECTOR2 size,
	AnchorPosition anchorPos, bool isBillBoard, bool isAlphaToCoverage)
{
	// ������
	m_TextureName = textureName;
	m_Size = size;
	m_AnchorPosition = anchorPos;
	m_IsBillBoard = isBillBoard;
	m_IsAlphaToCoverage = isAlphaToCoverage;
	m_TexCoordBegin = D3DXVECTOR2(0.0f, 0.0f);
	m_TexColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Animation = NULL;

	// �e�N�X�`���擾
	m_Texture = TextureContainer::GetTexture_InName(m_TextureName);
	m_TexCoordDistance.x = 1.0f / m_Texture->GetWidthDiv();
	m_TexCoordDistance.y = 1.0f / m_Texture->GetHeightDiv();

	//-------------------
	// �o�b�t�@�ݒ�
	//-------------------
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	VERTEX_3D vertex[4];
	GetVertex(vertex);
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	// �o�b�t�@����
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �V�F�[�_�[�ݒ�
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}

void Plate3DRenderer::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Plate3DRenderer::Draw3d()
{
	if (!m_Texture)
	{
		return;
	}

	// �e�N�X�`�����X�V
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);
	GetVertex((VERTEX_3D*)msr.pData);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	// �A���t�@�g�D�J�o���b�W�ݒ�
	Renderer::SetAlphaToCaverage(m_IsAlphaToCoverage);

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ���[���h�}�g���N�X�ݒ�
	if (m_IsBillBoard)
	{// �r���{�[�h�L��
		Camera* cam = Manager::GetScene()->GetActiveCamera();
		D3DXMATRIX view = cam->GetViewMatrix();
		D3DXMATRIX invView;
		D3DXMatrixInverse(&invView, NULL, &view);
		invView._41 = 0.0f;
		invView._42 = 0.0f;
		invView._43 = 0.0f;

		D3DXMATRIX world, scale, trans;
		D3DXVECTOR3 scl = m_AttachObject->GetWorldScale();
		D3DXVECTOR3 pos = m_AttachObject->GetWorldPosition();
		D3DXMatrixScaling(&scale, scl.x, scl.y, scl.z);
		D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);

		world = scale * invView * trans;
		Renderer::SetWorldMatrix(&world);
	}
	else
	{// �r���{�[�h����
		D3DXMATRIX m = m_AttachObject->GetWorldMatrix();
		Renderer::SetWorldMatrix(&m);
	}

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	ID3D11ShaderResourceView* resource = m_Texture->GetResource();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &resource);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}

/*******************************************************************************
*	�e�N�X�`���A�j���[�V�����Ɛڑ�
*******************************************************************************/
void Plate3DRenderer::Connect(TextureAnimation* animation,
	D3DXVECTOR2*& texCoordBegin, D3DXVECTOR2*& texCoordDistance,
	int& widthDiv, int& heightDiv)
{
	// �R���|�[�l���g��o�^
	m_Animation = animation;

	// �ϐ��̒l�����L
	texCoordBegin = &m_TexCoordBegin;
	texCoordDistance = &m_TexCoordDistance;
	widthDiv = m_Texture->GetWidthDiv();
	heightDiv = m_Texture->GetHeightDiv();
}

/*******************************************************************************
*	�e�N�X�`���A�j���[�V�����Ƃ̐ڑ�������
*******************************************************************************/
void Plate3DRenderer::DisConnect()
{
	if (!m_Animation) { return; }
	m_Animation->SetDestroy();
	m_Animation = nullptr;
}

/*******************************************************************************
*	���g�̍폜��\��
*******************************************************************************/
void Plate3DRenderer::SetDestroy()
{
	if (m_Animation) { m_Animation->SetDestroy(); }
	m_IsDestroy = true;
}

/*******************************************************************************
*	�e�N�X�`���ύX
*******************************************************************************/
void Plate3DRenderer::SetTexture(string name)
{
	m_TextureName = name;
	m_Texture = TextureContainer::GetTexture_InName(m_TextureName);

	if (m_Texture)
	{
		// �e�N�X�`�����W���Čv�Z
		m_TexCoordDistance.x = 1.0f / m_Texture->GetWidthDiv();
		m_TexCoordDistance.y = 1.0f / m_Texture->GetHeightDiv();
	}
}

/*******************************************************************************
*	���_�����擾
*******************************************************************************/
void Plate3DRenderer::GetVertex(VERTEX_3D* vertex)
{
	D3DXVECTOR3 s = m_AttachObject->GetWorldScale();
	D3DXVECTOR2 halfScale;

	// �s�{�b�g�ʒu�ɉ������ݒ�Ő���
	switch (m_AnchorPosition)
	{
	case AnchorPosition::Left_Top:
		CreateVertex(vertex, D3DXVECTOR2(0.0f, 0.0f));
		break;
	case AnchorPosition::Center_Middle:
		halfScale = m_Size * 0.5f;
		CreateVertex(vertex, -halfScale);
		break;
	default: return;
	}
}

/*******************************************************************************
*	���_���𐶐�
*******************************************************************************/
void Plate3DRenderer::CreateVertex(VERTEX_3D* vertex, D3DXVECTOR2 offset)
{
	D3DXVECTOR3 n;
	// �@���ݒ�
	if (m_IsBillBoard)
	{
		// �r���{�[�h���L���̏ꍇ�̓J�����̕����������悤�ɂ���
		n = -Manager::GetScene()->GetActiveCamera()->GetAttachObject()->GetForward();
	}
	else { n = m_AttachObject->GetForward(); }

	vertex[0].Position = D3DXVECTOR3(offset.x, offset.y + m_Size.y, 0.0f);
	vertex[0].Normal = n;
	vertex[0].Diffuse = m_TexColor;
	vertex[0].TexCoord = m_TexCoordBegin;

	vertex[1].Position = D3DXVECTOR3(offset.x + m_Size.x, offset.y + m_Size.y, 0.0f);
	vertex[1].Normal = n;
	vertex[1].Diffuse = m_TexColor;
	vertex[1].TexCoord = D3DXVECTOR2(m_TexCoordBegin.x + m_TexCoordDistance.x,
		m_TexCoordBegin.y);

	vertex[2].Position = D3DXVECTOR3(offset.x, offset.y, 0.0f);
	vertex[2].Normal = n;
	vertex[2].Diffuse = m_TexColor;
	vertex[2].TexCoord = D3DXVECTOR2(m_TexCoordBegin.x,
		m_TexCoordBegin.y + m_TexCoordDistance.y);

	vertex[3].Position = D3DXVECTOR3(offset.x + m_Size.x, offset.y, 0.0f);
	vertex[3].Normal = n;
	vertex[3].Diffuse = m_TexColor;
	vertex[3].TexCoord = m_TexCoordBegin + m_TexCoordDistance;
}

#ifdef _DEBUG
/*******************************************************************************
*	�C���X�y�N�^�[�\������
*******************************************************************************/
void Plate3DRenderer::DrawInspector()
{
	if (ImGui::CollapsingHeader("Plate3DRenderer"))
	{
		int tempInt;
		float tempVec2[2];
		float tempCol[4];

		ImGui::Text("TextureName : %s", m_TextureName.c_str());

		tempInt = (int)m_AnchorPosition;
		if (DebugManager::ComboBox("AnchorPosition", tempInt, m_ANCHOR_POSITION_STR))
		{
			m_AnchorPosition = (Plate3DRenderer::AnchorPosition)tempInt;
		}

		ImGui::Checkbox("IsBillBoard", &m_IsBillBoard);

		ImGui::Checkbox("m_IsAlphaToCoverage", &m_IsAlphaToCoverage);

		tempVec2[0] = m_Size.x;
		tempVec2[1] = m_Size.y;
		if (ImGui::InputFloat2("Size", tempVec2))
		{
			m_Size = { tempVec2[0], tempVec2[1] };
		}

		tempVec2[0] = m_TexCoordBegin.x;
		tempVec2[1] = m_TexCoordBegin.y;
		if (ImGui::InputFloat2("TexCoordBegin", tempVec2))
		{
			m_TexCoordBegin = { tempVec2[0], tempVec2[1] };
		}

		tempVec2[0] = m_TexCoordDistance.x;
		tempVec2[1] = m_TexCoordDistance.y;
		if (ImGui::InputFloat2("TexCoordDistance", tempVec2))
		{
			m_TexCoordDistance = { tempVec2[0], tempVec2[1] };
		}

		tempCol[0] = m_TexColor.r;
		tempCol[1] = m_TexColor.g;
		tempCol[2] = m_TexColor.b;
		tempCol[3] = m_TexColor.a;
		if (ImGui::ColorEdit4("TexColor", tempCol))
		{
			m_TexColor = { tempCol[0], tempCol[1], tempCol[2], tempCol[3] };
		}

		ImGui::Text("TextureAnimation : %s", (!m_Animation ? "Unconnect" : "Connect"));
	}
}
#endif
