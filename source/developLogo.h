/*******************************************************************************
*
*	�^�C�g���F	�J�����S�\���V�[���N���X
*	�t�@�C���F	developLogo.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "scene.h"
#include "gameObject.h"

// �R���|�[�l���g
#include "camera.h"
#include "plate2dRenderer.h"

// �X�N���v�g
#include "debugCameraControl.h"
#include "developLogoManager.h"

class DevelopLogo : public Scene
{
public:
	~DevelopLogo() {}

	void Init() override
	{
		GameObject* obj;

		// �V�[���}�l�[�W���[
		obj = AddGameObject("Manager");
		obj->AddComponent<DevelopLogoManager>()->Init();

		// �J����
		obj = AddGameObject("Camera");
		obj->m_LocalPosition = { 0.0f, 0.0f, -5.0f };
		obj->AddComponent<Camera>()->Init(true);
#ifdef _DEBUG
		obj->AddComponent<DebugCameraControl>()->Init();
#endif

		// �w�i
		obj = AddGameObject("BackGround");
		obj->AddComponent<Plate2DRenderer>()->Init("White",
			{ WINDOW_RESOLUTION_WIDTH, WINDOW_RESOLUTION_HEIGHT },
			Plate2DRenderer::AnchorPosition::Left_Top);

		// �J�����S
		obj = AddGameObject("DevelopLogo");
		obj->m_LocalPosition =
			{ WINDOW_RESOLUTION_WIDTH * 0.5f, WINDOW_RESOLUTION_HEIGHT * 0.5f, 0.0f };
		obj->AddComponent<Plate2DRenderer>()->Init("DevelopLogo",
			{ 300.0f, 300.0f }, Plate2DRenderer::AnchorPosition::Center_Middle);

		//InitD();
	}

#ifdef _DEBUG
	//// �V�[�����̃A�j���[�V�������
	//struct AnimationClip
	//{
	//	std::string name;
	//	float length;
	//};

	//// �A�j���[�V�������
	//std::vector<AnimationClip> animationClips;
	//int selectedClipIndex = -1;

	//// �A�j���[�V�����^�C�����C���֘A�̕ϐ�
	//float currentTime = 0.0f;
	//float totalTime = 10.0f;
	//float playbackSpeed = 1.0f;

	//void InitD()
	//{
	//	// �A�j���[�V�����N���b�v�̏�����
	//	animationClips.push_back({ "Clip 1", 5.0f });
	//	animationClips.push_back({ "Clip 2", 10.0f });
	//	animationClips.push_back({ "Clip 3", 8.0f });
	//}

	void DrawInspector(ImVec2 pos, ImVec2 size) override
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 v1 = pos;
		ImVec2 v2 = pos;
		v2.x += 200.0f;
		v2.y += 200.0f;
		drawList->AddLine(v1, v2, IM_COL32(255, 0, 0, 255), 1.0f);





		//// �A�j���[�V�����N���b�v���X�g
		//ImGui::Text("Animation Clips:");
		//ImGui::Separator();
		//ImGui::BeginChild("AnimationClipList", ImVec2(200, 200), true);
		//for (int i = 0; i < animationClips.size(); i++)
		//{
		//	const bool isSelected = (selectedClipIndex == i);
		//	if (ImGui::Selectable(animationClips[i].name.c_str(), isSelected))
		//		selectedClipIndex = i;
		//}
		//ImGui::EndChild();

		//ImGui::Separator();

		//// �A�j���[�V�����^�C�����C��
		//ImGui::Text("Animation Timeline:");
		//ImGui::SliderFloat("Current Time", &currentTime, 0.0f, totalTime);
		//ImGui::SliderFloat("Playback Speed", &playbackSpeed, 0.0f, 2.0f);





	}
#endif
};
