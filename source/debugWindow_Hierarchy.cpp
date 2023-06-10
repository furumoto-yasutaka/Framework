/*******************************************************************************
*
*	タイトル：	ヒエラルキーウィンドウクラス
*	ファイル：	debugWindow_Hierarchy.cpp
*	作成者：	古本 泰隆
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
	// リストをクリア
	m_ObjList.clear();

	ImGui::SetNextWindowPos(m_WindowPos, ImGuiCond_Once);
	ImGui::SetNextWindowSize(m_WindowSize, ImGuiCond_Once);

	// 座標固定設定でウィンドウを表示
	ImGui::Begin(m_WindowName.c_str(), ((bool*)0), ImGuiWindowFlags_HorizontalScrollbar);

	list<GameObject*> objList = Manager::GetScene()->GetGameObjectAll();

	if (!objList.empty())
	{
		CountDigit(objList.size());

		// 描画処理
		for (GameObject* obj : objList)
		{
			if (!obj->GetParent())
			{
				if (!obj->GetChild(0))
				{// 子供が存在しない
					DrawName(obj);
				}
				else
				{// 子供が存在する
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
*	オブジェクト数から最大桁数を求める
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
*	子を持つオブジェクトを表示
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
	{// リストを展開している
		if (!obj->GetIsActive())
		{
			ImGui::PopStyleColor();
		}

		for (GameObject* child : obj->GetChildList())
		{
			if (!child->GetChild(0))
			{// 子供が存在しない
				DrawName(child);
			}
			else
			{// 子供が存在する
				DrawTree(child);
			}
		}

		ImGui::TreePop();
	}
	else
	{// リストを展開していない
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
*	子を持たないオブジェクトを表示
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
		// アクティブでない場合は色を暗くする
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));

		string str = CreateNumStr(m_ObjList.size() - 1) + "    " + obj->GetName();
		ImGui::RadioButton(str.c_str(), &m_SelectedObjectId, m_ObjList.size() - 1);

		ImGui::PopStyleColor();
	}
}

/*******************************************************************************
*	表示対象ではない子オブジェクトを格納
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
*	最大桁数から数値を生成
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
*	選択オブジェクトが有効か確認
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
*	選択状態のオブジェクトの情報を取得
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
*	選択状態のオブジェクトの情報を取得
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
*	選択状態のオブジェクトのヒエラルキー上の番号を取得
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
