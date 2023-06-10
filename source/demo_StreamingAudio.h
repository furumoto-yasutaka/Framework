/*******************************************************************************
*
*	�^�C�g���F	�X�g���[�~���O�Đ��f���V�[��
*	�t�@�C���F	demo_StreamingAudio.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "scene.h"
#include "gameObject.h"

// �R���|�[�l���g
#include "camera.h"
#include "audioSpeaker.h"

// �X�N���v�g
#include "debugCameraControl.h"

class Demo_StreamingAudio : public Scene
{
public:
	~Demo_StreamingAudio() {}

	void Init() override
	{
		GameObject* obj;


		obj = AddGameObject("Telop");
		obj->m_LocalPosition = { 440.0f, 490.0f, 0.0f };
		obj->AddComponent<Plate2DRenderer>()->Init("Telop_StreamingAudio", { 1100.0f, 380.0f });


		obj = AddGameObject("Audio");
		AudioSpeaker* speaker = obj->AddComponent<AudioSpeaker>();
		speaker->Init();
		speaker->Play("TestBgm1", true);
		speaker->Play("TestBgm2", true);
		speaker->Play("TestBgm3", true);
		speaker->Play("TestBgm4", true);


		// �J����
		obj = AddGameObject("Camera");
		obj->m_LocalPosition = { 0.0f, 0.0f, -5.0f };
		obj->AddComponent<Camera>()->Init(true);
		obj->AddComponent<DebugCameraControl>()->Init();
	}
};
