/*******************************************************************************
*
*	�^�C�g���F	�����s���E3D�R���W�����R���|�[�l���g
*	�t�@�C���F	aabbCollider3d.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "collider3d.h"

class AABBCollider3D : public Collider3D
{
private:
	D3DXVECTOR3	m_LocalRadius;

public:
	AABBCollider3D(GameObject* attachObject)
		: Collider3D(attachObject, Collider3D::ColliderId::AABB)
	{}
	~AABBCollider3D() {}

	void Init(D3DXVECTOR3 offset, D3DXVECTOR3 radius,
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

	D3DXVECTOR3 GetWorldRadius();

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	D3DXVECTOR3 GetLocalRadius() { return m_LocalRadius; }
	void SetRadius(D3DXVECTOR3 radius) { m_LocalRadius = radius; }
	/////////////////////////////

#ifdef _DEBUG
private:
	ID3D11Buffer* m_VertexBuffer[4] = { NULL, NULL, NULL, NULL };	// ���_�o�b�t�@
	ID3D11VertexShader* m_VertexShader = NULL;	// ���_�V�F�[�_�[
	ID3D11PixelShader* m_PixelShader = NULL;	// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout* m_VertexLayout = NULL;	// ���̓��C�A�E�g

public:
	void DrawInspector() override;
	void InitDebug() override;
	void UninitDebug() override;
	void DrawDebug3d() override;
private:
	void CreateVertex(VERTEX_3D* vertex, int index, D3DXVECTOR3 vertexPos[2][4]);
#endif
};
