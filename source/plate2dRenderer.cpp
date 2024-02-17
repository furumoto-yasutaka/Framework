/*******************************************************************************
*
*	タイトル：	2D板ポリゴン描画コンポーネント
*	ファイル：	plate2dRenderer.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "Plate2dRenderer.h"
#include "textureContainer.h"
#include "gameObject.h"
#include "textureAnimation.h"

#ifdef _DEBUG
#include "debugManager.h"
#endif

void Plate2DRenderer::Init(const char* textureName,
	D3DXVECTOR2 size, AnchorPosition anchorPos)
{
	// 初期化
	m_TextureName = textureName;
	m_Size = size;
	m_AnchorPosition = anchorPos;
	m_TexCoordBegin = D3DXVECTOR2(0.0f, 0.0f);
	m_TexColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Animation = NULL;

	// テクスチャ取得
	m_Texture = TextureContainer::GetTexture_InName(m_TextureName);
	m_TexCoordDistance.x = 1.0f / m_Texture->GetWidthDiv();
	m_TexCoordDistance.y = 1.0f / m_Texture->GetHeightDiv();

	CreateBuffer();
	CreateShader();
}
void Plate2DRenderer::Uninit()
{
	DisConnect();

	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}
void Plate2DRenderer::Draw2d()
{
	if (!m_Texture) { return; }

	// テクスチャ情報更新
	D3DXVECTOR3 p = m_AttachObject->m_LocalPosition;
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);
	GetVertex((VERTEX_3D*)msr.pData);
	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// テクスチャ設定
	ID3D11ShaderResourceView* resource = m_Texture->GetResource();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &resource);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

/*******************************************************************************
*	バッファ設定
*******************************************************************************/
void Plate2DRenderer::CreateBuffer()
{
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

	// バッファ生成
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
}

/*******************************************************************************
*	シェーダー設定
*******************************************************************************/
void Plate2DRenderer::CreateShader()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}

/*******************************************************************************
*	テクスチャアニメーションと接続
*******************************************************************************/
void Plate2DRenderer::Connect(TextureAnimation* animation,
	D3DXVECTOR2*& texCoordBegin, D3DXVECTOR2*& texCoordDistance,
	int& widthDiv, int& heightDiv)
{
	// コンポーネントを登録
	m_Animation = animation;

	// 変数の値を共有
	texCoordBegin = &m_TexCoordBegin;
	texCoordDistance = &m_TexCoordDistance;
	widthDiv = m_Texture->GetWidthDiv();
	heightDiv = m_Texture->GetHeightDiv();
}

/*******************************************************************************
*	テクスチャアニメーションとの接続を解除
*******************************************************************************/
void Plate2DRenderer::DisConnect()
{
	if (!m_Animation) { return; }
	m_Animation->SetDestroy();
	m_Animation = nullptr;
}

/*******************************************************************************
*	自身の削除を予約
*******************************************************************************/
void Plate2DRenderer::SetDestroy()
{
	if (m_Animation) { m_Animation->SetDestroy(); }
	m_IsDestroy = true;
}

/*******************************************************************************
*	テクスチャ変更
*******************************************************************************/
void Plate2DRenderer::SetTexture(string name)
{
	m_TextureName = name;
	m_Texture = TextureContainer::GetTexture_InName(m_TextureName);

	if (m_Texture)
	{
		// テクスチャ座標を再計算
		m_TexCoordDistance.x = 1.0f / m_Texture->GetWidthDiv();
		m_TexCoordDistance.y = 1.0f / m_Texture->GetHeightDiv();
	}
}

/*******************************************************************************
*	頂点情報を取得
*******************************************************************************/
void Plate2DRenderer::GetVertex(VERTEX_3D* vertex)
{
	D3DXVECTOR3 s = m_AttachObject->GetWorldScale();
	D3DXVECTOR2 halfScale;

	// ピボット位置に応じた設定で生成
	switch (m_AnchorPosition)
	{
	case AnchorPosition::Left_Top:
		halfScale = { m_Size.x * s.x * 0.5f, m_Size.y * s.y * 0.5f };
		CreateVertex(vertex, halfScale);
		break;
	case AnchorPosition::Center_Middle:
		CreateVertex(vertex, D3DXVECTOR2(0.0f, 0.0f));
		break;
	default: return;
	}
}

