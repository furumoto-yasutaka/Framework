/*******************************************************************************
*
*	タイトル：	遷移演出用静的クラス
*	ファイル：	transition.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "renderer.h"
#include "textureContainer.h"
#include "callbackListner.h"
#include "draw2dComponent.h"

class Texture;

class Transition : public Draw2DComponent
{
public:
	// トランジションの形式
	enum class TransitionPattern
	{
		None = 0,	// ぱっと切り替え
		Fade,		// フェード
		Wipe,
	};
	// トランジションの状況
	enum class TransitionState
	{
		Out = 0,		// 画面を隠していく
		In,				// 画面を見せていく
		OutComplete,	// 画面が隠れた状態になった
		None,			// 無処理・画面が見える状態になった
		Length,
	};

	// トランジション設定用構造体
	struct TransitionOption
	{
		Transition::TransitionPattern	Mode[2];			// 現在は未使用
		float							TimeLimitSec[2];
		Texture*						Texture;

		TransitionOption()
		{
			for (int i = 0; i < 2; i++)
			{
				Mode[i] = TransitionPattern::None;
				TimeLimitSec[i] = 0.0f;
			}
			Texture = NULL;
		}
	};

private:
	static TransitionOption	m_Option;			// トランジション設定

	ID3D11Buffer*		m_VertexBuffer = NULL;	// 頂点バッファ
	ID3D11VertexShader* m_VertexShader = NULL;	// 頂点シェーダー
	ID3D11PixelShader*	m_PixelShader = NULL;	// ピクセルシェーダー
	ID3D11InputLayout*	m_VertexLayout = NULL;	// 入力レイアウト

	TransitionState		m_State = TransitionState::None;	// トランジションの状況
	float				m_TimeCountSec = 0.0f;	// 経過時間
	CallbackBase<>*		m_Callback = NULL;		// コールバック関数(シーンが切り替わった瞬間呼ばれる)

public:
	Transition(GameObject* attachObject)
		: Draw2DComponent(attachObject)
	{}
	~Transition() {}

	void Init();
	void Uninit() override;
	void Update() override;
	void Draw2d() override;

	// シーン切り替え前トランジション開始
	void StartTransitionOut();

	// シーン切り替え後トランジション開始
	void StartTransitionIn();

	// ゲーム起動時のトランジションオプションを取得
	static TransitionOption GetFirstTransitionOption();

	// コールバックを設定
	template<class T>
	void SetCallback(T* origin, void(T::* function)())
	{
		m_Callback = DBG_NEW Callback(origin, function);
	}

	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	static void SetTransitionOption(TransitionOption option) { m_Option = option; }

	TransitionState GetTransitionState() { return m_State; }
	/////////////////////////////
private:
	// 頂点情報を生成
	void CreateVertex(VERTEX_3D* vertex, float alpha);
	void CreateVertex_WipeLeft(VERTEX_3D* vertex, float rate);
	void CreateVertex_WipeRight(VERTEX_3D* vertex, float rate);

	// 描画処理
	void Draw_Fade();
	void Draw_Wipe();
};
