/*******************************************************************************
*
*	タイトル：	カプセル型3Dコリジョンコンポーネント
*	ファイル：	capsuleCollider3d.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "capsuleCollider3d.h"
#include "gameObject.h"
#include "aabbCollider3d.h"
#include "obbCollider3D.h"
#include "sphereCollider3d.h"

#ifdef _DEBUG
#include "debugManager.h"
#endif

void CapsuleCollider3D::Init(D3DXVECTOR3 offset, float radius, float height,
	DirectionPattern heightDirection, bool isTrigger, bool isStatic)
{
	Collider3D::Init(offset, isTrigger, isStatic);

	m_LocalRadius = radius;
	m_LocalHeight = height;
	m_HeightDirection = heightDirection;
}

void CapsuleCollider3D::Uninit()
{
	Collider3D::Uninit();
}

void CapsuleCollider3D::CollisionAABB(AABBCollider3D* collider)
{

}

void CapsuleCollider3D::CollisionOBB(OBBCollider3D* collider)
{

}

void CapsuleCollider3D::CollisionSphere(SphereCollider3D* collider)
{

}

void CapsuleCollider3D::CollisionCapsule(CapsuleCollider3D* collider)
{

}

D3DXVECTOR3 CapsuleCollider3D::GetMortonMinPos()
{
	float r = GetWorldRadius();
	D3DXVECTOR3 vec3Radius = { r, r, r };
	return GetColliderWorldPos() - vec3Radius;
}

D3DXVECTOR3 CapsuleCollider3D::GetMortonMaxPos()
{
	float r = GetWorldRadius();
	D3DXVECTOR3 vec3Radius = { r, r, r };
	return GetColliderWorldPos() + vec3Radius;
}

float CapsuleCollider3D::GetWorldRadius()
{
	D3DXVECTOR3 scale = m_AttachObject->GetWorldScale();
	float max = 0.0f;

	switch (m_HeightDirection)
	{
		using enum DirectionPattern;
	case X:
		max = scale.y;
		if (scale.z > max) { max = scale.z; }
		break;
	case Y:
		max = scale.x;
		if (scale.z > max) { max = scale.z; }
		break;
	case Z:
		max = scale.x;
		if (scale.y > max) { max = scale.z; }
		break;
	}

	return m_LocalRadius * max;
}

float CapsuleCollider3D::GetWorldHeight()
{
	D3DXVECTOR3 scale = m_AttachObject->GetWorldScale();
	float h = 0.0f;

	switch (m_HeightDirection)
	{
		using enum DirectionPattern;
	case X:
		h = m_LocalHeight * scale.x;
		break;
	case Y:
		h = m_LocalHeight * scale.y;
		break;
	case Z:
		h = m_LocalHeight * scale.z;
		break;
	}

	return h;
}

#ifdef _DEBUG
/*******************************************************************************
*	インスペクター表示処理
*******************************************************************************/
void CapsuleCollider3D::DrawInspector()
{
	if (ImGui::CollapsingHeader("CapsuleCollider3D"))
	{
		float tempFloat;

		ImGui::InputFloat("LocalRadius", &m_LocalRadius);

		tempFloat = GetWorldRadius();
		ImGui::InputFloat("WorldRadius", &tempFloat, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);

		ImGui::InputFloat("LocalHeight", &m_LocalHeight);

		tempFloat = GetWorldHeight();
		ImGui::InputFloat("WorldHeight", &tempFloat, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);

		Collider3D::DrawInspector();
	}
}

void CapsuleCollider3D::InitDebug()
{
	//-------------------
	// バッファ設定
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

	// バッファ生成
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	delete[] vertex;

	// シェーダー設定
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "debugLineVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "debugLinePS.cso");
}

void CapsuleCollider3D::UninitDebug()
{
	m_VertexBuffer->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
	m_VertexLayout->Release();
}

void CapsuleCollider3D::DrawDebug3d()
{
	if (!DebugManager::m_IsCollisionFrame) return;

	// 頂点情報書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);
	CreateVertex((VERTEX_3D*)msr.pData);
	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ワールドマトリクス設定
	D3DXMATRIX m = m_AttachObject->GetWorldMatrix();
	Renderer::SetWorldMatrix(&m);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	Renderer::SetMaterial(material);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(m_VertexNum, 0);
}

void CapsuleCollider3D::CreateVertex(VERTEX_3D* vertex)
{
	float straightHalfLen = GetWorldHeight() - GetWorldRadius() * 2;
	if (straightHalfLen < 0.0f) { straightHalfLen = 0.0f; }

	switch (m_HeightDirection)
	{
		using enum DirectionPattern;
	case X:
		CreateVertexAxisX(vertex, straightHalfLen);
		break;
	case Y:
		CreateVertexAxisY(vertex, straightHalfLen);
		break;
	case Z:
		CreateVertexAxisZ(vertex, straightHalfLen);
		break;
	}
}

void CapsuleCollider3D::CreateVertexAxisX(VERTEX_3D* vertex, float straightHalfLen)
{

}

