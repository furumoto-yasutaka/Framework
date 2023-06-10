/*******************************************************************************
*
*	�^�C�g���F	�f�o�b�O�E�B���h�E����ÓI�N���X
*	�t�@�C���F	debugManager.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#ifdef _DEBUG
#pragma once
#include "main.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <list>
#include <vector>
#include <string>

class DebugWindow;

class DebugManager
{
	struct SceneButtonInfo
	{
		string Name;
		bool IsWireFrame;
		bool IsParentLine;
		bool IsCollisionFrame;
		bool IsSpaceDivisionFrame;
		void(* ChangeFunc)();
	};

private:
	static inline const ImVec2 m_WINDOW_POS = { 20.0f, 20.0f };
	static inline const ImVec2 m_WINDOW_SIZE = { 300.0f, 300.0f };
	static inline const float m_LIST_WIDTH = 250.0f;
	static inline const float m_LIST_BLANK_HEIGHT = 12.0f;
	static inline const float m_LIST_LINE_HEIGHT = 25.0f;

	static inline list<DebugWindow*> m_WindowList;
	static inline int m_CaptionIndex = 1;

	static inline float m_UseMemory[180];
	static inline float m_MaxMemory = 0.0f;

public:
	static inline bool m_IsWireFrame = false;				// ���C���[�t���[��
	static inline bool m_IsParentLine = true;				// �I�u�W�F�N�g�Ƃ̐e�q�֌W
	static inline bool m_IsCollisionFrame = true;			// �����蔻��̃t���[��
	static inline bool m_IsSpaceDivisionFrame = false;		// ��ԕ����̃t���[��

private:
	DebugManager() {}
public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();	// �e�E�B���h�E�̕\������p�̃E�B���h�E����肽��

	static void DrawBegin();
	static void DrawEnd();

	// �������g�p�ʕ\��
	static void DrawMemory();
	// �f�o�b�O�@�\�`�F�b�N�{�b�N�X�\��
	static void DrawDebugFlag();
	// �f�o�b�O�E�B���h�E�`�F�b�N�{�b�N�X�\��
	static void DrawWindowFlag();
	// �f���V�[���؂�ւ��{�^���\��
	static void DrawDemoSceneSelectButton();
	// �Q�[���V�[���؂�ւ��{�^���\��
	static void DrawGameSceneSelectButton();

	// �V�[���؂�ւ�(�f��)
	static void ChangeScene_Demo_Invoke();
	static void ChangeScene_Demo_Parent();
	static void ChangeScene_Demo_CollisionSplit();
	static void ChangeScene_Demo_GpuParticle();
	static void ChangeScene_Demo_StreamingAudio();

	// �V�[���؂�ւ�(�Q�[��)
	static void ChangeScene_Game_DevelopLogo();

	// �֗��@�\
	static void SpacingAndSeparator();
	static bool ComboBox(string caption,
		int& currentIndex, vector<string> items);

private:
	static inline const vector<SceneButtonInfo> m_DEMO_SCENE_INFO =
	{
		{ "Demo_Invoke", false, true, true, false, &ChangeScene_Demo_Invoke },
		{ "Demo_Parent & CollisionFrame", false, true, true, false, &ChangeScene_Demo_Parent },
		{ "Demo_CollisionSplit", false, false, true, true, &ChangeScene_Demo_CollisionSplit },
		{ "Demo_GpuParticle", false, false, false, false, &ChangeScene_Demo_GpuParticle },
		{ "Demo_StreamingAudio", false, false, false, false, &ChangeScene_Demo_StreamingAudio },
	};

	static inline const vector<SceneButtonInfo> m_GAME_SCENE_INFO =
	{
		{ "DevelopLogo", false, false, true, true, &ChangeScene_Game_DevelopLogo },
	};
};
#endif
