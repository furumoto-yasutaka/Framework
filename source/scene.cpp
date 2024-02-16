/*******************************************************************************
*
*	�^�C�g���F	�V�[�����N���X
*	�t�@�C���F	scene.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "scene.h"
#include "gameObject.h"
#include "draw2dComponent.h"
#include "modelContainer.h"
#include "textureContainer.h"
#include "audioContainer.h"
#include "skyDome.h"
#include "collision3dManager.h"
#include "camera.h"

#include <typeinfo>
#include <algorithm>

#ifdef _DEBUG
#include "debugManager.h"
#endif

void Scene::Uninit()
{
	auto itr = m_GameObject.begin();
	while (itr != m_GameObject.end())
	{
#ifdef _DEBUG
		itr->second->UninitDebug();
#endif
		itr->second->Uninit();
		delete itr->second;
		itr = m_GameObject.erase(itr);
	}
}

void Scene::StaticUninit()
{
	auto itr = m_StaticGameObject.begin();
	while (itr != m_StaticGameObject.end())
	{
#ifdef _DEBUG
		itr->second->UninitDebug();
#endif
		itr->second->Uninit();
		delete itr->second;
		itr = m_StaticGameObject.erase(itr);
	}
}

void Scene::Update()
{
	// ��������R���|�[�l���g��LateInit�Ăяo��
	for (auto itr : Component::GetCreateOrderComponents())
	{
		itr->ChangeOrderList();
	}

	// UpdateDebug�EUpdate
	for (auto itr : Component::GetUpdatePriorityOrderComponents())
	{
		if (itr.second->GetIsActive())
		{
			itr.second->Update();
			itr.second->UpdateDebug();
		}
	}

	// LateUpdate�ECheckInvoke
	for (auto itr : Component::GetUpdatePriorityOrderComponents())
	{
		if (itr.second->GetIsActive())
		{
			itr.second->LateUpdate();
			itr.second->CheckInvoke();
		}
	}

	// �`��̃^�C�~���O�ɍ��킹�ăV�[���̌�ɍs��
	//Transition::Update();
}

void Scene::Draw()
{
	DrawCamera();
	Draw3d();
	Draw2d();
}

/*******************************************************************************
*	�J�����ݒ�
*******************************************************************************/
void Scene::DrawCamera()
{
	// ActiveCamera
	if (m_ActiveCamera)
	{
		if (m_ActiveCamera->GetIsActive())
		{
			m_ActiveCamera->DrawView();
		}
	}


	// Camera
	for (auto itr : CameraComponent::GetGroupComponents())
	{
		if (itr->GetIsActive())
		{
			itr->DrawDebug3d();
		}
	}
}

/*******************************************************************************
*	3D�`��
*******************************************************************************/
void Scene::Draw3d()
{
	// 2DDraw�������Ȃ��Q�[���I�u�W�F�N�g
	for (auto itr : m_GameObject)
	{
		if (itr.second->GetIsActive())
		{
			itr.second->DrawDebugParentLine3d();
		}
	}
	for (auto itr : m_StaticGameObject)
	{
		if (itr.second->GetIsActive())
		{
			itr.second->DrawDebugParentLine3d();
		}
	}

	// Draw3D
	for (auto itr : Draw3DComponent::GetGroupComponents())
	{
		if (itr->GetIsActive())
		{
			itr->Draw3d();
			itr->DrawDebug3d();
		}
	}

	//SkyDome::Draw();
	Collision3DManager::Draw();
}

/*******************************************************************************
*	2D�`��
*******************************************************************************/
void Scene::Draw2d()
{
	Renderer::SetWorldViewProjection2D();

	// 2DDraw�����Q�[���I�u�W�F�N�g
	for (auto itr : m_GameObject)
	{
		if (itr.second->GetIsActive())
		{
			itr.second->DrawDebugParentLine2d();
		}
	}
	for (auto itr : m_StaticGameObject)
	{
		if (itr.second->GetIsActive())
		{
			itr.second->DrawDebugParentLine2d();
		}
	}

	// Draw2D
	for (auto itr : Draw2DComponent::GetDrawPriorityOrderComponents())
	{
		if (itr.second->GetIsActive())
		{
			itr.second->Draw2d();
			itr.second->DrawDebug2d();
		}
	}

	// �g�����W�V��������Ԏ�O�łȂ���΂Ȃ�Ȃ����߂��̃^�C�~���O
	//Transition::Draw();
}