void CapsuleCollider3D::CreateVertexAxisY(VERTEX_3D* vertex, float straightHalfLen)
{
	float radius = GetWorldRadius();
	D3DXVECTOR3 forward = { 0.0f, 0.0f, 1.0f };
	D3DXVECTOR3 up = { 0.0f, 1.0f, 0.0f };
	D3DXVECTOR3 right = { 1.0f, 0.0f, 0.0f };

	D3DXVECTOR3 n = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR d = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	D3DXVECTOR2 c = D3DXVECTOR2(0.0f, 0.0f);

	// 1つ目のサークルの4分の1(forward up)
	float piPerVertex = Math::m_PI<float> / (m_HalfCircleVertexNum);
	float rot = 0.0f;
	int num = m_QuarterCircleVertexNum;
	int i;
	for (i = 0; i < num; i++)
	{
		vertex[i].Position = m_Offset + D3DXVECTOR3(0.0f, straightHalfLen, 0.0f);
		vertex[i].Position += (forward * sinf(rot) * radius) + (up * cosf(rot) * radius);
		vertex[i].Normal = n;
		vertex[i].Diffuse = d;
		vertex[i].TexCoord = c;

		rot += piPerVertex;
	}
	// 2つ目のサークル(right forward)
	rot = 0.0f;
	num += m_HalfCircleVertexNum * 2 + 1;
	for (; i < num; i++)
	{
		vertex[i].Position = m_Offset + D3DXVECTOR3(0.0f, straightHalfLen, 0.0f);
		vertex[i].Position += (right * sinf(rot) * radius) + (forward * cosf(rot) * radius);
		vertex[i].Normal = n;
		vertex[i].Diffuse = d;
		vertex[i].TexCoord = c;

		rot += piPerVertex;
	}
	// 3つ目のサークル(right forward)
	rot = 0.0f;
	num += m_HalfCircleVertexNum * 2;
	for (; i < num; i++)
	{
		vertex[i].Position = m_Offset - D3DXVECTOR3(0.0f, straightHalfLen, 0.0f);
		vertex[i].Position += (right * sinf(rot) * radius) + (forward * cosf(rot) * radius);
		vertex[i].Normal = n;
		vertex[i].Diffuse = d;
		vertex[i].TexCoord = c;

		rot += piPerVertex;
	}
	// 1つ目のサークルの4分の2(forward up)
	rot = Math::m_PI<float> * 0.5f;
	num += m_HalfCircleVertexNum + 1;
	for (; i < num; i++)
	{
		vertex[i].Position = m_Offset - D3DXVECTOR3(0.0f, straightHalfLen, 0.0f);
		vertex[i].Position += (forward * sinf(rot) * radius) + (up * cosf(rot) * radius);
		vertex[i].Normal = n;
		vertex[i].Diffuse = d;
		vertex[i].TexCoord = c;

		rot += piPerVertex;
	}
	// 1つ目のサークルの4分の1(forward up)
	rot = Math::m_PI<float> * 1.5f;
	num += m_QuarterCircleVertexNum;
	for (; i < num; i++)
	{
		vertex[i].Position = m_Offset + D3DXVECTOR3(0.0f, straightHalfLen, 0.0f);
		vertex[i].Position += (forward * sinf(rot) * radius) + (up * cosf(rot) * radius);
		vertex[i].Normal = n;
		vertex[i].Diffuse = d;
		vertex[i].TexCoord = c;

		rot += piPerVertex;
	}
	// 4つ目のサークル4分の1(right up)
	rot = 0.0f;
	num += m_QuarterCircleVertexNum + 1;
	for (; i < num; i++)
	{
		vertex[i].Position = m_Offset + D3DXVECTOR3(0.0f, straightHalfLen, 0.0f);
		vertex[i].Position += (right * sinf(rot) * radius) + (up * cosf(rot) * radius);
		vertex[i].Normal = n;
		vertex[i].Diffuse = d;
		vertex[i].TexCoord = c;

		rot += piPerVertex;
	}
	// 4つ目のサークル4分の2(right up)
	rot = Math::m_PI<float> * 0.5f;
	num += m_HalfCircleVertexNum + 1;
	for (; i < num; i++)
	{
		vertex[i].Position = m_Offset - D3DXVECTOR3(0.0f, straightHalfLen, 0.0f);
		vertex[i].Position += (right * sinf(rot) * radius) + (up * cosf(rot) * radius);
		vertex[i].Normal = n;
		vertex[i].Diffuse = d;
		vertex[i].TexCoord = c;

		rot += piPerVertex;
	}
	// 4つ目のサークル4分の1(right up)
	rot = Math::m_PI<float> * 1.5f;
	num += m_QuarterCircleVertexNum + 1;
	for (; i < num; i++)
	{
		vertex[i].Position = m_Offset + D3DXVECTOR3(0.0f, straightHalfLen, 0.0f);
		vertex[i].Position += (right * sinf(rot) * radius) + (up * cosf(rot) * radius);
		vertex[i].Normal = n;
		vertex[i].Diffuse = d;
		vertex[i].TexCoord = c;

		rot += piPerVertex;
	}
}

void CapsuleCollider3D::CreateVertexAxisZ(VERTEX_3D* vertex, float straightHalfLen)
{

}
#endif
