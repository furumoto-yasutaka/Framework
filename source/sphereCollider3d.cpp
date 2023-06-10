/*******************************************************************************
*
*	�^�C�g���F	���^3D�R���W�����R���|�[�l���g
*	�t�@�C���F	sphereCollider3d.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "sphereCollider3d.h"
#include "gameObject.h"
#include "aabbCollider3d.h"
#include "obbCollider3D.h"
#include "capsuleCollider3d.h"

#ifdef _DEBUG
#include "debugManager.h"
#endif

void SphereCollider3D::Init(D3DXVECTOR3 offset, float radius,
	bool isTrigger, bool isStatic)
{
	Collider3D::Init(offset, isTrigger, isStatic);

	m_LocalRadius = radius;
}

void SphereCollider3D::Uninit()
{
	Collider3D::Uninit();
}

void SphereCollider3D::CollisionAABB(AABBCollider3D* collider)
{

}

void SphereCollider3D::CollisionOBB(OBBCollider3D* collider)
{

}

void SphereCollider3D::CollisionSphere(SphereCollider3D* collider)
{

}

void SphereCollider3D::CollisionCapsule(CapsuleCollider3D* collider)
{

}

D3DXVECTOR3 SphereCollider3D::GetMortonMinPos()
{
	float r = GetWorldRadius();
	D3DXVECTOR3 vec3Radius = { r, r, r };
	return GetColliderWorldPos() - vec3Radius;
}

D3DXVECTOR3 SphereCollider3D::GetMortonMaxPos()
{
	float r = GetWorldRadius();
	D3DXVECTOR3 vec3Radius = { r, r, r };
	return GetColliderWorldPos() + vec3Radius;
}

float SphereCollider3D::GetWorldRadius()
{
	D3DXVECTOR3 scale = m_AttachObject->GetWorldScale();
	float max = scale.x;
	if (scale.y > max) { max = scale.y; }
	if (scale.z > max) { max = scale.z; }

	return m_LocalRadius * max;
}

#ifdef _DEBUG
/*******************************************************************************
*	�C���X�y�N�^�[�\������
*******************************************************************************/
void SphereCollider3D::DrawInspector()
{
	if (ImGui::CollapsingHeader("SphereCollider3D"))
	{
		float tempFloat;

		ImGui::InputFloat("LocalRadius", &m_LocalRadius);

		tempFloat = GetWorldRadius();
		ImGui::InputFloat("WorldRadius", &tempFloat, 0.0f, 0.0f, "%.3f",
			ImGuiInputTextFlags_ReadOnly);

		Collider3D::DrawInspector();
	}
}

void SphereCollider3D::InitDebug()
{
	//-------------------
	// �o�b�t�@�ݒ�
	//-------------------
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VERTEX_3D) * m_VertexNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	VERTEX_3D* vertex = DBG_NEW VERTEX_3D[m_VertexNum];
	CreateVertex(vertex);

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	// �o�b�t�@����
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	delete[] vertex;

	// �V�F�[�_�[�ݒ�
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "debugLineVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "debugLinePS.cso");
}

void SphereCollider3D::UninitDebug()
{
	m_VertexBuffer->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_VertexLayout->Release();
}

void SphereCollider3D::DrawDebug3d()
{
	if (!DebugManager::m_IsCollisionFrame) return;

	// ���_��񏑂�����
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);
	CreateVertex((VERTEX_3D*)msr.pData);
	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ���[���h�}�g���N�X�ݒ�
	D3DXMATRIX m = m_AttachObject->GetWorldMatrix();
	Renderer::SetWorldMatrix(&m);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	Renderer::SetMaterial(material);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(m_VertexNum, 0);
}

void SphereCollider3D::CreateVertex(VERTEX_3D* vertex)
{
	float radius = GetWorldRadius();
	D3DXVECTOR3 forward = { 0.0f, 0.0f, 1.0f };
	D3DXVECTOR3 up = { 0.0f, 1.0f, 0.0f };
	D3DXVECTOR3 right = { 1.0f, 0.0f, 0.0f };

	D3DXVECTOR3 n = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR d = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	D3DXVECTOR2 c = D3DXVECTOR2(0.0f, 0.0f);

	// 1�ڂ̃T�[�N����4����1(forward up)
	float perPi = (Math::m_PI<float> * 2) / m_CircleVertexNum;
	float rot = 0.0f;
	int num = m_QuarterCircleVertexNum;
	int i;
	for (i = 0; i < num; i++)
	{
		vertex[i].Position = m_Offset + (forward * sinf(rot) * radius) + (up * cosf(rot) * radius);
		vertex[i].Normal = n;
		vertex[i].Diffuse = d;
		vertex[i].TexCoord = c;

		rot += perPi;
	}
	// 2�ڂ̃T�[�N��(right forward)
	rot = 0.0f;
	num += m_CircleVertexNum;
	for (; i < num; i++)
	{
		vertex[i].Position = m_Offset + (right * sinf(rot) * radius) + (forward * cosf(rot) * radius);
		vertex[i].Normal = n;
		vertex[i].Diffuse = d;
		vertex[i].TexCoord = c;

		rot += perPi;
	}
	// 1�ڂ̃T�[�N����4����3(forward up)
	rot = Math::m_PI<float> * 0.5f;
	num += m_CircleVertexNum - m_QuarterCircleVertexNum;
	for (; i < num; i++)
	{
		vertex[i].Position = m_Offset + (forward * sinf(rot) * radius) + (up * cosf(rot) * radius);
		vertex[i].Normal = n;
		vertex[i].Diffuse = d;
		vertex[i].TexCoord = c;

		rot += perPi;
	}
	// 3�ڂ̃T�[�N��(right up)
	rot = 0.0f;
	num += m_CircleVertexNum;
	for (; i < num; i++)
	{
		vertex[i].Position = m_Offset + (right * sinf(rot) * radius) + (up * cosf(rot) * radius);
		vertex[i].Normal = n;
		vertex[i].Diffuse = d;
		vertex[i].TexCoord = c;

		rot += perPi;
	}
	
	// �Ō�̒��_
	rot = 0.0f;
	vertex[m_VertexNum - 1].Position = m_Offset + (right * sinf(rot) * radius) + (up * cosf(rot) * radius);
	vertex[m_VertexNum - 1].Normal = n;
	vertex[m_VertexNum - 1].Diffuse = d;
	vertex[m_VertexNum - 1].TexCoord = c;
}
#endif