/*******************************************************************************
*	�I�u�W�F�N�g���V�[���ɒǉ�
*******************************************************************************/
GameObject* Scene::AddGameObject(string name, int tag, bool isStatic)
{
	GameObject* gameObject = DBG_NEW GameObject();

	// ���X�g�ɒǉ����A�C�e���[�^��ۑ�
	if (isStatic)
	{// �ÓI�I�u�W�F�N�g�Ƃ��Ēǉ�
		m_StaticGameObject.insert({ name, gameObject });
	}
	else
	{// ���I�I�u�W�F�N�g�Ƃ��Ēǉ�
		m_GameObject.insert({ name, gameObject });
	}

	gameObject->Init(name, tag);
#ifdef _DEBUG
	gameObject->InitDebug();
#endif

	return gameObject;
}

/*******************************************************************************
*	�I�u�W�F�N�g�E�R���|�[�l���g�폜�m�F
*******************************************************************************/
void Scene::CheckDestroy()
{
	auto itr = m_GameObject.begin();
	while (itr != m_GameObject.end())
	{
		if (itr->second->GetIsDestroy())
		{
			itr->second->Destroy();
			itr = m_GameObject.erase(itr);
		}
		else
		{
			itr->second->CheckComponentDestroy();
			itr++;
		}
	}

	itr = m_StaticGameObject.begin();
	while (itr != m_StaticGameObject.end())
	{
		if (itr->second->GetIsDestroy())
		{
			itr->second->Destroy();
			itr = m_StaticGameObject.erase(itr);
		}
		else
		{
			itr->second->CheckComponentDestroy();
			itr++;
		}
	}
}

/*******************************************************************************
*	�J�����I�u�W�F�N�g���폜���A����̃J����������
*******************************************************************************/
void Scene::DeleteActiveCamera()
{
	list<Camera*> cameraList = Camera::GetCameraList();

	auto itr = find_if(cameraList.begin(), cameraList.end(),
		[](Camera* c) { return c->GetIsActive(); });

	if (itr != cameraList.end())
	{
		m_ActiveCamera = (*itr);
	}
	else
	{
		m_ActiveCamera = NULL;
	}
}

/*******************************************************************************
*	�}�b�v������
*******************************************************************************/
unordered_multimap<string, GameObject*>::iterator Scene::SearchObjectList(GameObject* obj)
{
	auto range = m_GameObject.equal_range(obj->GetName());
	auto itr = find_if(range.first, range.second,
		[&](pair<string, GameObject*> pair) { return pair.second == obj; });

	if (itr != range.second)
	{
		return itr;
	}
	else
	{
		return m_GameObject.end();
	}
}
unordered_multimap<string, GameObject*>::iterator Scene::SearchStaticObjectList(GameObject* obj)
{
	auto range = m_StaticGameObject.equal_range(obj->GetName());
	auto itr = find_if(range.first, range.second,
		[&](pair<string, GameObject*> pair) { return pair.second == obj; });

	if (itr != range.second)
	{
		return itr;
	}
	else
	{
		return m_StaticGameObject.end();
	}
}

#pragma region GetGameObject
/*******************************************************************************
*	�S�I�u�W�F�N�g�擾
*******************************************************************************/
list<GameObject*> Scene::GetGameObjectAll()
{
	list<GameObject*> list;

	for (auto itr : m_GameObject)
	{
		list.push_back(itr.second);
	}
	for (auto itr : m_StaticGameObject)
	{
		list.push_back(itr.second);
	}

	return list;
}

