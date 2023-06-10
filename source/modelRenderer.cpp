/*******************************************************************************
*
*	タイトル：	モデル描画コンポーネント
*	ファイル：	modelRenderer.cpp
*	作成者：	古本 泰隆
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

	// モデル取得
	m_Model = ModelContainer::GetModel_InName(m_ModelName);

	// シェーダー設定
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

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// ワールドマトリクス設定
	D3DXMATRIX world, scale;
	float s = m_Model->GetScale();
	D3DXMatrixScaling(&scale, s, s, s);
	world = scale * m_AttachObject->GetWorldMatrix();
	Renderer::SetWorldMatrix(&world);

	// モデル描画
	m_Model->Draw();
}

/*******************************************************************************
*	モデル変更
*******************************************************************************/
void ModelRenderer::SetModel(string modelName)
{
	m_ModelName = modelName;

	m_Model = ModelContainer::GetModel_InName(m_ModelName);
}

#ifdef _DEBUG
/*******************************************************************************
*	インスペクター表示処理
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
