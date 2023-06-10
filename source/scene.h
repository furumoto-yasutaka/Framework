/*******************************************************************************
*
*	タイトル：	シーン基底クラス
*	ファイル：	scene.h
*	作成者：	古本 泰隆
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
	// オブジェクトタグ(ビットフラグ)
	enum class ObjTag
	{
		Default			= 0,
		Player			= 1 << 0,
		Platform		= 1 << 1,
		Enemy			= 1 << 2,
		Everything		= (1 << 3) - 1,
	};

protected:
	// 動的オブジェクトリスト
	unordered_multimap<string, GameObject*> m_GameObject;
	// 静的オブジェクトリスト
	static inline unordered_multimap<string, GameObject*> m_StaticGameObject;

	Camera*	m_ActiveCamera = NULL;	// 現在有効になっているカメラ

public:
	virtual ~Scene() {}

	virtual void Init() = 0;
	void Uninit();
	static void StaticUninit();
	void Update();
	void Draw();

	// 各グループの描画
	void DrawCamera();
	void Draw3d();
	void Draw2d();

	// オブジェクトをシーンに追加
	GameObject* AddGameObject(string name, int tag = (int)ObjTag::Default, bool isStatic = false);
	// オブジェクト・コンポーネント削除確認
	void CheckDestroy();

	// 現在のアクティブカメラとなっているカメラを削除し、代わりのカメラを探す
	void DeleteActiveCamera();

private:
	unordered_multimap<string, GameObject*>::iterator SearchObjectList(GameObject* obj);
	unordered_multimap<string, GameObject*>::iterator SearchStaticObjectList(GameObject* obj);

public:
	/////////////////////////////
	//　↓↓　アクセサ　↓↓　//

	// 全オブジェクト取得
	list<GameObject*> GetGameObjectAll();
	// オブジェクト取得(名称参照)
	GameObject* GetGameObject(string name);
	// オブジェクト全取得(名称参照)
	vector<GameObject*> GetGameObjects(string name);
	// オブジェクト全取得(タグ参照)
	vector<GameObject*> GetGameObjectsFromTag(int tag);

	Camera* GetActiveCamera() { return m_ActiveCamera; }
	void SetActiveCamera(Camera* activeCamera) { m_ActiveCamera = activeCamera; }
	/////////////////////////////

	/////////////////////////////////
	//　↓↓　基本呼出禁止　↓↓　//

	// 静的オブジェクトに変更する
	void SetStatic(GameObject* object);
	// 動的オブジェクトに変更する
	void SetDynamic(GameObject* object);
	// オブジェクトの名前(キー)を変更する
	void SetObjectName(GameObject* object, string newName);
	/////////////////////////////////

#ifdef _DEBUG
public:
	virtual void DrawInspector(ImVec2 pos, ImVec2 size) {}
#endif
};