/*******************************************************************************
*	�I�u�W�F�N�g�擾(���̎Q��)
*******************************************************************************/
GameObject* Scene::GetGameObject(string name)
{
	// ���I�I�u�W�F�N�g�Ŗ��O����v����C�e���[�^�����ׂĎ擾
	auto range = m_GameObject.equal_range(name);
	// �P�ł�����������ŏ��̃C�e���[�^�̗v�f��Ԃ�
	if (range.first != m_GameObject.end())
	{
		return range.first->second;
	}

	// �ÓI�I�u�W�F�N�g�Ŗ��O����v����C�e���[�^�����ׂĎ擾
	range = m_StaticGameObject.equal_range(name);
	// �P�ł�����������ŏ��̃C�e���[�^�̗v�f��Ԃ�
	if (range.first != m_StaticGameObject.end())
	{
		return range.first->second;
	}
	
	return NULL;
}

/*******************************************************************************
*	�I�u�W�F�N�g�S�擾(���̎Q��)
*******************************************************************************/
vector<GameObject*> Scene::GetGameObjects(string name)
{
	vector<GameObject*> objects;

	// ���I�I�u�W�F�N�g�Ŗ��O����v����C�e���[�^�����ׂĎ擾
	auto range = m_GameObject.equal_range(name);
	// �����������ׂĂ̗v�f�����X�g�Ɋi�[����
	auto itr = range.first;
	while (itr != m_GameObject.end())
	{
		objects.push_back(itr->second);
		itr++;
	}

	// �ÓI�I�u�W�F�N�g�Ŗ��O����v����C�e���[�^�����ׂĎ擾
	range = m_StaticGameObject.equal_range(name);
	// �����������ׂĂ̗v�f�����X�g�Ɋi�[����
	itr = range.first;
	while (itr != m_StaticGameObject.end())
	{
		objects.push_back(itr->second);
		itr++;
	}

	return objects;
}

/*******************************************************************************
*	�I�u�W�F�N�g�S�擾(�^�O�Q��)
*******************************************************************************/
vector<GameObject*> Scene::GetGameObjectsFromTag(int tag)
{
	vector<GameObject*> objects;

	// �^�O����v�������ׂẴI�u�W�F�N�g��Ԃ�
	for (auto itr : m_GameObject)
	{
		if (itr.second->GetTag() & tag)
		{
			objects.push_back(itr.second);
		}
	}
	for (auto itr : m_StaticGameObject)
	{
		if (itr.second->GetTag() & tag)
		{
			objects.push_back(itr.second);
		}
	}

	return objects;
}
#pragma endregion

/*******************************************************************************
*	�ÓI�I�u�W�F�N�g�ɕύX����
*******************************************************************************/
void Scene::SetStatic(GameObject* object)
{
	auto itr = SearchObjectList(object);

	if (itr != m_GameObject.end())
	{
		// �Y������I�u�W�F�N�g��ÓI�I�u�W�F�N�g�Ɉړ�����
		m_GameObject.erase(itr);

		m_StaticGameObject.insert({ object->GetName(), object });
	}
}

/*******************************************************************************
*	���I�I�u�W�F�N�g�ɕύX����
*******************************************************************************/
void Scene::SetDynamic(GameObject* object)
{
	auto itr = SearchStaticObjectList(object);

	if (itr != m_StaticGameObject.end())
	{
		// �Y������I�u�W�F�N�g�𓮓I�I�u�W�F�N�g�Ɉړ�����
		m_StaticGameObject.erase(itr);

		m_GameObject.insert({ object->GetName(), object });
	}
}

/*******************************************************************************
*	�I�u�W�F�N�g�̖��O(�L�[)��ύX����
*******************************************************************************/
void Scene::SetObjectName(GameObject* object, string newName)
{
	if (object->GetIsStatic())
	{// �ÓI�I�u�W�F�N�g
		auto itr = SearchStaticObjectList(object);

		if (itr != m_StaticGameObject.end())
		{
			m_StaticGameObject.erase(itr);
			m_StaticGameObject.insert({ newName, object });
		}
	}
	else
	{// ���I�I�u�W�F�N�g
		auto itr = SearchObjectList(object);

		if (itr != m_GameObject.end())
		{
			m_GameObject.erase(itr);
			m_GameObject.insert({ newName, object });
		}
	}
}
