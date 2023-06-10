/*******************************************************************************
*
*	タイトル：	テクスチャブラーコンポーネント
*	ファイル：	plate2dBlur.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "plate2dBlur.h"
#include "gameObject.h"
#include "plate2dRenderer.h"
#include "texture.h"

void Plate2DBlur::Init(Plate2DRenderer* plate2D, unsigned int bulrLayer,
	unsigned int bulrInterval, unsigned int interpolationLayer)
{
	m_Plate2DRenderer = plate2D;

	// 不正な値の場合は補正する
	if (bulrLayer <= 0) { m_BulrLayer = 1; }
	else { m_BulrLayer = bulrLayer; }

	// 不正な値の場合は補正する
	if (bulrInterval <= 0) { m_BulrInterval = 1; }
	else { m_BulrInterval = bulrInterval; }

	m_BulrAlphaDecayRate = 1.0f / (m_BulrLayer * interpolationLayer);
	m_InterpolationLayer = interpolationLayer;

	//-------------------
	// バッファ設定
	//-------------------
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// バッファ生成
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	// シェーダー設定
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}

void Plate2DBlur::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Plate2DBlur::Update()
{
	if (m_FrameCount >= m_BulrInterval)
	{
		// ブラーが最大数の場合最後に生成したブラーを削除する
		if ((signed)m_BulrPosition.size() == m_BulrLayer)
		{
			m_BulrPosition.pop_back();
			m_BulrRotationZ.pop_back();
			
			for (int i = 0; i < m_InterpolationLayer; i++)
			{
				m_BulrPosition.pop_back();
				m_BulrRotationZ.pop_back();
			}
		}

		// ブラーを追加する
		m_BulrPosition.push_front({ m_AttachObject->GetWorldPosition().x, m_AttachObject->GetWorldPosition().y });
		m_BulrRotationZ.push_front(Math::QuaternionToEulerAngle(m_AttachObject->GetWorldRotation()).z);

		// 補正ブラーも追加する
		auto posItr = m_BulrPosition.rbegin();
		auto posItrNext = posItr; posItrNext++;
		D3DXVECTOR2 divPosRate = *posItrNext - *posItr;
		divPosRate /= (float)(m_InterpolationLayer + 1);

		auto rotItr = m_BulrRotationZ.rbegin();
		auto rotItrNext = rotItr; rotItrNext++;
		float divRotRate = *rotItrNext - *rotItr;
		divRotRate /= (float)(m_InterpolationLayer + 1);

		for (int i = 0; i < m_InterpolationLayer; i++)
		{
			m_BulrPosition.push_front(*posItr + divPosRate * (float)i);
			m_BulrRotationZ.push_front(*rotItr + divRotRate * (float)i);
		}
	}

	m_FrameCount++;
}

void Plate2DBlur::Draw2d()
{
	auto posItr = m_BulrPosition.rbegin();
	auto rotItr = m_BulrRotationZ.rbegin();
	int i = 0;
	for (; posItr != m_BulrPosition.rend() && rotItr != m_BulrRotationZ.rend(); posItr++, rotItr++, i++)
	{
		// 頂点情報書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		float alpha = m_BulrAlphaDecayRate * i;	// 透明度
		GetVertex((VERTEX_3D*)msr.pData, *posItr, alpha, *rotItr);

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
		ID3D11ShaderResourceView* resource = m_Plate2DRenderer->GetTexture()->GetResource();
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &resource);

		// プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
}

/*******************************************************************************
*	頂点情報を取得
*******************************************************************************/
void Plate2DBlur::GetVertex(VERTEX_3D* vertex, D3DXVECTOR2 pos, float alpha, float rot)
{
	D3DXVECTOR3 worldScale = m_AttachObject->GetWorldScale();
	D3DXVECTOR2 size = m_Plate2DRenderer->GetSize();
	D3DXVECTOR2 halfScale;

	// ピボット位置に応じた設定で生成
	switch (m_Plate2DRenderer->GetAnchorPosition())
	{
	case Plate2DRenderer::AnchorPosition::Left_Top:
		halfScale = { size.x * worldScale.x * 0.5f, size.y * worldScale.y * 0.5f };
		CreateVertex(vertex, pos, halfScale, alpha, rot);
		break;
	case Plate2DRenderer::AnchorPosition::Center_Middle:
		CreateVertex(vertex, pos, D3DXVECTOR2(0.0f, 0.0f), alpha, rot);
		break;
	default: return;
	}
}

