/*******************************************************************************
*
*	タイトル：	シーン基底クラス
*	ファイル：	scene.cpp
*	作成者：	古本 泰隆
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
	// 生成直後コンポーネントのLateInit呼び出し
	for (auto itr : Component::GetCreateOrderComponents())
	{
		itr->ChangeOrderList();
	}

	// UpdateDebug・Update
	for (auto itr : Component::GetUpdatePriorityOrderComponents())
	{
		if (itr.second->GetIsActive())
		{
			itr.second->Update();
			itr.second->UpdateDebug();
		}
	}

	// LateUpdate・CheckInvoke
	for (auto itr : Component::GetUpdatePriorityOrderComponents())
	{
		if (itr.second->GetIsActive())
		{
			itr.second->LateUpdate();
			itr.second->CheckInvoke();
		}
	}

	// 描画のタイミングに合わせてシーンの後に行う
	//Transition::Update();
}

void Scene::Draw()
{
	DrawCamera();
	Draw3d();
	Draw2d();
}

/*******************************************************************************
*	カメラ設定
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
*	3D描画
*******************************************************************************/
void Scene::Draw3d()
{
	// 2DDrawを持たないゲームオブジェクト
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
*	2D描画
*******************************************************************************/
void Scene::Draw2d()
{
	Renderer::SetWorldViewProjection2D();

	// 2DDrawを持つゲームオブジェクト
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

	// トランジションが一番手前でなければならないためこのタイミング
	//Transition::Draw();
}

/*******************************************************************************
*	オブジェクトをシーンに追加
*******************************************************************************/
GameObject* Scene::AddGameObject(string name, int tag, bool isStatic)
{
	GameObject* gameObject = DBG_NEW GameObject();

	// リストに追加し、イテレータを保存
	if (isStatic)
	{// 静的オブジェクトとして追加
		m_StaticGameObject.insert({ name, gameObject });
	}
	else
	{// 動的オブジェクトとして追加
		m_GameObject.insert({ name, gameObject });
	}

	gameObject->Init(name, tag);
#ifdef _DEBUG
	gameObject->InitDebug();
#endif

	return gameObject;
}

/*******************************************************************************
*	オブジェクト・コンポーネント削除確認
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
*	カメラオブジェクトを削除し、代わりのカメラを検索
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
*	マップを検索
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
*	全オブジェクト取得
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
*	オブジェクト取得(名称参照)
*******************************************************************************/
GameObject* Scene::GetGameObject(string name)
{
	// 動的オブジェクトで名前が一致するイテレータをすべて取得
	auto range = m_GameObject.equal_range(name);
	// １つでも見つかったら最初のイテレータの要素を返す
	if (range.first != m_GameObject.end())
	{
		return range.first->second;
	}

	// 静的オブジェクトで名前が一致するイテレータをすべて取得
	range = m_StaticGameObject.equal_range(name);
	// １つでも見つかったら最初のイテレータの要素を返す
	if (range.first != m_StaticGameObject.end())
	{
		return range.first->second;
	}
	
	return NULL;
}

/*******************************************************************************
*	オブジェクト全取得(名称参照)
*******************************************************************************/
vector<GameObject*> Scene::GetGameObjects(string name)
{
	vector<GameObject*> objects;

	// 動的オブジェクトで名前が一致するイテレータをすべて取得
	auto range = m_GameObject.equal_range(name);
	// 見つかったすべての要素をリストに格納する
	auto itr = range.first;
	while (itr != m_GameObject.end())
	{
		objects.push_back(itr->second);
		itr++;
	}

	// 静的オブジェクトで名前が一致するイテレータをすべて取得
	range = m_StaticGameObject.equal_range(name);
	// 見つかったすべての要素をリストに格納する
	itr = range.first;
	while (itr != m_StaticGameObject.end())
	{
		objects.push_back(itr->second);
		itr++;
	}

	return objects;
}

/*******************************************************************************
*	オブジェクト全取得(タグ参照)
*******************************************************************************/
vector<GameObject*> Scene::GetGameObjectsFromTag(int tag)
{
	vector<GameObject*> objects;

	// タグが一致したすべてのオブジェクトを返す
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
*	静的オブジェクトに変更する
*******************************************************************************/
void Scene::SetStatic(GameObject* object)
{
	auto itr = SearchObjectList(object);

	if (itr != m_GameObject.end())
	{
		// 該当するオブジェクトを静的オブジェクトに移動する
		m_GameObject.erase(itr);

		m_StaticGameObject.insert({ object->GetName(), object });
	}
}

/*******************************************************************************
*	動的オブジェクトに変更する
*******************************************************************************/
void Scene::SetDynamic(GameObject* object)
{
	auto itr = SearchStaticObjectList(object);

	if (itr != m_StaticGameObject.end())
	{
		// 該当するオブジェクトを動的オブジェクトに移動する
		m_StaticGameObject.erase(itr);

		m_GameObject.insert({ object->GetName(), object });
	}
}

/*******************************************************************************
*	オブジェクトの名前(キー)を変更する
*******************************************************************************/
void Scene::SetObjectName(GameObject* object, string newName)
{
	if (object->GetIsStatic())
	{// 静的オブジェクト
		auto itr = SearchStaticObjectList(object);

		if (itr != m_StaticGameObject.end())
		{
			m_StaticGameObject.erase(itr);
			m_StaticGameObject.insert({ newName, object });
		}
	}
	else
	{// 動的オブジェクト
		auto itr = SearchObjectList(object);

		if (itr != m_GameObject.end())
		{
			m_GameObject.erase(itr);
			m_GameObject.insert({ newName, object });
		}
	}
}
