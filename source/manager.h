/*******************************************************************************
*
*	�^�C�g���F	�A�v���P�[�V�����Ǘ��p�ÓI�N���X
*	�t�@�C���F	manager.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "scene.h"
#include "transition.h"

class Manager
{
public:
	static inline Scene* m_Scene = NULL;		// �V�[���̃C���X�^���X
	static inline Scene* m_NextScene = NULL;	// �\��p�V�[���̃C���X�^���X

protected:
	Manager() = default;
public:
	static void Init(HINSTANCE hInstance);
	static void Uninit();
	static void Update();
	static void Draw();

	// ���̃V�[����\��
	template<class T>
	static void SetScene(Transition::TransitionOption info)
	{
		if (m_NextScene)
		{
			delete m_NextScene;
			m_NextScene = NULL;
		}

		m_NextScene = DBG_NEW T();

		Transition::SetTransitionOption(info);

		// �V�[�������݂���(exe�N������ł͂Ȃ�)�ꍇ
		// �J�ڑO�g�����W�V�������s��
		if (m_Scene)
		{
			SetTransition_In();
		}
	}

	// �g�����W�V�����p�Q�[���I�u�W�F�N�g�𐶐�����
	static Transition* SetTransition_In();

	// �V�[���J�ڂ��s�����m�F
	static void CheckScene();

protected:
	static void SetInitialScene();

public:
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	static Scene* GetScene() { return m_Scene; }
	/////////////////////////////
};
