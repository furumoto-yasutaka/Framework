/*******************************************************************************
*
*	タイトル：	遷移演出用静的クラス
*	ファイル：	transition.cpp
*	作成者：	古本 泰隆
*
*******************************************************************************/
#include "transition.h"
#include "texture.h"
#include "time.h"

#include <string>

void Transition::Init()
{
	// 初期化
	m_State = TransitionState::None;

	//-------------------
	// バッファ設定
	//-------------------
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	VERTEX_3D vertex[4];
	CreateVertex(vertex, 1.0f);
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	// バッファ生成
	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	// シェーダー設定
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}

void Transition::Uninit()
{
	m_VertexShader->Release();
	m_VertexLayout->Release();
	m_PixelShader->Release();
	m_VertexBuffer->Release();
	if (m_Callback) { delete m_Callback; }
}

void Transition::Update()
{
	// トランジション終了状態の場合は何もせず終了する
	if (m_State == TransitionState::OutComplete ||
		m_State == TransitionState::None)
	{
		return;
	}
	// Noneパターンは即座に変化の完了を表すステートに変更する
	if (m_Option.Mode[(int)m_State] == TransitionPattern::None)
	{
		if (m_State == TransitionState::Out)
		{
			m_State = TransitionState::OutComplete;
		}
		else
		{
			m_State = TransitionState::None;
		}
		return;
	}
	// ロードが行われるフレームは何もしない
	if (m_IsLoadFrame)
	{
		m_IsLoadFrame = false;
		return;
	}

	// 経過時間を加算
	m_TimeCountSec += Time::GetDeltaTimeSec();	// ミリ秒から秒に変換
	
	// 指定の時間経過したら変化の完了を表すステートに変更する
	if (m_TimeCountSec >= m_Option.TimeLimitSec[(int)m_State])
	{
		if (m_State == TransitionState::Out)
		{
			m_State = TransitionState::OutComplete;
		}
		else
		{
			m_State = TransitionState::None;
		}
	}
}

void Transition::Draw()
{
	// テクスチャがロードされているか確認
	if (m_State == TransitionState::None)
	{
		return;
	}

	// トランジション設定に合わせて描画を行う
	switch (m_Option.Mode[(int)m_State % 2])
	{
	case TransitionPattern::Fade:
		Renderer::SetAlphaToCaverage(false);
		Draw_Fade();
		Renderer::SetAlphaToCaverage(true);
		break;
	case TransitionPattern::Wipe:
		Draw_Wipe();
		break;
	}
}

/*******************************************************************************
*	シーン切り替え前トランジション開始
*******************************************************************************/
void Transition::StartTransitionOut(TransitionOption option)
{
	if (m_State == TransitionState::In)
	{
		float remainRate = (1.0f - m_TimeCountSec / m_Option.TimeLimitSec[(int)TransitionState::In]);
		m_TimeCountSec = remainRate * option.TimeLimitSec[(int)TransitionState::Out];
	}
	else
	{
		m_TimeCountSec = 0.0f;
	}

	m_Option = option;
	m_State = TransitionState::Out;
}

/*******************************************************************************
*	シーン切り替え後トランジション開始
*******************************************************************************/
void Transition::StartTransitionIn()
{
	if (m_State == TransitionState::Out)
	{
		float remainRate = (1.0f - m_TimeCountSec / m_Option.TimeLimitSec[(int)TransitionState::Out]);
		m_TimeCountSec = remainRate * m_Option.TimeLimitSec[(int)TransitionState::In];
	}
	else
	{
		m_TimeCountSec = 0.0f;
	}

	m_State = TransitionState::In;
	m_IsLoadFrame = true;

	// コールバック関数呼出
	if (m_Callback)
	{
		m_Callback->Execute();
		delete m_Callback;
		m_Callback = NULL;
	}
}

/*******************************************************************************
*	ゲーム起動時のトランジションオプションを取得
*******************************************************************************/
Transition::TransitionOption Transition::GetFirstTransitionOption()
{
	TransitionOption info;
	info.Mode[(int)Transition::TransitionState::In] = Transition::TransitionPattern::Fade;
	info.TimeLimitSec[(int)Transition::TransitionState::In] = 1.5f;
	info.Texture = TextureContainer::GetTexture_InName("Black");

	return info;
}

