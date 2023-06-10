/*******************************************************************************
*
*	�^�C�g���F	�V�[�����N���X
*	�t�@�C���F	scene.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <unordered_map>
#include <list>
#include <vector>
#include <string>

class Camera;
class GameObject;

class Scene
{
public:
	// �I�u�W�F�N�g�^�O(�r�b�g�t���O)
	enum class ObjTag
	{
		Default			= 0,
		Player			= 1 << 0,
		Platform		= 1 << 1,
		Enemy			= 1 << 2,
		Everything		= (1 << 3) - 1,
	};

protected:
	// ���I�I�u�W�F�N�g���X�g
	unordered_multimap<string, GameObject*> m_GameObject;
	// �ÓI�I�u�W�F�N�g���X�g
	static inline unordered_multimap<string, GameObject*> m_StaticGameObject;

	Camera*	m_ActiveCamera = NULL;	// ���ݗL���ɂȂ��Ă���J����

public:
	virtual ~Scene() {}

	virtual void Init() = 0;
	void Uninit();
	static void StaticUninit();
	void Update();
	void Draw();

	// �e�O���[�v�̕`��
	void DrawCamera();
	void Draw3d();
	void Draw2d();

	// �I�u�W�F�N�g���V�[���ɒǉ�
	GameObject* AddGameObject(string name, int tag = (int)ObjTag::Default, bool isStatic = false);
	// �I�u�W�F�N�g�E�R���|�[�l���g�폜�m�F
	void CheckDestroy();

	// ���݂̃A�N�e�B�u�J�����ƂȂ��Ă���J�������폜���A����̃J������T��
	void DeleteActiveCamera();

private:
	unordered_multimap<string, GameObject*>::iterator SearchObjectList(GameObject* obj);
	unordered_multimap<string, GameObject*>::iterator SearchStaticObjectList(GameObject* obj);

public:
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//

	// �S�I�u�W�F�N�g�擾
	list<GameObject*> GetGameObjectAll();
	// �I�u�W�F�N�g�擾(���̎Q��)
	GameObject* GetGameObject(string name);
	// �I�u�W�F�N�g�S�擾(���̎Q��)
	vector<GameObject*> GetGameObjects(string name);
	// �I�u�W�F�N�g�S�擾(�^�O�Q��)
	vector<GameObject*> GetGameObjectsFromTag(int tag);

	Camera* GetActiveCamera() { return m_ActiveCamera; }
	void SetActiveCamera(Camera* activeCamera) { m_ActiveCamera = activeCamera; }
	/////////////////////////////

	/////////////////////////////////
	//�@�����@��{�ďo�֎~�@�����@//

	// �ÓI�I�u�W�F�N�g�ɕύX����
	void SetStatic(GameObject* object);
	// ���I�I�u�W�F�N�g�ɕύX����
	void SetDynamic(GameObject* object);
	// �I�u�W�F�N�g�̖��O(�L�[)��ύX����
	void SetObjectName(GameObject* object, string newName);
	/////////////////////////////////

#ifdef _DEBUG
public:
	virtual void DrawInspector(ImVec2 pos, ImVec2 size) {}
#endif
};
