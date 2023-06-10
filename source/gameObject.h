/*******************************************************************************
*
*	�^�C�g���F	�Q�[���I�u�W�F�N�g�N���X
*	�t�@�C���F	gameObject.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "component.h"
#include "manager.h"

#include <list>
#include <vector>
#include <typeinfo>
#include <string>

class GameObject
{
public:
	// �e�I�u�W�F�N�g�ݒ莞�̎q�ɑ΂���I�v�V����
	enum class SetParentOption
	{
		Nothing		= 0,
		KeepPos		= 1 << 0,		// ���W���ێ�
		KeepScale	= 1 << 1,		// �g�k���ێ�
		All			= (1 << 2) - 1,
	};

private:
	list<Component*>	m_ComponentList;		// �R���|�[�l���g���X�g

	GameObject*			m_Parent = NULL;		// �e�I�u�W�F�N�g
	list<GameObject*>	m_Child;				// �q�I�u�W�F�N�g

	bool				m_IsActive = true;		// �A�N�e�B�u�t���O
	bool				m_IsDestroy = false;	// �폜�t���O
	bool				m_IsStatic = false;		// �ÓI�t���O
	string				m_Name;					// �I�u�W�F�N�g��
	int					m_Tag;					// �I�u�W�F�N�g�^�O

public:	
	D3DXVECTOR3			m_LocalPosition;		// ���W
	D3DXQUATERNION		m_LocalRotation;		// ��]
	D3DXVECTOR3			m_LocalScale;			// �T�C�Y

public:
	~GameObject() {}

	void Init(string name, int tag);
	void Uninit();

	// �R���|�[�l���g�̍폜�m�F
	void CheckComponentDestroy();

	// ���g�Ƃ��̎q�I�u�W�F�N�g�̍폜��\��
	void SetDestroy();
	// �폜����
	void Destroy();

	// �}�g���N�X���擾
	D3DXMATRIX GetMatrix();
	// ���ʕ����̒P�ʃx�N�g�����擾
	D3DXVECTOR3 GetForward();
	// ������̒P�ʃx�N�g�����擾
	D3DXVECTOR3 GetUp();
	// �E�����̒P�ʃx�N�g�����擾
	D3DXVECTOR3 GetRight();

	/////////////////////////////
	//�@�����@�e�q�֌W�@�����@//
	
	// ���g�̃��[�J���}�g���N�X�̌��_�ɂ����郏�[���h�}�g���N�X(�e�̃��[���h�}�g���N�X)�擾
	D3DXMATRIX GetLocalMatrixOrigin();
	// ���[���h�}�g���N�X�擾
	D3DXMATRIX GetWorldMatrix(D3DXMATRIX matrix = Math::GetMatrixIdentity());

	// ���g�̃��[�J�����W�̌��_�ɂ����郏�[���h���W(�e�̃��[���h���W)�擾
	D3DXVECTOR3 GetLocalPositionOrigin();
	// ���[���h���W�擾
	D3DXVECTOR3 GetWorldPosition(D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	// ���[���h���W�ݒ�
	void SetWorldPosition(D3DXVECTOR3 position);

	// ���g�̃��[�J�����[�e�[�V�����̌��_�ɂ����郏�[���h���[�e�[�V����(�e�̃��[���h���[�e�[�V����)�擾
	D3DXQUATERNION GetLocalRotationOrigin();
	// ���[���h���[�e�[�V�����擾
	D3DXQUATERNION GetWorldRotation(D3DXQUATERNION rotation = Math::GetQuaternionIdentity());
	// ���[���h���[�e�[�V�����ݒ�
	void SetWorldRotation(D3DXQUATERNION rotation);

	// ���g�̃��[�J���X�P�[���̌��_�ɂ����郏�[���h�X�P�[��(�e�̃��[���h�X�P�[��)�擾
	D3DXVECTOR3 GetLocalScaleOrigin();
	// ���[���h�X�P�[���擾
	D3DXVECTOR3 GetWorldScale(D3DXVECTOR3 scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	// ���[���h�X�P�[���ݒ�
	void SetWorldScale(D3DXVECTOR3 scale);
	/////////////////////////////

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	list<Component*> GetComponentAll();

	GameObject* GetRoot();

	GameObject* GetParent() { return m_Parent; }
	void SetParent(GameObject* parent,
		int removeOption = (int)SetParentOption::Nothing,
		int setOption = (int)SetParentOption::Nothing);

	list<GameObject*> GetChildList() { return m_Child; }
	GameObject* GetChild(int index);

	bool GetIsActive();
	bool GetIsActiveSelf() { return m_IsActive; }
	void SetIsActive(bool value) { m_IsActive = value; }

	bool GetIsDestroy() { return m_IsDestroy; }

	bool GetIsStatic() { return m_IsStatic; }
	void SetIsStatic(bool value);

	string GetName() { return m_Name; }
	void SetName(string newName);

	int GetTag() { return m_Tag; }
	void AddTag(int tag) { m_Tag |= tag; }
	/////////////////////////////

private:
	// ���ʕ����̒P�ʃx�N�g�����擾
	D3DXVECTOR3 GetForwardInverse();
	// ������̒P�ʃx�N�g�����擾
	D3DXVECTOR3 GetUpInverse();
	// �E�����̒P�ʃx�N�g�����擾
	D3DXVECTOR3 GetRightInverse();

	void CheckRemoveOption(int option, GameObject* parent);
	void CheckSetOption(int option, GameObject* parent);

public:
	/*******************************************************************************
	*	�R���|�[�l���g�ǉ�
	*******************************************************************************/
	template<class T>
	T* AddComponent()
	{
		T* component = DBG_NEW T(this);

		m_ComponentList.push_back(component);

#ifdef _DEBUG
		component->InitDebug();

		if (component->GetDrawGroup() == Component::DrawGroup::Draw2D)
		{
			m_AttachDraw2dComponentCount++;
		}
#endif
		return component;
	}

	/*******************************************************************************
	*	�R���|�[�l���g�擾(�N���X���Q��)
	*******************************************************************************/
	template<class T>
	T* GetComponent()
	{
		auto itr = find_if(m_ComponentList.begin(), m_ComponentList.end(),
			[](Component* c) { return typeid(*c) == typeid(T); });

		if (itr != m_ComponentList.end())
		{
			return reinterpret_cast<T*>((*itr));
		}
		else
		{
			return NULL;
		}
	}

	/*******************************************************************************
	*	�R���|�[�l���g�S�擾(�N���X���Q��)
	*******************************************************************************/
	template<class T>
	vector<T*> GetComponents()
	{
		vector<T*> components;
		for (Component* c : m_ComponentList)
		{
			if (typeid(*c) == typeid(T))
			{
				components.push_back(c);
			}
		}
		return components;
	}

	/////////////////////////////////
	//�@�����@��{�ďo�֎~�@�����@//
	// �q�I�u�W�F�N�g��ǉ��@����{�ďo�֎~
	void AddChild(GameObject* child) { m_Child.push_back(child); }
	// �q�I�u�W�F�N�g���폜�@����{�ďo�֎~
	void RemoveChild(GameObject* child) { m_Child.remove(child); }
	/////////////////////////////////

#ifdef _DEBUG
private:
	bool m_IsPreDestroy = false;
	int	m_AttachDraw2dComponentCount = 0; // Draw2D�R���|�[�l���g�̃A�^�b�`��

	ID3D11Buffer* m_VertexBuffer = NULL;		// ���_�o�b�t�@
	ID3D11VertexShader* m_VertexShader = NULL;	// ���_�V�F�[�_�[
	ID3D11PixelShader* m_PixelShader = NULL;	// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout* m_VertexLayout = NULL;	// ���̓��C�A�E�g

public:
	void DrawInspector();
	void InitDebug();
	void UninitDebug();
	void DrawDebugParentLine2d();
	void DrawDebugParentLine3d();
private:
	void DrawDebugParentLine();
	void CreateVertex(VERTEX_3D* vertex);
#endif
};
