/*******************************************************************************
*
*	�^�C�g���F	�^�C�g���V�[���N���X
*	�t�@�C���F	test.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "scene.h"
#include "gameObject.h"

// �R���|�[�l���g
#include "camera.h"
#include "Plate2dRenderer.h"
#include "modelRenderer.h"
#include "thirdPersonCamera.h"
#include "particleSystem.h"
#include "particleEmitChecker.h"
#include "skyDome.h"

// �X�N���v�g
#include "debugCameraControl.h"
#include "testScript.h"
#include "testScript2.h"
#include "playerManager.h"
#include "eggEnemyManager.h"
#include "collisionContactRecorder.h"
#include "collisionContactRecorder_PlayerGround.h"
#include "collisionContactRecorder_PlayerAttack.h"
#include "collisionContactRecorder_EnemyAttack.h"
#include "playerHpManager.h"

class Test : public Scene
{
public:
	~Test() {}

	void Init() override
	{
		GameObject* obj;
		GameObject* player;

		// �X�J�C�h�[��
		obj = AddGameObject("SkyDome");
		obj->AddComponent<SkyDome>()->Init();

		player = CreatePlayer();
		CreateEnemy({ 0.0f, 0.0f, 0.0f });


		// �J����
		obj = AddGameObject("Camera");
		obj->AddComponent<ThirdPersonCamera>()->Init(true,
			ThirdPersonCamera::InputModeId::Mouse, player, { 0.0f, 1.0f, -7.0f });

		obj = AddGameObject("DebugCamera");
		obj->AddComponent<Camera>()->Init(false);
		obj->AddComponent<DebugCameraControl>()->Init();

		// �n��
		obj = AddGameObject("Platform1", (int)Scene::ObjTag::Platform);
		obj->m_LocalPosition = { 0.0f, -5.0f, 0.0f };
		obj->m_LocalScale = { 100.0f, 1.0f, 100.0f };
		obj->AddComponent<ModelRenderer>()->Init("Platform_Cube");
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, false, true);

		// �n��
		obj = AddGameObject("Platform2", (int)Scene::ObjTag::Platform);
		obj->m_LocalPosition = { 20.0f, -3.0f, 0.0f };
		obj->m_LocalScale = { 10.0f, 1.0f, 10.0f };
		obj->AddComponent<ModelRenderer>()->Init("Platform_Cube");
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, false, true);
	}

	GameObject* CreatePlayer()
	{
		// ���v���C���[���[�g				Player
		// �����n�ʐڒn����p�R���W����		Player_GroundCollision
		// �������f�����[�g					Player_Model_Loot
		// �@�������ʋ����f��				Player_Model_Sphere
		// �@�����J�o�[���f��				Player_Model_Cover
		// 	 �@��������ڑ������f��			Player_Model_WeaponConnection
		// 	 �@�@�������풆�S�I�u�W�F�N�g	Player_Model_WeaponCenter
		// 	   �@�@�������탂�f��			Player_Model_Weapon
		// �@�@�@�@�@�����_�ɂ��������G	Player_StickEnemyList

		GameObject* player;
		GameObject* parent;
		GameObject* obj;

		// �v���C���[
		player = AddGameObject("Player", (int)ObjTag::Player);
		player->m_LocalPosition = { 0.0f, 0.0f, -30.0f };
		player->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.5f });
		player->AddComponent<PlayerManager>()->Init(PlayerManager::InputModeId::MouseKeyboard);

		obj = AddGameObject("Player_GroundCollision");
		obj->SetParent(player);
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, -0.6f, 0.0f }, { 0.45f, 0.05f, 0.45f }, true);
		obj->AddComponent<CollisionContactRecorder_PlayerGround<AABBCollider3D>>()->Init((int)Scene::ObjTag::Platform);

		obj = AddGameObject("Player_Model_Loot");
		obj->SetParent(player);

		parent = obj;

		obj = AddGameObject("Player_Model_Sphere");
		obj->SetParent(parent);
		obj->AddComponent<ModelRenderer>()->Init("Player_Sphere", "toonVS", "toonPS");

		obj = AddGameObject("Player_Model_Cover");
		obj->SetParent(parent);
		obj->AddComponent<ModelRenderer>()->Init("Player_Cover", "toonVS", "toonPS");

		parent = obj;

		obj = AddGameObject("Player_Model_WeaponConnection");
		obj->SetParent(parent);
		obj->m_LocalPosition = { 0.0f, 0.5f, 0.0f };
		obj->AddComponent<ModelRenderer>()->Init("Player_WeaponConnection", "toonVS", "toonPS");

		parent = obj;

		obj = AddGameObject("Player_Model_WeaponCenter");
		obj->SetParent(parent);
		obj->m_LocalPosition = { 0.0f, 0.2f, 0.0f };

		parent = obj;

		obj = AddGameObject("Player_Model_Weapon");
		obj->SetParent(parent);
		obj->AddComponent<OBBCollider3D>()->Init({ 0.0f, 0.0f, -2.5f }, { 0.1f, 0.2f, 2.5f }, true);
		obj->AddComponent<CollisionContactRecorder_PlayerAttack<OBBCollider3D>>()->Init((int)Scene::ObjTag::Enemy);
		obj->AddComponent<ModelRenderer>()->Init("Player_Weapon", "toonVS", "toonPS");

		parent = obj;

		obj = AddGameObject("Player_StickEnemyList");
		obj->SetParent(parent);

		// �v���C���[HP
		parent = AddGameObject(PlayerHpManager::m_PLAYER_HP_MANAGER_OBJ_NAME);
		parent->m_LocalPosition = { 10.0f, 10.0f, 0.0f };
		parent->AddComponent<PlayerHpManager>()->Init();


		return player;
	}

	GameObject* CreateEnemy(D3DXVECTOR3 Position)
	{
		GameObject* enemy;
		GameObject* parent;
		GameObject* obj;

		OBBCollider3D* obbColider;

		// �����܂�
		// ���G�l�~�[���[�g					Enemy_Egg
		// �����ڒn����p�R���W����			Enemy_GroundCollision
		// �����T�m����p�R���W����			Enemy_SearchCollision
		// �����U������p�R���W����			Enemy_AttackCollision
		// �����W�����v����p�R���W����		Enemy_JumpCollision
		// �������f�����[�g					Enemy_Model_Loot
		// �@�������܂����f��				Enemy_Model_Egg

		enemy = AddGameObject("Enemy_Egg", (int)ObjTag::Enemy);
		enemy->m_LocalPosition = Position;
		enemy->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.2f, 0.0f }, { 0.5f, 0.75f, 0.5f });
		enemy->AddComponent<EggEnemyManager>()->Init();

		obj = AddGameObject("Enemy_GroundCollision");
		obj->SetParent(enemy);
		obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, -0.6f, 0.0f }, { 0.45f, 0.05f, 0.45f }, true);
		obj->AddComponent<CollisionContactRecorder<AABBCollider3D>>()->Init((int)Scene::ObjTag::Platform);

		obj = AddGameObject("Enemy_SearchCollision");
		obj->SetParent(enemy);
		obbColider = obj->AddComponent<OBBCollider3D>();
		obbColider->Init({ 0.0f, 0.0f, 0.0f }, { 0.1f, 0.1f, 0.1f }, true);//
		obj->AddComponent<CollisionContactRecorder<OBBCollider3D>>()->Init((int)Scene::ObjTag::Player);

		obj = AddGameObject("Enemy_AttackCollision");
		obj->SetParent(enemy);
		obbColider = obj->AddComponent<OBBCollider3D>();
		obbColider->Init({ 0.0f, 0.0f, 0.0f }, { 0.1f, 0.1f, 0.1f }, true);//
		obj->AddComponent<CollisionContactRecorder_EnemyAttack<OBBCollider3D>>()->Init((int)Scene::ObjTag::Player);

		obj = AddGameObject("Enemy_JumpCollision");
		obj->SetParent(enemy);
		obbColider = obj->AddComponent<OBBCollider3D>();
		obbColider->Init({ 0.0f, 0.2f, 1.0f }, { 0.25f, 0.25f, 0.1f }, true);
		obj->AddComponent<CollisionContactRecorder<OBBCollider3D>>()->Init((int)Scene::ObjTag::Platform);

		obj = AddGameObject("Enemy_Model_Loot");
		obj->SetParent(enemy);

		parent = obj;

		obj = AddGameObject("Enemy_Model_Egg");
		obj->SetParent(parent);
		obj->AddComponent<ModelRenderer>()->Init("Enemy_Egg", "toonVS", "toonPS");

		return enemy;

		//// ���Ђ悱
		//// ���G�l�~�[���[�g								Enemy_Chick
		//// �����n�ʐڒn����p�R���W����					Enemy_GroundCollision
		//// �������f�����[�g								Enemy_Model_Loot
		//// �@�����Ђ悱���f��							Enemy_Model_Chick

		//enemy = AddGameObject("Enemy_Chick");
		//enemy->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.5f });

		//obj = AddGameObject("Enemy_GroundCollision");
		//obj->SetParent(enemy);
		//obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, -0.6f, 0.0f }, { 0.45f, 0.05f, 0.45f }, true);
		//obj->AddComponent<PlayerGroundCollision>()->Init();

		//obj = AddGameObject("Enemy_Model_Loot");
		//obj->SetParent(enemy);

		//parent = obj;

		//obj = AddGameObject("Enemy_Model_Chick");
		//obj->SetParent(parent);
		//obj->AddComponent<ModelRenderer>()->Init("Enemy_Chick");



		//// ���ɂ�Ƃ�
		//// ���G�l�~�[���[�g								Enemy_Chicken
		//// �����n�ʐڒn����p�R���W����					Enemy_GroundCollision
		//// �������f�����[�g								Enemy_Model_Loot
		//// �@�����ɂ�Ƃ胂�f��							Enemy_Model_Chicken

		//enemy = AddGameObject("Enemy_Chicken");
		//enemy->AddComponent<AABBCollider3D>()->Init({ 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.5f });

		//obj = AddGameObject("Enemy_GroundCollision");
		//obj->SetParent(enemy);
		//obj->AddComponent<AABBCollider3D>()->Init({ 0.0f, -0.6f, 0.0f }, { 0.45f, 0.05f, 0.45f }, true);
		//obj->AddComponent<PlayerGroundCollision>()->Init();

		//obj = AddGameObject("Enemy_Model_Loot");
		//obj->SetParent(enemy);

		//parent = obj;

		//obj = AddGameObject("Enemy_Model_Chicken");
		//obj->SetParent(parent);
		//obj->AddComponent<ModelRenderer>()->Init("Enemy_Chicken");
	}
};