/*******************************************************************************
*	頂点情報を生成
*******************************************************************************/
void Plate2DBlur::CreateVertex(VERTEX_3D* vertex, D3DXVECTOR2 pos, D3DXVECTOR2 offset, float alpha, float rot)
{
	D3DXVECTOR2 p = pos - offset;
	D3DXVECTOR3 n = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR c = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
	D3DXVECTOR2 begin = m_Plate2DRenderer->GetTexCoordBegin();
	D3DXVECTOR2 distance = m_Plate2DRenderer->GetTexCoordDistance();
	D3DXVECTOR2 rotOffsetPlus;
	D3DXVECTOR2 rotOffsetMinus;

	// 回転を反映した頂点座標の情報を保存
	{
		D3DXVECTOR2 size = m_Plate2DRenderer->GetSize();
		D3DXVECTOR3 worldScale = m_AttachObject->GetWorldScale();
		D3DXVECTOR2 halfScale = { size.x * worldScale.x * 0.5f, size.y * worldScale.y * 0.5f };
		float baseAngle = atan2f(halfScale.x, halfScale.y);
		float len = D3DXVec2Length(&halfScale);
		rotOffsetPlus = { sinf(baseAngle + rot) * len, cosf(baseAngle + rot) * len };
		rotOffsetMinus = { sinf(baseAngle - rot) * len, cosf(baseAngle - rot) * len };
	}

	vertex[0].Position = D3DXVECTOR3(p.x - rotOffsetMinus.x, p.y - rotOffsetMinus.y, 0.0f);
	vertex[0].Normal = n;
	vertex[0].Diffuse = c;
	vertex[0].TexCoord = begin;

	vertex[1].Position = D3DXVECTOR3(p.x + rotOffsetPlus.x, p.y - rotOffsetPlus.y, 0.0f);
	vertex[1].Normal = n;
	vertex[1].Diffuse = c;
	vertex[1].TexCoord = D3DXVECTOR2(begin.x + distance.x, begin.y);

	vertex[2].Position = D3DXVECTOR3(p.x - rotOffsetPlus.x, p.y + rotOffsetPlus.y, 0.0f);
	vertex[2].Normal = n;
	vertex[2].Diffuse = c;
	vertex[2].TexCoord = D3DXVECTOR2(begin.x, begin.y + distance.y);

	vertex[3].Position = D3DXVECTOR3(p.x + rotOffsetMinus.x, p.y + rotOffsetMinus.y, 0.0f);
	vertex[3].Normal = n;
	vertex[3].Diffuse = c;
	vertex[3].TexCoord = begin + distance;
}

#ifdef _DEBUG
/*******************************************************************************
*	インスペクター表示処理
*******************************************************************************/
void Plate2DBlur::DrawInspector()
{
	if (ImGui::CollapsingHeader("Plate2DBlur"))
	{
		string tempStr;

		ImGui::SliderInt("Layer", &m_BulrLayer, 0, 100);

		ImGui::SliderInt("Interval(Frame)", &m_BulrInterval, 0, 1000);

		ImGui::SliderInt("InterpolationLayer", &m_InterpolationLayer, 0, 1000);

		ImGui::SliderFloat("AlphaDecayRate", &m_BulrAlphaDecayRate, 0.0f, 100.0f);

		ImGui::Text("InterpolationLayer : %d", m_FrameCount);
	}
}
#endif
