/*******************************************************************************
*
*	�^�C�g���F	�^�C�g���V�[���N���X
*	�t�@�C���F	title.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "scene.h"
#include "gameObject.h"

// �R���|�[�l���g
#include "camera.h"
#include "plate2dRenderer.h"
#include "skyDome.h"

// �X�N���v�g
#include "debugCameraControl.h"

class Title : public Scene
{
public:
	~Title() {}

	void Init() override
	{
		GameObject* obj;

		// �X�J�C�h�[��
		obj = AddGameObject("SkyDome");
		obj->AddComponent<SkyDome>()->Init();

		obj = AddGameObject("Manager");

		// �w�i
		obj = AddGameObject("BackGround");
		obj->AddComponent<Plate2DRenderer>()->Init("White",
			{ Application::m_WINDOW_RESOLUTION.x, Application::m_WINDOW_RESOLUTION.y });

		// �^�C�g�����S
		obj = AddGameObject("GameLogo");
		obj->AddComponent<Plate2DRenderer>()->Init("GameLogo",
			{ 1000.0f, 650.0f });

		// �Ñ��e�L�X�g
		obj = AddGameObject("PromptText");
		obj->AddComponent<Plate2DRenderer>()->Init("Title_PromptText",
			{ 600.0f, 80.0f });

		// �J����
		obj = AddGameObject("Camera");
		obj->m_LocalPosition = { 0.0f, 0.0f, -5.0f };
		obj->AddComponent<Camera>()->Init(true);
#ifdef _DEBUG
		obj->AddComponent<DebugCameraControl>()->Init();
#endif
	}
};

// �Ñ��e�L�X�g�̓_��
// �{�^���̎�t�Ǝ������S�\���߂�(�Ђ��߂����炱�҂�)