void Transition::Draw_Fade()
{
	// トランジションの進行状況に応じた透明度を設定する(Fadeパターン)
	float alpha = 1.0f;
	switch (m_State)
	{
	case TransitionState::Out:
		alpha = m_TimeCountSec / m_Option.TimeLimitSec[(int)TransitionState::Out];
		break;
	case TransitionState::In:
		alpha = 1.0f - (m_TimeCountSec / m_Option.TimeLimitSec[(int)TransitionState::In]);
		break;
	case TransitionState::OutComplete:
		break;
	default: return;
	}

	// 頂点情報更新
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);
	CreateVertex((VERTEX_3D*)msr.pData, alpha);
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
	ID3D11ShaderResourceView* resource = m_Option.Texture->GetResource();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &resource);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

void Transition::Draw_Wipe()
{
	// トランジションの進行状況に応じた透明度を設定する(Fadeパターン)
	float rate = 1.0f;
	switch (m_State)
	{
	case TransitionState::Out:
		rate = m_TimeCountSec / m_Option.TimeLimitSec[(int)TransitionState::Out];
		break;
	case TransitionState::In:
		rate = 1.0f - (m_TimeCountSec / m_Option.TimeLimitSec[(int)TransitionState::In]);
		break;
	case TransitionState::OutComplete:
		break;
	default: return;
	}

	//★★1回目(左)★★

	// 頂点情報更新
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);
	CreateVertex_WipeLeft((VERTEX_3D*)msr.pData, rate);
	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// テクスチャ設定
	ID3D11ShaderResourceView* resource = m_Option.Texture->GetResource();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &resource);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);


	//★★2回目(右)★★

	// 頂点情報更新
	msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);
	CreateVertex_WipeRight((VERTEX_3D*)msr.pData, rate);
	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 頂点バッファ設定
	stride = sizeof(VERTEX_3D);
	offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// テクスチャ設定
	resource = m_Option.Texture->GetResource();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &resource);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);
}

/*******************************************************************************
*	頂点情報を生成
*******************************************************************************/
void Transition::CreateVertex(VERTEX_3D* vertex, float alpha)
{
	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(Application::m_WINDOW_RESOLUTION.x, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, Application::m_WINDOW_RESOLUTION.y, 0.0f);
	vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(Application::m_WINDOW_RESOLUTION.x, Application::m_WINDOW_RESOLUTION.y, 0.0f);
	vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
}

/*******************************************************************************
*	頂点情報を生成
*******************************************************************************/
void Transition::CreateVertex_WipeLeft(VERTEX_3D* vertex, float rate)
{
	float X = -Application::m_WINDOW_RESOLUTION.x * 0.5f * (1.0f - rate);
	vertex[0].Position = D3DXVECTOR3(X, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(X + Application::m_WINDOW_RESOLUTION.x * 0.5f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(0.5f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(X, Application::m_WINDOW_RESOLUTION.y, 0.0f);
	vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(X + Application::m_WINDOW_RESOLUTION.x * 0.5f, Application::m_WINDOW_RESOLUTION.y, 0.0f);
	vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(0.5f, 1.0f);
}

/*******************************************************************************
*	頂点情報を生成
*******************************************************************************/
void Transition::CreateVertex_WipeRight(VERTEX_3D* vertex, float rate)
{
	float X = Application::m_WINDOW_RESOLUTION.x * 0.5f * (1.0f - rate);
	vertex[0].Position = D3DXVECTOR3(X + Application::m_WINDOW_RESOLUTION.x * 0.5f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.5f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(X + Application::m_WINDOW_RESOLUTION.x, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(X + Application::m_WINDOW_RESOLUTION.x * 0.5f, (float)Application::m_WINDOW_RESOLUTION.y, 0.0f);
	vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.5f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(X + Application::m_WINDOW_RESOLUTION.x, Application::m_WINDOW_RESOLUTION.y, 0.0f);
	vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
}
