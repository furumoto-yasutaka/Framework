/*******************************************************************************
*
*	�^�C�g���F	�p�[�e�B�N�������R���|�[�l���g
*	�t�@�C���F	particleSystem.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "draw3dComponent.h"
#include "callbackListner.h"

#include <string>
#include <list>

class Texture;
class ParticleEmitChackerBase;

class ParticleSystem : public Draw3DComponent
{
public:
	enum class SimulationSpaceId
	{
		World = 0,	// ���[���h�}�g���N�X���f�t�H���g�ɐݒ肵�ăp�[�e�B�N���̃p�����[�^�ɑ������
		Local,		// ���[���h�}�g���N�X�������ʂ�ݒ肷��
	};
	enum class EmitterStopActionId
	{
		ComponentDisable = 0,
		ObjectDestroy,
		ComponentDestroy,
	};
	enum class ParticleDirectionModeId
	{
		BillboardDirection = 0,	// �J�����̕����Ɍ�����
		RandomizeDirection,		// �p�[�e�B�N���������_���ȕ����Ɍ�����
		AlignToDirection,		// �ŏ��ɐݒ肵�����������ɉ�]���ăJ�����֌�����
	};

	struct EmitParam
	{
		bool					IsLooping;			// ���[�v�t���O
		float					Duration;			// �G�~�b�^�[��1�T�C�N���̎���
		float					DurationCount;		// �G�~�b�^�[�̃T�C�N���̃J�E���g
		unsigned int			MaxParticles;		// �����ɑ��݉\�ȍő�p�[�e�B�N�������w�肷��
		unsigned int			ParticleCount;		// ���ݑ��݂���p�[�e�B�N����
		D3DXVECTOR3				Offset;				// �G�~�b�^�[�̍��W����(�p�[�e�B�N���������Ƀp�[�e�B�N���̍��W�ɑ������킹��)
		SimulationSpaceId		SimulationSpace;	// �G�~�b�^�[�̍��W�
		EmitterStopActionId		StopAction;			// �G�t�F�N�g�I�����̋���
		CallbackBase<>*			StopCallback;		// �R�[���o�b�N�֐�

		EmitParam();
		EmitParam(bool isLooping, float duration, int maxParticles,
			D3DXVECTOR3 offset, D3DXVECTOR3 rotation,
			SimulationSpaceId simulationSpace, EmitterStopActionId stopAction)
		{
			this->IsLooping = isLooping;
			this->Duration = duration;
			this->DurationCount = duration;
			this->MaxParticles = maxParticles;
			this->ParticleCount = 0;
			this->Offset = offset;
			this->SimulationSpace = simulationSpace;
			this->StopAction = stopAction;
			this->StopCallback = NULL;
		}
		template<class T>
		EmitParam(bool isLooping, float duration, int maxParticles, D3DXVECTOR3 offset,
			D3DXVECTOR3 rotation, SimulationSpaceId simulationSpace,
			EmitterStopActionId stopAction, T* callbackOrigin, void(T::* callBackFunction)())
		{
			this->IsLooping = isLooping;
			this->Duration = duration;
			this->DurationCount = duration;
			this->MaxParticles = maxParticles;
			this->ParticleCount = 0;
			this->Offset = offset;
			this->SimulationSpace = simulationSpace;
			this->StopAction = stopAction;
			if (callbackOrigin && callBackFunction)
			{
				this->StopCallback = DBG_NEW Callback(callbackOrigin, callBackFunction);
			}
			else
			{
				this->StopCallback = NULL;
			}
		}
	};

	struct ParticleParam
	{
		bool			IsActive;	// �g�p�����ǂ���
		float			Lifetime;	// �p�[�e�B�N������������ł܂ł̎���
		float			Size;		// �p�[�e�B�N���̃T�C�Y
		D3DXVECTOR3		Position;	// �p�[�e�B�N���̍��W
		D3DXVECTOR3		Velocity;	// �p�[�e�B�N���̏���
		D3DXVECTOR3		Gravity;	// �p�[�e�B�N���ɂ�����d��
		D3DXVECTOR4		Color;		// �p�[�e�B�N���̐F
	};

	struct ParticleDefaultParam
	{
		float						Lifetime;		// �p�[�e�B�N������������ł܂ł̎���
		Math::MinMax<float>			StartSize;		// �p�[�e�B�N���̃T�C�Y
		D3DXVECTOR3					Position;		// �p�[�e�B�N���̏������W(�Q�[���I�u�W�F�N�g�̍��W�ƃI�t�Z�b�g�𑫂������́����t���[���X�V�̕K�v����)
		Math::MinMax<D3DXVECTOR3>	StartVelocity;	// �p�[�e�B�N���̏���
		D3DXVECTOR3					Gravity;		// �p�[�e�B�N���ɂ�����d��
		string						TexName;		// �e�N�X�`����
		Texture*					Tex;			// �p�[�e�B�N���̃e�N�X�`��
		D3DXVECTOR4					Color;			// �p�[�e�B�N���̐F

		ParticleDefaultParam();
		ParticleDefaultParam(float lifetime, Math::MinMax<float> startSize,
			Math::MinMax<D3DXVECTOR3> startVelocity,
			D3DXVECTOR3 gravity, string texName, D3DXVECTOR4 color);
	};

	struct ParticleDefaultParamSRV
	{
		float			Lifetime;			// �p�[�e�B�N������������ł܂ł̎���
		float			StartSizeMax;		// �p�[�e�B�N���̃T�C�Y
		float			StartSizeMin;		// �p�[�e�B�N���̃T�C�Y
		D3DXVECTOR3		Position;			// �p�[�e�B�N���̏������W(�Q�[���I�u�W�F�N�g�̍��W�ƃI�t�Z�b�g�𑫂������́����t���[���X�V�̕K�v����)
		D3DXVECTOR3		StartVelocityMax;	// �p�[�e�B�N���̏���
		D3DXVECTOR3		StartVelocityMin;	// �p�[�e�B�N���̏���
		D3DXVECTOR3		Gravity;			// �p�[�e�B�N���ɂ�����d��
		D3DXVECTOR4		Color;				// �p�[�e�B�N���̐F
	};

private:
	bool						m_IsPlay = false;
	EmitParam					m_EmitterParam;				// �G�~�b�^�[�̃p�����[�^
	ParticleDefaultParam		m_ParticleDefaultParam;		// �p�[�e�B�N���̃f�t�H���g�p�����[�^
	ParticleEmitChackerBase*	m_EmitChacker = NULL;		// �p�[�e�B�N�������ʒm�N���X
	
	// �R���s���[�g�V�F�[�_�[
	ID3D11ComputeShader*		m_ComputeShader_Init = NULL;
	ID3D11ComputeShader*		m_ComputeShader_Emit = NULL;
	ID3D11ComputeShader*		m_ComputeShader_Update = NULL;

	// �R���s���[�g�V�F�[�_�[���\�[�X
	ID3D11Buffer*				m_ParticleParamBuf = NULL;
	ID3D11ShaderResourceView*	m_ParticleParamSRV = NULL;
	ID3D11UnorderedAccessView*	m_ParticleParamUAV = NULL;

	ID3D11Buffer*				m_ParticlePoolBuf = NULL;
	ID3D11UnorderedAccessView*	m_ParticlePoolUAV = NULL;

	ID3D11Buffer*				m_ParticlePoolCountBuf = NULL;
	ID3D11UnorderedAccessView*	m_ParticlePoolCountUAV = NULL;

	ID3D11Buffer*				m_ParticleDefaultParamBuf = NULL;
	ID3D11ShaderResourceView*	m_ParticleDefaultParamSRV = NULL;

	ID3D11Buffer*				m_DeltaTimeBuf = NULL;
	ID3D11ShaderResourceView*	m_DeltaTimeSRV = NULL;

	ID3D11Buffer*				m_RandomTableBuf = NULL;
	ID3D11ShaderResourceView*	m_RandomTableSRV = NULL;

	ID3D11Buffer*				m_RandomSeedBuf = NULL;
	ID3D11UnorderedAccessView*	m_RandomSeedUAV = NULL;

	// �`��p���\�[�X
	ID3D11Buffer*				m_VertexBuffer = NULL;		// ���_�o�b�t�@
	ID3D11VertexShader*			m_VertexShader = NULL;		// ���_�V�F�[�_�[
	ID3D11PixelShader*			m_PixelShader = NULL;		// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout*			m_VertexLayout = NULL;		// ���̓��C�A�E�g

public:
	ParticleSystem(GameObject* attachObject)
		: Draw3DComponent(attachObject, -100)
	{}
	~ParticleSystem() {}

	void Init(float generateDelay, bool isPlayOnAwake,
		ParticleSystem::EmitParam emitterParam,
		ParticleDefaultParam particleDefaltParam,
		ParticleEmitChackerBase* particleEmitChacker);
	void Uninit() override;
	void Update() override;
	void LateUpdate() override;
	void Draw3d() override;

	void Play();
	void Pause();
	void Stop();

	void RunConputeShader_Emit(int count);

private:
	ID3D11ShaderResourceView* CreateSRV(ID3D11Buffer* buf, int numElements);
	ID3D11UnorderedAccessView* CreateUAV(ID3D11Buffer* buf, int numElements, unsigned int bufferFlag);

	void CreateShader();
	void CreateVertex();
	void CreateBuffer();

	void RunComputeShader_Init();
	void RunComputeShader_Update();

	void ResetComputeShader();

	void CheckEmitStop();
	void EndSystem();

	ID3D11Buffer* CreateAndCopyToDebugBuf(ID3D11Buffer* buffer);
};
