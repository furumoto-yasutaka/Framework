/*******************************************************************************
*
*	�^�C�g���F	�f�o�b�O�E�B���h�E����ÓI�N���X
*	�t�@�C���F	debugManager.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#ifdef _DEBUG
#include "debugManager.h"
#include "manager.h"
#include "renderer.h"
#include "debugWindow.h"
#include "debugWindow_Hierarchy.h"
#include "debugWindow_ObjectInspector.h"
#include "debugWindow_SceneInspector.h"

#include "demo_Invoke.h"
#include "demo_Parent.h"
#include "demo_CollisionSplit.h"
#include "demo_GpuParticle.h"
#include "demo_StreamingAudio.h"

#include "developLogo.h"

#include <windows.h>
#include <psapi.h>

void DebugManager::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// ���{��ݒ�
	//ImGuiIO& io = ImGui::GetIO();
	//ImFontConfig guiFontConfig;
	//guiFontConfig.MergeMode = true;
	//io.Fonts->AddFontFromFileTTF("asset/font/Roboto-Medium.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("asset/font/meiryo.ttc", 15.0f,
	//	&guiFontConfig, glyphRangesJapanese);

	// �t�H���g�ݒ�
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("asset/font/Roboto-Medium.ttf", 15.0f);
	
	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());

	// �e��E�B���h�E�̓o�^
	m_WindowList.push_back(DBG_NEW DebugWindow_Hierarchy());
	m_WindowList.push_back(DBG_NEW DebugWindow_ObjectInspector());
	m_WindowList.push_back(DBG_NEW DebugWindow_SceneInspector());

	for (int i = 0; i < sizeof(m_UseMemory) / sizeof(float); i++)
	{
		m_UseMemory[i] = 0.0f;
	}
}

void DebugManager::Uninit()
{
	for (DebugWindow* w : m_WindowList)
	{
		delete w;
	}
	m_WindowList.clear();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugManager::Update()
{

}

void DebugManager::Draw()
{
	DrawBegin();

	// �ėp�E�B���h�E�\��
	ImGui::SetNextWindowPos(m_WINDOW_POS, ImGuiCond_Once);
	ImGui::SetNextWindowSize(m_WINDOW_SIZE, ImGuiCond_Once);

	ImGui::Begin("GeneralWindow", ((bool*)0), ImGuiWindowFlags_NoMove);

	string str = "Scene : " + m_DEMO_SCENE_INFO[m_CaptionIndex].Name;
	ImGui::Text(str.c_str());

	SpacingAndSeparator();

	DrawMemory();

	DrawDebugFlag();

	DrawWindowFlag();

	DrawDemoSceneSelectButton();
	DrawGameSceneSelectButton();

	ImGui::End();

	// �J�X�^���E�B���h�E�\��
	for (DebugWindow* w : m_WindowList)
	{
		if (w->GetIsActive())
		{
			w->Draw();
		}
	}

	DrawEnd();
}

void DebugManager::DrawBegin()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DebugManager::DrawEnd()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

/*******************************************************************************
*	�������g�p�ʕ\��
*******************************************************************************/
void DebugManager::DrawMemory()
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("UseMemory"))
	{
		// �������g�p�ʕ\��
		HANDLE hProc = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS_EX pmc;
		BOOL isSuccess = GetProcessMemoryInfo(
			hProc,
			(PROCESS_MEMORY_COUNTERS*)&pmc,
			sizeof(pmc));
		CloseHandle(hProc);
		if (isSuccess != FALSE)
		{
			int size = sizeof(m_UseMemory) / sizeof(float);
			for (int i = 0; i < size - 1; i++)
			{
				m_UseMemory[i] = m_UseMemory[i + 1];
			}
			m_UseMemory[size - 1] = (float)pmc.PrivateUsage / 1000000;

			if (m_MaxMemory < m_UseMemory[size - 1])
			{
				m_MaxMemory = m_UseMemory[size - 1];
			}

			ImGui::PlotLines("", m_UseMemory, size, 0, NULL, 0.0f, m_MaxMemory, ImVec2(0, 50));

			string temp = "UseMemory : " + to_string((int)pmc.PrivateUsage / 1000000) + "MB";
			ImGui::Text(temp.c_str());
		}
	}
}

/*******************************************************************************
*	�f�o�b�O�@�\�`�F�b�N�{�b�N�X�\��
*******************************************************************************/
void DebugManager::DrawDebugFlag()
{
	if (ImGui::CollapsingHeader("DebugFlag"))
	{
		ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(m_LIST_WIDTH,
			m_LIST_BLANK_HEIGHT + m_LIST_LINE_HEIGHT * 4), ImGuiWindowFlags_NoTitleBar);

		bool temp = m_IsWireFrame;
		ImGui::Checkbox("WireFrame", &m_IsWireFrame);
		if (temp != m_IsWireFrame)
		{
			Renderer::SetWireFrame(m_IsWireFrame);
		}

		ImGui::Checkbox("ParentLine", &m_IsParentLine);

		ImGui::Checkbox("CollisionFrame", &m_IsCollisionFrame);

		ImGui::Checkbox("SpaceDivisionFrame", &m_IsSpaceDivisionFrame);

		ImGui::EndChild();
	}
}

