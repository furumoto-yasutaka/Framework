/*******************************************************************************
*
*	�^�C�g���F	�q�G�����L�[�E�B���h�E�N���X
*	�t�@�C���F	debugWindow_Hierarchy.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#ifdef _DEBUG
#include "debugWindow_Hierarchy.h"
#include "gameObject.h"
#include "math.h"

#include <list>
#include <algorithm>

void DebugWindow_Hierarchy::Draw()
{
	// ���X�g���N���A
	m_ObjList.clear();

	ImGui::SetNextWindowPos(m_WindowPos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(m_WindowSize, ImGuiCond_Once);

	// ���W�Œ�ݒ�ŃE�B���h�E��\��
	ImGui::Begin(m_WindowName.c_str(), ((bool*)0), ImGuiWindowFlags_HorizontalScrollbar);

	list<GameObject*> objList = Manager::GetScene()->GetGameObjectAll();

	if (!objList.empty())
	{
		CountDigit(objList.size());

		// �`�揈��
		for (GameObject* obj : objList)
		{
			if (!obj->GetParent())
			{
				if (!obj->GetChild(0))
				{// �q�������݂��Ȃ�
					DrawName(obj);
				}
				else
				{// �q�������݂���
					DrawTree(obj);
				}
			}
		}
	}
	else
	{
		m_SelectedObjectId = -1;
	}


	ImGui::End();
}

/*******************************************************************************
*	�I�u�W�F�N�g������ő包�������߂�
*******************************************************************************/
void DebugWindow_Hierarchy::CountDigit(int num)
{
	m_DigitNum = 0;

	while (num != 0)
	{
		num /= 10;
		m_DigitNum++;
	}
}

/*******************************************************************************
*	�q�����I�u�W�F�N�g��\��
*******************************************************************************/
void DebugWindow_Hierarchy::DrawTree(GameObject* obj)
{
	m_ObjList.push_back(obj);

	if (!obj->GetIsActive())
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	ImGui::RadioButton(CreateNumStr(m_ObjList.size() - 1).c_str(), &m_SelectedObjectId, m_ObjList.size() - 1);

	ImGui::SameLine();
	if (ImGui::TreeNode(obj->GetName().c_str()))
	{// ���X�g��W�J���Ă���
		if (!obj->GetIsActive())
		{
			ImGui::PopStyleColor();
		}

		for (GameObject* child : obj->GetChildList())
		{
			if (!child->GetChild(0))
			{// �q�������݂��Ȃ�
				DrawName(child);
			}
			else
			{// �q�������݂���
				DrawTree(child);
			}
		}

		ImGui::TreePop();
	}
	else
	{// ���X�g��W�J���Ă��Ȃ�
		if (!obj->GetIsActive())
		{
			ImGui::PopStyleColor();
		}

		for (GameObject* child : obj->GetChildList())
		{
			PushChild(child);
		}
	}
}

/*******************************************************************************
*	�q�������Ȃ��I�u�W�F�N�g��\��
*******************************************************************************/
void DebugWindow_Hierarchy::DrawName(GameObject* obj)
{
	m_ObjList.push_back(obj);

	if (obj->GetIsActive())
	{
		string str = CreateNumStr(m_ObjList.size() - 1) + "    " + obj->GetName();
		ImGui::RadioButton(str.c_str(), &m_SelectedObjectId, m_ObjList.size() - 1);
	}
	else
	{
		// �A�N�e�B�u�łȂ��ꍇ�͐F���Â�����
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

		string str = CreateNumStr(m_ObjList.size() - 1) + "    " + obj->GetName();
		ImGui::RadioButton(str.c_str(), &m_SelectedObjectId, m_ObjList.size() - 1);

		ImGui::PopStyleColor();
	}
}

/*******************************************************************************
*	�\���Ώۂł͂Ȃ��q�I�u�W�F�N�g���i�[
*******************************************************************************/
void DebugWindow_Hierarchy::PushChild(GameObject* obj)
{
	m_ObjList.push_back(obj);

	for (GameObject* child : obj->GetChildList())
	{
		PushChild(child);
	}
}

/*******************************************************************************
*	�ő包�����琔�l�𐶐�
*******************************************************************************/
string DebugWindow_Hierarchy::CreateNumStr(int num)
{
	string result = "";

	for (int i = 0; i < m_DigitNum; i++)
	{
		int n = num % 10;

		switch (n)
		{
		case 0: result = "0" + result; break;
		case 1: result = "1" + result; break;
		case 2: result = "2" + result; break;
		case 3: result = "3" + result; break;
		case 4: result = "4" + result; break;
		case 5: result = "5" + result; break;
		case 6: result = "6" + result; break;
		case 7: result = "7" + result; break;
		case 8: result = "8" + result; break;
		case 9: result = "9" + result; break;
		}

		num /= 10;
	}

	return result;
}

/*******************************************************************************
*	�I���I�u�W�F�N�g���L�����m�F
*******************************************************************************/
void DebugWindow_Hierarchy::DeleteObj(GameObject* obj)
{
	auto itr = m_ObjList.begin();
	while (itr != m_ObjList.end())
	{
		if ((*itr) == obj)
		{
			m_ObjList.erase(itr);
			break;
		}
		else
		{
			itr++;
		}
	}

	m_SelectedObjectId = clamp(m_SelectedObjectId, -1, (int)m_ObjList.size() - 1);
}

/*******************************************************************************
*	�I����Ԃ̃I�u�W�F�N�g�̏����擾
*******************************************************************************/
GameObject* DebugWindow_Hierarchy::GetSelectedObject()
{
	if (m_SelectedObjectId != -1)
	{
		return m_ObjList[m_SelectedObjectId];
	}
	else
	{
		return NULL;
	}
}

/*******************************************************************************
*	�I����Ԃ̃I�u�W�F�N�g�̏����擾
*******************************************************************************/
optional<int> DebugWindow_Hierarchy::GetObjectIndex(GameObject* obj)
{
	for (size_t i = 0; i < m_ObjList.size(); i++)
	{
		if (m_ObjList[i] == obj)
		{
			return i;
		}
	}

	return nullopt;
}

/*******************************************************************************
*	�I����Ԃ̃I�u�W�F�N�g�̃q�G�����L�[��̔ԍ����擾
*******************************************************************************/
optional<int> DebugWindow_Hierarchy::GetSelectedObjectIndex()
{
	if (m_SelectedObjectId != -1)
	{
		return m_SelectedObjectId;
	}
	else
	{
		return nullopt;
	}
}
#endif
