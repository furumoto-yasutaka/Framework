/*******************************************************************************
*
*	�^�C�g���F	�J�v�Z���^3D�R���W�����R���|�[�l���g
*	�t�@�C���F	capsuleCollider3d.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "collider3d.h"

class CapsuleCollider3D : public Collider3D
{
public:
	enum class DirectionPattern
	{
		X = 0,
		Y,
		Z,
	};

private:
	float m_LocalRadius = 0.0f;
	float m_LocalHeight = 0.0f;
	DirectionPattern m_HeightDirection = DirectionPattern::X;

public:
	CapsuleCollider3D(GameObject* attachObject)
		: Collider3D(attachObject, Collider3D::ColliderId::Capsule)
	{}
	~CapsuleCollider3D() {}

	void Init(D3DXVECTOR3 offset, float radius, float height,
		DirectionPattern heightDirection,
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
	float GetWorldHeight();

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	float GetLocalRadius() { return m_LocalRadius; }
	void SetLocalRadius(float radius) { m_LocalRadius = radius; }

	float GetLocalHeight() { return m_LocalHeight; }
	void SetLocalHeight(float radius) { m_LocalHeight = radius; }

	DirectionPattern GetDirection() { return m_HeightDirection; }
	void SetDirection(DirectionPattern direction) { m_HeightDirection = direction; }
	/////////////////////////////

#ifdef _DEBUG
private:
	static inline int m_QuarterCircleVertexNum = 25;
	static inline int m_HalfCircleVertexNum = m_QuarterCircleVertexNum * 2;
	static inline int m_VertexNum = m_HalfCircleVertexNum * 8 + 5;
	ID3D11Buffer* m_VertexBuffer = NULL;		// ���_�o�b�t�@
	ID3D11VertexShader* m_VertexShader = NULL;	// ���_�V�F�[�_�[
	ID3D11PixelShader* m_PixelShader = NULL;	// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout* m_VertexLayout = NULL;	// ���̓��C�A�E�g

public:
	void DrawInspector() override;
	void InitDebug() override;
	void UninitDebug() override;
	void DrawDebug3d() override;
private:
	void CreateVertex(VERTEX_3D* vertex);
	void CreateVertexAxisX(VERTEX_3D* vertex, float straightHalfLen);
	void CreateVertexAxisY(VERTEX_3D* vertex, float straightHalfLen);
	void CreateVertexAxisZ(VERTEX_3D* vertex, float straightHalfLen);
#endif
};
