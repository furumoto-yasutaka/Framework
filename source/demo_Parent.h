/*******************************************************************************
*
*	タイトル：	Invoke紹介シーンクラス
*	ファイル：	demo_Parent.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "scene.h"
#include "gameObject.h"

// コンポーネント
#include "camera.h"
#include "aabbCollider3d.h"
#include "obbCollider3d.h"
#include "sphereCollider3d.h"
#include "capsuleCollider3d.h"
#include "skyDome.h"

// スクリプト
#include "testInvoke.h"
#include "debugCameraControl.h"
#include "modelRenderer.h"
#include "testScript.h"
#include "testScript2.h"

class Demo_Parent : public Scene
{
public:
	~Demo_Parent() {}

	void Init() override
	{
		GameObject* obj;
		GameObject* parent;
		GameObject* children;

		// スカイドーム
		obj = AddGameObject("SkyDome");
		obj->AddComponent<SkyDome>()->Init();

		obj = AddGameObject("Camera");
		obj->AddComponent<Camera>()->Init(true);
		//obj->AddComponent<DebugCameraControl>()->Init();



		// コリジョンフレーム
		obj = AddGameObject("TestAABB");
		obj->m_LocalPosition = D3DXVECTOR3(-6.0f, -3.0f, 10.0f);
		obj->AddComponent<ModelRenderer>()->Init("Coconut", "toonVS", "toonPS");
		obj->AddComponent<TestScript>()->Init();
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });


		obj = AddGameObject("TestOBB");
		obj->m_LocalPosition = D3DXVECTOR3(-2.0f, -3.0f, 10.0f);
		obj->AddComponent<ModelRenderer>()->Init("Coconut", "toonVS", "toonPS");
		obj->AddComponent<TestScript>()->Init();
		obj->AddComponent<OBBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });


		obj = AddGameObject("TestSphere");
		obj->m_LocalPosition = D3DXVECTOR3(2.0f, -3.0f, 10.0f);
		obj->AddComponent<ModelRenderer>()->Init("Coconut", "toonVS", "toonPS");
		obj->AddComponent<TestScript>()->Init();
		obj->AddComponent<SphereCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, 1.0f);


		obj = AddGameObject("TestCapsule");
		obj->m_LocalPosition = D3DXVECTOR3(6.0f, -3.0f, 10.0f);
		obj->AddComponent<ModelRenderer>()->Init("Coconut", "toonVS", "toonPS");
		obj->AddComponent<TestScript>()->Init();
		obj->AddComponent<CapsuleCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, 1.0f, 3.0f, CapsuleCollider3D::DirectionPattern::Y);



		// ペアレント
		parent = AddGameObject("parent");
		parent->m_LocalPosition = D3DXVECTOR3(0.0f, 1.5f, 10.0f);
		parent->AddComponent<ModelRenderer>()->Init("Coconut", "toonVS", "toonPS");
		parent->AddComponent<TestScript>()->Init();
		parent->AddComponent<OBBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, { 0.6f, 0.6f, 0.6f });


		children = AddGameObject("children");
		children->m_LocalPosition = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
		children->AddComponent<ModelRenderer>()->Init("Coconut", "toonVS", "toonPS");
		children->AddComponent<TestScript2>()->Init();
		children->AddComponent<OBBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, { 0.6f, 0.6f, 0.6f });
		children->SetParent(parent);


		obj = AddGameObject("children2");
		obj->m_LocalPosition = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		obj->AddComponent<ModelRenderer>()->Init("Coconut", "toonVS", "toonPS");
		obj->AddComponent<OBBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, { 0.6f, 0.6f, 0.6f });
		obj->SetParent(children);
	}
};
