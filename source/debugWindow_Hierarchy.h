/*******************************************************************************
*
*	�^�C�g���F	�q�G�����L�[�E�B���h�E�N���X
*	�t�@�C���F	debugWindow_Hierarchy.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#ifdef _DEBUG
#pragma once
#include "debugWindow.h"

#include <list>
#include <vector>
#include <optional>
#include <string>

class GameObject;

class DebugWindow_Hierarchy : public DebugWindow
{
private:
	static inline vector<GameObject*> m_ObjList;
	static inline int m_DigitNum = 0;
public:
	static inline int m_SelectedObjectId = -1;

public:
	DebugWindow_Hierarchy(bool isActive = true)
		: DebugWindow(isActive)
	{
		m_WindowName = "Hierarchy";
		m_WindowPos = ImVec2(740.0f, 20.0f);
		m_WindowSize = ImVec2(300.0f, 300.0f);
	}
	~DebugWindow_Hierarchy() {}

	void Draw() override;

	// �E�B���h�E�����
	void BeginWindow();

	// �I�u�W�F�N�g������ő包�������߂�
	void CountDigit(int num);

	// �I�u�W�F�N�g�����ׂĕ\������
	void DrawObjList(list<GameObject*> objList);

	// �q�����I�u�W�F�N�g��\��
	void DrawTree(GameObject* obj);

	// �q�������Ȃ��I�u�W�F�N�g��\��
	void DrawName(GameObject* obj);

	// �\���Ώۂł͂Ȃ��q�I�u�W�F�N�g���i�[
	void PushChild(GameObject* obj);

	// �ő包�����琔�l�𐶐�
	string CreateNumStr(int num);

	// �I�u�W�F�N�g�����X�g����폜
	static void DeleteObj(GameObject* obj);

	// �I����Ԃ̃I�u�W�F�N�g�̏����擾
	static GameObject* GetSelectedObject();

	// �I�u�W�F�N�g�̃q�G�����L�[��̔ԍ����擾
	static optional<int> GetObjectIndex(GameObject* obj);

	// �I����Ԃ̃I�u�W�F�N�g�̃q�G�����L�[��̔ԍ����擾
	static optional<int> GetSelectedObjectIndex();
};
#endif
