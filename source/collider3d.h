/*******************************************************************************
*
*	�^�C�g���F	3D�R���W�������N���X
*	�t�@�C���F	collider3d.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "draw3dComponent.h"
#include "callbackListner.h"

#include <list>
#include <optional>

template<class T>
class TreeCellData;
class AABBCollider3D;
class OBBCollider3D;
class SphereCollider3D;
class CapsuleCollider3D;
struct VERTEX_3D;

class Collider3D : public Draw3DComponent
{
public:
	enum class ColliderId
	{
		AABB,
		OBB,
		Sphere,
		Capsule,
	};

protected:
	TreeCellData<Collider3D>* m_ThisCellData = NULL;
	ColliderId m_Id;
	D3DXVECTOR3 m_Offset;
	bool m_IsContact = false;
	bool m_IsPrevContact = false;
	bool m_IsTrigger = false;
	bool m_IsStatic = false;
	list<Collider3D*> m_ContactList;	// ���ݏՓ˂��Ă���I�u�W�F�N�g�̃��X�g
	list<Collider3D*> m_RemainList;		// �O�t���[���ɏՓ˂��Ă���I�u�W�F�N�g�̃��X�g

	D3DXVECTOR3* m_VelocityRef = NULL;	// �Փ˂𑬓x�ɔ��f������ׂ̎Q��
	float m_Damp = 0.0f;				// �Փˎ��̔��ˌW��

	CallBackBase<Collider3D*>* m_OnContactEnter = NULL;	// �ڐG�J�n�t���[���̃R�[���o�b�N�֐�
	CallBackBase<Collider3D*>* m_OnContactStay = NULL;	// �ڐG���̃R�[���o�b�N�֐�
	CallBackBase<Collider3D*>* m_OnContactExit = NULL;	// �ڐG�I���t���[���̃R�[���o�b�N�֐�

public:
	Collider3D(GameObject* attachObject, ColliderId id)
		: Draw3DComponent(attachObject),
		m_Id(id)
	{}
	virtual ~Collider3D() {}

protected:
	void Init(D3DXVECTOR3 offset, bool isTrigger, bool isStatic);
public:
	virtual void Uninit() override;

	void Collision(Collider3D* collider);
	virtual void CollisionAABB(AABBCollider3D* collider) = 0;
	virtual void CollisionOBB(OBBCollider3D* collider) = 0;
	virtual void CollisionSphere(SphereCollider3D* collider) = 0;
	virtual void CollisionCapsule(CapsuleCollider3D* collider) = 0;

	D3DXVECTOR3 GetColliderWorldPos();
	virtual D3DXVECTOR3 GetMortonMinPos() = 0;
	virtual D3DXVECTOR3 GetMortonMaxPos() = 0;

	void CheckCallBack();

	// �ڐG�J�n�t���[���̃R�[���o�b�N�֐���o�^
	template<class T>
	void SetOnContactEnter(T* origin, void(T::* function)(Collider3D*))
	{
		m_OnContactEnter = DBG_NEW CallBack(origin, function);
	}

	// �ڐG���̃R�[���o�b�N�֐���o�^
	template<class T>
	void SetOnContactStay(T* origin, void(T::* function)(Collider3D*))
	{
		m_OnContactStay = DBG_NEW CallBack(origin, function);
	}

	// �ڐG�I���t���[���̃R�[���o�b�N�֐���o�^
	template<class T>
	void SetOnContactExit(T* origin, void(T::* function)(Collider3D*))
	{
		m_OnContactExit = DBG_NEW CallBack(origin, function);
	}

	void CallOnContactEnter(Collider3D* collider);
	void CallOnContactStay(Collider3D* collider);
	void CallOnContactExit(Collider3D* collider);

	void AddContactList(Collider3D* collider);
	void RemoveRemainElement(Collider3D* collider);

	float CalcDirectionVectorSeparateAxisLength(
		D3DXVECTOR3 a_DirVec, D3DXVECTOR3 a_DirVecNormal,
		D3DXVECTOR3 b_DirVecRight, D3DXVECTOR3 b_DirVecUp, D3DXVECTOR3 b_DirVecForward,
		D3DXVECTOR3 distance);
	float CalcCrossSeparateAxisLength(D3DXVECTOR3 cross,
		D3DXVECTOR3 a_DirVec1, D3DXVECTOR3 a_DirVec2,
		D3DXVECTOR3 b_DirVec1, D3DXVECTOR3 b_DirVec2,
		D3DXVECTOR3 distance);
	float LenSegOnSeparateAxis(D3DXVECTOR3* sep,
		D3DXVECTOR3* edge1, D3DXVECTOR3* edge2, D3DXVECTOR3* edge3 = NULL);
	void SetMinBiteAndMinBiteAxis(
		float bite, D3DXVECTOR3 biteAxis, float& minBite, D3DXVECTOR3& minBiteAxis);

	void SetVelocityRef(D3DXVECTOR3* velocityRef, optional<float> damp);
	void ReflectionVelocityRef(D3DXVECTOR3 base);
	void DeleteVelocityRef();

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//

	ColliderId GetId() { return m_Id; }

	D3DXVECTOR3 GetOffset() { return m_Offset; }
	void SetOffset(D3DXVECTOR3 offset) { m_Offset = offset; }

	bool GetIsTrigger() { return m_IsTrigger; }
	void SetIsTrigger(bool value);

	bool GetIsStatic() { return m_IsStatic; }
	void SetIsStatic(bool value) { m_IsStatic = value; }

	list<Collider3D*> GetContactList() { return m_RemainList; }

	D3DXVECTOR3* GetVelocityRef() { return m_VelocityRef; }
	/////////////////////////////

#ifdef _DEBUG
protected:
	static inline const float m_CHILD_WINDOW_WIDTH = 250.0f;
	static inline const float m_CHILD_WINDOW_BLANK_HEIGHT = 16.0f;
	static inline const float m_CHILD_WINDOW_LINE_HEIGHT = 19.0f;
	static inline const float m_CHILD_WINDOW_HEIGHT_MAX =
		m_CHILD_WINDOW_BLANK_HEIGHT + m_CHILD_WINDOW_LINE_HEIGHT * 6;

public:
	void DrawInspector() override;
#endif
};
