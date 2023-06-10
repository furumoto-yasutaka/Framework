/*******************************************************************************
*
*	�^�C�g���F	�Q�[���S�̊Ǘ��p�ÓI�N���X
*	�t�@�C���F	manager.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "photonTest.h"
#include "manager.h"

// �ÓI�N���X
#include "renderer.h"
#include "time.h"
#include "input.h"
#include "modelContainer.h"
#include "textureContainer.h"
#include "audioContainer.h"
#include "skyDome.h"
#include "savedataManager.h"
#include "collision3dManager.h"
#include "collider3d.h"

#ifdef _DEBUG
#include "debugManager.h"
#include "debugConsole.h"
#endif

// ��ԍŏ��ɊJ�n����V�[��
#include "developLogo.h"

#include <typeinfo>


void Manager::Init(HINSTANCE hInstance)
{
	// �ÓI�N���X�̏�����
#ifdef _DEBUG
	DebugConsole::Init();
#endif
	Renderer::Init();
	Input::Init(hInstance);
	TextureContainer::Init();
	ModelContainer::Init();
	AudioContainer::Init();
	SkyDome::Init();
	Transition::Init();
	SavedataManager::Init();
	Collision3DManager::Init(0, D3DXVECTOR3(-60.0f, -60.0f, -60.0f), D3DXVECTOR3(120.0f, 120.0f, 120.0f));

#ifdef _DEBUG
	DebugManager::Init();
#endif
	MyPhoton::getInstance().connect();


	// �ŏ��̃V�[�����N��
	SetInitialScene();

	// ���[�h���Ԃ��v�����Ԃɓ���Ă��܂��̂ōŌ�ɍs��
	Time::Init();
}

void Manager::Uninit()
{
	// �������Ƌt���ɏI���������s��
	m_Scene->Uninit();
	delete m_Scene;
	Scene::StaticUninit();

	MyPhoton::getInstance().disconnect();

	// �ÓI�N���X�̏I������(Init�̋t����)
#ifdef _DEBUG
	DebugManager::Uninit();
#endif
	Collision3DManager::Uninit();
	SavedataManager::Uninit();
	Transition::Uninit();
	SkyDome::Uninit();
	AudioContainer::Uninit();
	ModelContainer::Uninit();
	TextureContainer::Uninit();
	Input::Uninit();
	Time::Uninit();
	Renderer::Uninit();
#ifdef _DEBUG
	DebugConsole::Uninit();
#endif
}

void Manager::Update()
{
#ifdef _DEBUG
	DebugConsole::Update();
#endif
	// ���Ԍn�Ɠ��͌n��D��ōX�V
	Time::Update();
	Input::Update();

	// �V�[���X�V
	m_Scene->Update();

	// �I�u�W�F�N�g�̍X�V��ɏՓ˔�����s��
	Collision3DManager::Update();

#ifdef _DEBUG
	// �f�o�b�O����\��
	DebugManager::Update();
#endif

	MyPhoton::getInstance().run();

	// �I�u�W�F�N�g�̍폜���m�F����
	m_Scene->CheckDestroy();
}

void Manager::Draw()
{
	// �w�i���N���A����
	Renderer::Begin();

	// �V�[�����I�u�W�F�N�g�`��
	m_Scene->Draw();

#ifdef _DEBUG
	DebugManager::Draw();
#endif

	// ��ʂ��X���b�v
	Renderer::End();
}

/*******************************************************************************
*	�V�[���J�ڂ��s�����m�F
*******************************************************************************/
void Manager::CheckScene()
{
	// �\�񂪖����ꍇ�܂��̓g�����W�V�������̏ꍇ�͑J�ڂ����I������
	if (!m_NextScene ||
		Transition::GetTransitionState() == Transition::TransitionState::Out)
	{ return; }

	//------------------------
	// �J�ڏ���
	//------------------------
	if (m_Scene)
	{
		// ���݂܂ł̃V�[�����폜
		m_Scene->Uninit();
		delete m_Scene;
	}

	// �\�񂵂��V�[���ɐ؂�ւ�
	m_Scene = m_NextScene;
	m_NextScene = NULL;

	// ������
	m_Scene->Init();
	Transition::StartTransitionIn();
}

/*******************************************************************************
*	�ŏ��̃V�[����ݒ�
*******************************************************************************/
void Manager::SetInitialScene()
{
	SetScene<DevelopLogo>(Transition::GetFirstTransitionOption());
	CheckScene();
}
