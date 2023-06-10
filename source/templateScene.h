/*******************************************************************************
*
*	�^�C�g���F	�V�[���N���X
*	�t�@�C���F	.h
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

class TemplateScene : public Scene
{
public:
	~TemplateScene() {}

	void Init() override
	{
		GameObject* obj;

		// �J����
		obj = AddGameObject("Camera");
		obj->m_LocalPosition = { 0.0f, 0.0f, -5.0f };
		obj->AddComponent<Camera>()->Init(true);
		obj->AddComponent<DebugCameraControl>()->Init();
	}
};
