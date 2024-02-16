/*******************************************************************************
*
*	�^�C�g���F	�R�[���o�b�N�e�X�g�V�[���N���X
*	�t�@�C���F	callbackTest.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "scene.h"
#include "gameObject.h"

// �R���|�[�l���g
#include "camera.h"
#include "skyDome.h"

// �X�N���v�g
#include "debugCameraControl.h"
#include "testCallback_Call.h"
#include "testCallback_Func.h"

class CallbackTest : public Scene
{
public:
	~CallbackTest() {}

	void Init() override
	{
		GameObject* obj;

		// �X�J�C�h�[��
		obj = AddGameObject("SkyDome");
		obj->AddComponent<SkyDome>()->Init();

		obj = AddGameObject("Callback");
		obj->AddComponent<TestCallback_Call>()->Init();
		obj->AddComponent<TestCallback_Func>()->Init();

		// �J����
		obj = AddGameObject("Camera");
		obj->m_LocalPosition = { 0.0f, 0.0f, -5.0f };
		obj->AddComponent<Camera>()->Init(true);
		obj->AddComponent<DebugCameraControl>()->Init();
	}
};