/*******************************************************************************
*	頂点情報を生成
*******************************************************************************/
void Plate2DRenderer::CreateVertex(VERTEX_3D* vertex, D3DXVECTOR2 offset)
{
	D3DXVECTOR2 p;
	D3DXVECTOR3 n = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR2 rotOffsetPlus;
	D3DXVECTOR2 rotOffsetMinus;

	// 親の回転を反映
	CalcParentRotate(p, offset);
	// 自身の回転を反映
	CalcThisRotate(rotOffsetPlus, rotOffsetMinus);

	vertex[0].Position = D3DXVECTOR3(p.x - rotOffsetMinus.x, p.y - rotOffsetMinus.y, 0.0f);
	vertex[0].TexCoord = D3DXVECTOR2(m_TexCoordBegin.x,
									m_TexCoordBegin.y);

	vertex[1].Position = D3DXVECTOR3(p.x + rotOffsetPlus.x, p.y - rotOffsetPlus.y, 0.0f);
	vertex[1].TexCoord = D3DXVECTOR2(m_TexCoordBegin.x + m_TexCoordDistance.x,
									m_TexCoordBegin.y);

	vertex[2].Position = D3DXVECTOR3(p.x - rotOffsetPlus.x, p.y + rotOffsetPlus.y, 0.0f);
	vertex[2].TexCoord = D3DXVECTOR2(m_TexCoordBegin.x,
									m_TexCoordBegin.y + m_TexCoordDistance.y);

	vertex[3].Position = D3DXVECTOR3(p.x + rotOffsetMinus.x, p.y + rotOffsetMinus.y, 0.0f);
	vertex[3].TexCoord = D3DXVECTOR2(m_TexCoordBegin.x + m_TexCoordDistance.x,
									m_TexCoordBegin.y + m_TexCoordDistance.y);

	for (int i = 0; i < 4; i++)
	{
		vertex[i].Normal = n;
		vertex[i].Diffuse = m_TexColor;
	}
}

/*******************************************************************************
*	親の回転を反映
*******************************************************************************/
void Plate2DRenderer::CalcParentRotate(D3DXVECTOR2& p, D3DXVECTOR2 offset)
{
	p = { m_AttachObject->GetLocalPositionOrigin().x + offset.x, m_AttachObject->GetLocalPositionOrigin().y + offset.y };
	D3DXVECTOR2 localPos = { m_AttachObject->m_LocalPosition.x, m_AttachObject->m_LocalPosition.y };
	float baseAngle = atan2f(localPos.x, localPos.y);
	float len = D3DXVec2Length(&localPos);
	float rot = Math::QuaternionToEulerAngle(m_AttachObject->GetLocalRotationOrigin()).z;
	p += { sinf(baseAngle + rot)* len, cosf(baseAngle + rot)* len };
}

/*******************************************************************************
*	自身の回転を反映
*******************************************************************************/
void Plate2DRenderer::CalcThisRotate(D3DXVECTOR2& rotOffsetPlus, D3DXVECTOR2& rotOffsetMinus)
{
	D3DXVECTOR3 s = m_AttachObject->GetWorldScale();
	D3DXVECTOR2 halfScale = { m_Size.x * s.x * 0.5f, m_Size.y * s.y * 0.5f };
	float baseAngle = atan2f(halfScale.x, halfScale.y);
	float len = D3DXVec2Length(&halfScale);
	float rot = Math::QuaternionToEulerAngle(m_AttachObject->m_LocalRotation).z;
	rotOffsetPlus = { sinf(baseAngle + rot) * len, cosf(baseAngle + rot) * len };
	rotOffsetMinus = { sinf(baseAngle - rot) * len, cosf(baseAngle - rot) * len };
}

#ifdef _DEBUG
/*******************************************************************************
*	インスペクター表示処理
*******************************************************************************/
void Plate2DRenderer::DrawInspector()
{
	if (ImGui::CollapsingHeader("Plate2DRenderer"))
	{
		int tempInt;
		float tempVec2[2];
		float tempCol[4];

		ImGui::Text("TextureName : %s", m_TextureName.c_str());

		tempInt = (int)m_AnchorPosition;
		if (DebugManager::ComboBox("AnchorPosition", tempInt, m_ANCHOR_POSITION_STR))
		{
			m_AnchorPosition = (Plate2DRenderer::AnchorPosition)tempInt;
		}

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
