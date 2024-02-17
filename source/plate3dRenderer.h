/*******************************************************************************
*
*	タイトル：	3D板ポリゴン描画コンポーネント
*	ファイル：	plate3dRenderer.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "draw3dComponent.h"
#include "renderer.h"

#include <vector>
#include <string>

class Texture;
class TextureAnimation;

class Plate3DRenderer : public Draw3DComponent
{
public:
	// ピボット位置
	enum class AnchorPosition
	{
		Left_Top,		// 左上
		Center_Middle,	// 中央
	};

private:
	string				m_TextureName;			// テクスチャの名称(コンテナ上の名前)
	Texture*			m_Texture = NULL;		// テクスチャの詳細情報
	ID3D11Buffer*		m_VertexBuffer = NULL;	// 頂点バッファ
	ID3D11VertexShader* m_VertexShader = NULL;	// 頂点シェーダー
	ID3D11PixelShader*	m_PixelShader = NULL;	// ピクセルシェーダー
	ID3D11InputLayout*	m_VertexLayout = NULL;	// 入力レイアウト

	AnchorPosition				m_AnchorPosition = AnchorPosition::Center_Middle;	// ピボット位置
	bool						m_IsBillBoard = false;			// ビルボード設定
	bool						m_IsAlphaToCoverage = false;	// アルファトゥカパレッジ設定
	D3DXVECTOR2					m_Size;							// 描画サイズ
	D3DXVECTOR2					m_TexCoordBegin;				// テクスチャ座標(始点)
	D3DXVECTOR2					m_TexCoordDistance;				// テクスチャ座標(終点)
	D3DXCOLOR					m_TexColor;						// テクスチャカラー
	TextureAnimation*			m_Animation = NULL;				// 接続中のテクスチャアニメーションコンポーネント

public:
	Plate3DRenderer(GameObject* attachObject)
		: Draw3DComponent(attachObject)
	{}
	~Plate3DRenderer() {}

	void Init(const char* textureName, D3DXVECTOR2 size,
		AnchorPosition anchorPos = AnchorPosition::Center_Middle,
		bool isBillBoard = false, bool isAlphaToCoverage = true);
	void Uninit() override;
	void Update() override {}
	void Draw3d() override;

	// テクスチャアニメーションと接続
	void Connect(TextureAnimation* animation,
		D3DXVECTOR2*& texCoordBegin, D3DXVECTOR2*& texCoordDistance,
		int& widthDiv, int& heightDiv);

	// テクスチャアニメーションとの接続を解除
	void DisConnect();

	// 自身の削除を予約
	void SetDestroy() override;

private:
	void CreateBuffer();
	void CreateShader();

	// 頂点情報を取得
	void GetVertex(VERTEX_3D* vertex);

	// 頂点情報を生成
	void CreateVertex(VERTEX_3D* vertex, D3DXVECTOR2 offset);

public:
	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	string GetTextureName() { return m_TextureName; }

	Texture* GetTexture() { return m_Texture; }
	void SetTexture(string name);

	AnchorPosition GetAnchorPosition() { return m_AnchorPosition; }
	void SetAnchorPosition(AnchorPosition anchorPos) { m_AnchorPosition = anchorPos; }

	bool GetIsBillBoard() { return m_IsBillBoard; }
	void SetIsBillBoard(bool isBillBoard) { m_IsBillBoard = isBillBoard; }

	bool GetIsAlphaToCoverage() { return m_IsAlphaToCoverage; }
	void SetIsAlphaToCoverage(bool isAlphaToCoverage) { m_IsAlphaToCoverage = isAlphaToCoverage; }

	D3DXVECTOR2 GetSize() { return m_Size; }
	void SetSize(D3DXVECTOR2 size) { m_Size = size; }

	D3DXVECTOR2 GetTexCoordBegin() { return m_TexCoordBegin; }
	void SetTexCoordBegin(D3DXVECTOR2 coord) { m_TexCoordBegin = coord; }
	void SetTexCoordBeginX(float x) { m_TexCoordBegin.x = x; }
	void SetTexCoordBeginY(float y) { m_TexCoordBegin.y = y; }

	D3DXVECTOR2 GetTexCoordDistance() { return m_TexCoordDistance; }
	void SetTexCoordDistance(D3DXVECTOR2 coord) { m_TexCoordDistance = coord; }
	void SetTexCoordDistanceX(float x) { m_TexCoordDistance.x = x; }
	void SetTexCoordDistanceY(float y) { m_TexCoordDistance.y = y; }

	D3DXCOLOR GetTexColor() { return m_TexColor; }
	void SetTexColor(D3DXCOLOR color) { m_TexColor = color; }
	void SetTexAlphaColor(float alpha) { m_TexColor.a = alpha; }
	void SetTexRGBColor(float red, float green, float blue)
	{
		m_TexColor.r = red;
		m_TexColor.g = green;
		m_TexColor.b = blue;
	}

	TextureAnimation* GetTextureAnimation() { return m_Animation; }
	/////////////////////////////

#ifdef _DEBUG
private:
	const vector<string> m_ANCHOR_POSITION_STR = {
		"Left_Top",
		"Center_Middle",
	};

public:
	void DrawInspector() override;
#endif
};
