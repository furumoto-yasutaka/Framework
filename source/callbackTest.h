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

// �X�N���v�g
#include "debugCameraControl.h"
#include "testCallback_Call.h"
#include "testCallback_Func.h"

class CallBackTest : public Scene
{
public:
	~CallBackTest() {}

	void Init() override
	{
		GameObject* obj;


		obj = AddGameObject("CallBack");
		obj->AddComponent<TestCallback_Call>()->Init();
		obj->AddComponent<TestCallback_Func>()->Init();

		// �J����
		obj = AddGameObject("Camera");
		obj->m_LocalPosition = { 0.0f, 0.0f, -5.0f };
		obj->AddComponent<Camera>()->Init(true);
		obj->AddComponent<DebugCameraControl>()->Init();
	}
};
