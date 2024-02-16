/*******************************************************************************
*
*	�^�C�g���F	GPU�p�[�e�B�N���f���V�[��
*	�t�@�C���F	demo_GpuParticle.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "scene.h"
#include "gameObject.h"

// �R���|�[�l���g
#include "camera.h"
#include "particleSystem.h"
#include "particleEmitChecker.h"
#include "skyDome.h"

// �X�N���v�g
#include "debugCameraControl.h"
#include "testScript.h"
#include "testScript2.h"

class Demo_GpuParticle : public Scene
{
public:
	~Demo_GpuParticle() {}

	void Init() override
	{
		GameObject* obj;

		// �X�J�C�h�[��
		obj = AddGameObject("SkyDome");
		obj->AddComponent<SkyDome>()->Init();

		obj = AddGameObject("Telop");
		obj->m_LocalPosition = { 0.0f, 710.0f, 0.0f };
		obj->AddComponent<Plate2DRenderer>()->Init("Telop_GpuParticle", { 900.0f, 150.0f });


		ParticleSystem::EmitParam emitParam = {
			true, 200.0f, 100100,
			{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f },
			ParticleSystem::SimulationSpaceId::World,
			ParticleSystem::EmitterStopActionId::ObjectDestroy,
		};
		ParticleSystem::ParticleDefaultParam particleDefaultParam = {
			5.0f, Math::MinMax<float>(0.01f, 0.05f),
			Math::MinMax<D3DXVECTOR3>({ -0.1f, -0.1f, -0.1f }, { 0.1f, 0.1f, 0.1f }),
			{ 0.0f, -0.001f, 0.0f }, "DevelopLogo", { 1.0f, 1.0f, 1.0f, 1.0f },
		};
		obj = AddGameObject("ParticleTest");
		obj->AddComponent<ParticleSystem>()->Init(0.0f, true,
			emitParam, particleDefaultParam,
			DBG_NEW ParticleEmitChacker_Burst(1.0f, 100000, 10, 6.0f, 1.0f));


		// �J����
		obj = AddGameObject("Camera");
		obj->m_LocalPosition = { 0.0f, 0.0f, -5.0f };
		obj->AddComponent<Camera>()->Init(true);
		obj->AddComponent<DebugCameraControl>()->Init();
	}
};
