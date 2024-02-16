/*******************************************************************************
*
*	�^�C�g���F	�J�ډ��o�p�ÓI�N���X
*	�t�@�C���F	transition.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "renderer.h"
#include "textureContainer.h"
#include "callbackListner.h"
#include "draw2dComponent.h"

class Texture;

class Transition : public Draw2DComponent
{
public:
	// �g�����W�V�����̌`��
	enum class TransitionPattern
	{
		None = 0,	// �ς��Ɛ؂�ւ�
		Fade,		// �t�F�[�h
		Wipe,
	};
	// �g�����W�V�����̏�
	enum class TransitionState
	{
		Out = 0,		// ��ʂ��B���Ă���
		In,				// ��ʂ������Ă���
		OutComplete,	// ��ʂ��B�ꂽ��ԂɂȂ���
		None,			// �������E��ʂ��������ԂɂȂ���
		Length,
	};

	// �g�����W�V�����ݒ�p�\����
	struct TransitionOption
	{
		Transition::TransitionPattern	Mode[2];			// ���݂͖��g�p
		float							TimeLimitSec[2];
		Texture*						Texture;

		TransitionOption()
		{
			for (int i = 0; i < 2; i++)
			{
				Mode[i] = TransitionPattern::None;
				TimeLimitSec[i] = 0.0f;
			}
			Texture = NULL;
		}
	};

private:
	static TransitionOption	m_Option;			// �g�����W�V�����ݒ�

	ID3D11Buffer*		m_VertexBuffer = NULL;	// ���_�o�b�t�@
	ID3D11VertexShader* m_VertexShader = NULL;	// ���_�V�F�[�_�[
	ID3D11PixelShader*	m_PixelShader = NULL;	// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout*	m_VertexLayout = NULL;	// ���̓��C�A�E�g

	TransitionState		m_State = TransitionState::None;	// �g�����W�V�����̏�
	float				m_TimeCountSec = 0.0f;	// �o�ߎ���
	CallbackBase<>*		m_Callback = NULL;		// �R�[���o�b�N�֐�(�V�[�����؂�ւ�����u�ԌĂ΂��)

public:
	Transition(GameObject* attachObject)
		: Draw2DComponent(attachObject)
	{}
	~Transition() {}

	void Init();
	void Uninit() override;
	void Update() override;
	void Draw2d() override;

	// �V�[���؂�ւ��O�g�����W�V�����J�n
	void StartTransitionOut();

	// �V�[���؂�ւ���g�����W�V�����J�n
	void StartTransitionIn();

	// �Q�[���N�����̃g�����W�V�����I�v�V�������擾
	static TransitionOption GetFirstTransitionOption();

	// �R�[���o�b�N��ݒ�
	template<class T>
	void SetCallback(T* origin, void(T::* function)())
	{
		m_Callback = DBG_NEW Callback(origin, function);
	}

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	static void SetTransitionOption(TransitionOption option) { m_Option = option; }

	TransitionState GetTransitionState() { return m_State; }
	/////////////////////////////
private:
	// ���_���𐶐�
	void CreateVertex(VERTEX_3D* vertex, float alpha);
	void CreateVertex_WipeLeft(VERTEX_3D* vertex, float rate);
	void CreateVertex_WipeRight(VERTEX_3D* vertex, float rate);

	// �`�揈��
	void Draw_Fade();
	void Draw_Wipe();
};
