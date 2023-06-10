/*******************************************************************************
*
*	タイトル：	球型3Dコリジョンコンポーネント
*	ファイル：	sphereCollider3d.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "collider3d.h"

class SphereCollider3D : public Collider3D
{
private:
	float m_LocalRadius = 0.0f;

public:
	SphereCollider3D(GameObject* attachObject)
		: Collider3D(attachObject, Collider3D::ColliderId::Sphere)
	{}
	~SphereCollider3D() {}

	void Init(D3DXVECTOR3 offset, float radius,
		bool isTrigger = false, bool isStatic = false);
	void Uninit() override;
	void Update() override {}
	void Draw3d() override {}

	void CollisionAABB(AABBCollider3D* collider) override;
	void CollisionOBB(OBBCollider3D* collider) override;
	void CollisionSphere(SphereCollider3D* collider) override;
	void CollisionCapsule(CapsuleCollider3D* collider) override;

	D3DXVECTOR3 GetMortonMinPos() override;
	D3DXVECTOR3 GetMortonMaxPos() override;

	float GetWorldRadius();

	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//
	float GetLocalRadius() { return m_LocalRadius; }
	void SetLocalRadius(float radius) { m_LocalRadius = radius; }
	/////////////////////////////

#ifdef _DEBUG
private:
	static inline int m_QuarterCircleVertexNum = 25;
	static inline int m_CircleVertexNum = m_QuarterCircleVertexNum * 4;
	static inline int m_VertexNum = m_CircleVertexNum * 3 + 1;
	ID3D11Buffer* m_VertexBuffer = NULL;		// 頂点バッファ
	ID3D11VertexShader* m_VertexShader = NULL;	// 頂点シェーダー
	ID3D11PixelShader* m_PixelShader = NULL;	// ピクセルシェーダー
	ID3D11InputLayout* m_VertexLayout = NULL;	// 入力レイアウト

public:
	void DrawInspector() override;
	void InitDebug() override;
	void UninitDebug() override;
	void DrawDebug3d() override;
private:
	void CreateVertex(VERTEX_3D* vertex);
#endif
};