/*******************************************************************************
*	�f�o�b�O�E�B���h�E�`�F�b�N�{�b�N�X�\��
*******************************************************************************/
void DebugManager::DrawWindowFlag()
{
	// �ʃE�B���h�E�̕\������
	if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::BeginChild(ImGui::GetID((void*)1), ImVec2(m_LIST_WIDTH,
			m_LIST_BLANK_HEIGHT + m_LIST_LINE_HEIGHT * m_WindowList.size()), ImGuiWindowFlags_NoTitleBar);
		for (DebugWindow* w : m_WindowList)
		{
			bool isActive = w->GetIsActive();
			ImGui::Checkbox(w->GetName().c_str(), &isActive);
			w->SetIsActive(isActive);
		}

		ImGui::EndChild();
	}
}

/*******************************************************************************
*	�f���V�[���؂�ւ��{�^���\��
*******************************************************************************/
void DebugManager::DrawDemoSceneSelectButton()
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("DemoSceneSelect"))
	{
		for (unsigned int i = 0; i < m_DEMO_SCENE_INFO.size(); i++)
		{
			if (ImGui::Button(m_DEMO_SCENE_INFO[i].Name.c_str()))
			{
				m_CaptionIndex = i;

				m_IsWireFrame = m_DEMO_SCENE_INFO[i].IsWireFrame;
				m_IsParentLine = m_DEMO_SCENE_INFO[i].IsParentLine;
				m_IsCollisionFrame = m_DEMO_SCENE_INFO[i].IsCollisionFrame;
				m_IsSpaceDivisionFrame = m_DEMO_SCENE_INFO[i].IsSpaceDivisionFrame;
				m_DEMO_SCENE_INFO[i].ChangeFunc();
			}
		}
	}
}

/*******************************************************************************
*	�Q�[���V�[���؂�ւ��{�^���\��
*******************************************************************************/
void DebugManager::DrawGameSceneSelectButton()
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("GameSceneSelect"))
	{
		for (unsigned int i = 0; i < m_GAME_SCENE_INFO.size(); i++)
		{
			if (ImGui::Button(m_GAME_SCENE_INFO[i].Name.c_str()))
			{
				m_CaptionIndex = i;

				m_IsWireFrame = m_GAME_SCENE_INFO[i].IsWireFrame;
				m_IsParentLine = m_GAME_SCENE_INFO[i].IsParentLine;
				m_IsCollisionFrame = m_GAME_SCENE_INFO[i].IsCollisionFrame;
				m_IsSpaceDivisionFrame = m_GAME_SCENE_INFO[i].IsSpaceDivisionFrame;
				m_GAME_SCENE_INFO[i].ChangeFunc();
			}
		}
	}
}

void DebugManager::ChangeScene_Demo_Invoke()
{
	Manager::SetScene<Demo_Invoke>(Transition::GetFirstTransitionOption());
}

void DebugManager::ChangeScene_Demo_Parent()
{
	Manager::SetScene<Demo_Parent>(Transition::GetFirstTransitionOption());
}

void DebugManager::ChangeScene_Demo_CollisionSplit()
{
	Manager::SetScene<Demo_CollisionSplit>(Transition::GetFirstTransitionOption());
}

void DebugManager::ChangeScene_Demo_GpuParticle()
{
	Manager::SetScene<Demo_GpuParticle>(Transition::GetFirstTransitionOption());
}

void DebugManager::ChangeScene_Demo_StreamingAudio()
{
	Manager::SetScene<Demo_StreamingAudio>(Transition::GetFirstTransitionOption());
}

void DebugManager::ChangeScene_Game_DevelopLogo()
{
	Manager::SetScene<DevelopLogo>(Transition::GetFirstTransitionOption());
}

/*******************************************************************************
*	�����Ƌ󔒂̕\��
*******************************************************************************/
void DebugManager::SpacingAndSeparator()
{
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
}

/*******************************************************************************
*	�R���{�{�b�N�X�̕\��
*******************************************************************************/
bool DebugManager::ComboBox(string caption, int& currentIndex, vector<string> items)
{
	bool changed = false;
	string current;

	// �C���f�b�N�X�����������m�F
	if (currentIndex >= 0 && currentIndex < (int)items.size())
	{
		current = items[currentIndex];
	}
	else
	{
		// �A�C�e�����󂩂ǂ���
		if (items.empty())
		{
			// ��̏ꍇ�͏������I��
			return false;
		}
		else
		{
			// ��ԍŏ��̗v�f��I��
			current = items[0];
		}
	}

	if (ImGui::BeginCombo(caption.c_str(), current.c_str()))
	{
		for (size_t n = 0; n < items.size(); n++)
		{
			bool is_selected = (currentIndex == n);
			if (ImGui::Selectable(items[n].c_str(), is_selected))
			{
				currentIndex = n;
				changed = true;
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	return changed;
}
#endif
