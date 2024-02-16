/*******************************************************************************
*
*	タイトル：	スカイドーム静的クラス
*	ファイル：	skyDome.cpp
*	作成者：	古本 泰隆
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

	// ドームのサイズ設定(最大描画距離 * 0.9)
	m_Size = 1000.0f * 0.9f;
}

void SkyDome::Update()
{
	// 現在有効になっているカメラを取得
	Camera* camera = Manager::GetScene()->GetActiveCamera();
	if (camera)
	{
		// カメラに追従させる
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

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ワールドマトリクス設定
	D3DXMATRIX world, scale;
	D3DXMatrixScaling(&scale, m_Size, m_Size, m_Size);
	world = scale * m_AttachObject->GetWorldMatrix();
	Renderer::SetWorldMatrix(&world);

	// モデル描画
	m_Model->Draw();
}

#ifdef _DEBUG
/*******************************************************************************
*	インスペクター表示処理
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
