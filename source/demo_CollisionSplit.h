/*******************************************************************************
*
*	タイトル：	8分木空間分割デモシーン
*	ファイル：	demo_CollisionSplit.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "scene.h"
#include "gameObject.h"
#include "collision3dManager.h"

// コンポーネント
#include "camera.h"
#include "plate2dRenderer.h"
#include "plate3dRenderer.h"
#include "aabbCollider3d.h"

// スクリプト
#include "testMove.h"
#include "debugCameraControl.h"

class Demo_CollisionSplit : public Scene
{
public:
	~Demo_CollisionSplit() {}

	void Init() override
	{
		GameObject* obj;


		obj = AddGameObject("Telop");
		obj->m_LocalPosition = { 0.0f, 570.0f, 0.0f };
		obj->AddComponent<Plate2DRenderer>()->Init("Telop_CollisionSplit", { 650.0f, 300.0f });


		// コリジョン
		D3DXVECTOR3 colSize = { 1.0f, 1.0f, 1.0f };

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 5.0f, 5.0f, 5.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 10.0f, 0.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { -35.0f, 10.0f, -30.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ -10.0f, 0.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { -20.0f, -10.0f, 35.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 10.0f, 0.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 5.0f, 30.0f, -15.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ -10.0f, 0.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 20.0f, 5.0f, 15.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 10.0f, 0.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 24.0f, 5.0f, 15.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ -10.0f, 0.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { -35.0f, -10.0f, -20.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 10.0f, 0.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 10.0f, -10.0f, 5.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ -10.0f, 0.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 5.0f, 5.0f, 15.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 10.0f, 0.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 30.0f, 30.0f, 30.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ -10.0f, 0.0f, 0.0f });


		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { -30.0f, -35.0f, 5.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 10.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 20.0f, 35.0f, 35.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, -10.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 10.0f, 20.0f, -35.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 10.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { -10.0f, -30.0f, 20.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, -10.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 35.0f, -35.0f, 35.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 10.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { -30.0f, -30.0f, 30.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, -10.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 20.0f, 20.0f, 20.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 10.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { -10.0f, 30.0f, -40.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, -10.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { -20.0f, 30.0f, -20.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 10.0f, 0.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 30.0f, -15.0f, 20.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, -10.0f, 0.0f });


		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 30.0f, -20.0f, -10.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 0.0f, 10.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 35.0f, 20.0f, 40.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 0.0f, -10.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { -10.0f, 5.0f, -35.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 0.0f, 10.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { -35.0f, 5.0f, -10.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 0.0f, -10.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 5.0f, -35.0f, 5.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 0.0f, 10.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 5.0f, 5.0f, -10.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 0.0f, -10.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 5.0f, 5.0f, 30.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 0.0f, 10.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { -35.0f, 35.0f, 35.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 0.0f, -10.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { -35.0f, 5.0f, 5.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 0.0f, 10.0f });

		obj = AddGameObject("Collision");
		obj->m_LocalPosition = { 5.0f, 30.0f, 5.0f };
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, colSize, true);
		obj->AddComponent<TestMove>()->Init({ 0.0f, 0.0f, -10.0f });


		// カメラ
		obj = AddGameObject("Camera");
		obj->m_LocalPosition = { -40.0f, 10.0f, -50.0f };
		obj->m_LocalRotation = Math::AxisRotation_Degree({ 1.0f, 0.0f, 0.0f }, 20.0f);
		obj->m_LocalRotation = Math::AxisRotation_Degree({ 0.0f, 1.0f, 0.0f }, 25.0f);
		obj->AddComponent<Camera>()->Init(true);
		obj->AddComponent<DebugCameraControl>()->Init();


		Collision3DManager::ResetSpace(0, Collision3DManager::GetMin(), Collision3DManager::GetSize());
	}

#ifdef _DEBUG
	int m_SpritLevel = 0;
	vector<string> m_Items = {
		"0",
		"1",
		"2",
		"3",
	};

	void DrawInspector(ImVec2 pos, ImVec2 size) override
	{
		if (DebugManager::ComboBox("SplitLevel", m_SpritLevel, m_Items))
		{
			Collision3DManager::ResetSpace(m_SpritLevel, Collision3DManager::GetMin(), Collision3DManager::GetSize());
		}
		
		ImGui::Text("Collision : %d", Collision3DManager::GetCollisionCount());
	}
#endif
};
