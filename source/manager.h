/*******************************************************************************
*
*	�^�C�g���F	�Q�[���S�̊Ǘ��p�ÓI�N���X
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

private:
	Manager() {}
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

		if (m_Scene)
		{// �V�[�������݂���ꍇ
			// �ʏ�̑J��
			Transition::StartTransitionOut(info);
		}
		else
		{// �V�[�������݂��Ȃ��ꍇ
			// �Q�[���N������Ƃ������ƂȂ̂Ńt�F�[�h�A�E�g�͂����A�J�ڐݒ�𔽉f����
			Transition::SetTransitionOption(info);
		}
	}

	// �V�[���J�ڂ��s�����m�F
	static void CheckScene();

private:
	static void SetInitialScene();

public:
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	static Scene* GetScene() { return m_Scene; }
	/////////////////////////////
};