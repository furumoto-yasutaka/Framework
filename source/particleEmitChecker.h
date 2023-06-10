/*******************************************************************************
*
*	�^�C�g���F	�p�[�e�B�N�������ʒm�N���X
*	�t�@�C���F	particleEmitChecker.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "particleSystem.h"

// ���N���X
class ParticleEmitChackerBase
{
protected:
	ParticleSystem* m_ParticleSystem = NULL;

public:
	virtual ~ParticleEmitChackerBase() {}

	virtual void CheckGenerate(float elapsedTime) = 0;
	void SetParticleSystem(ParticleSystem* system) { m_ParticleSystem = system; }
};

// ��莞�Ԃ��ƂɃp�[�e�B�N���𐶐�����
class ParticleEmitChacker_RateOverTime : public ParticleEmitChackerBase
{
private:
	float m_CreateInterval = 0.0f;	// �����̃C���^�[�o��
	float m_NextBorder = 0.0f;		// �����܂ł̌o�ߎ���

public:
	ParticleEmitChacker_RateOverTime(float createInterval)
		: m_CreateInterval(createInterval),
		m_NextBorder(createInterval)
	{
		assert(m_CreateInterval > 0.0f);
	}
	~ParticleEmitChacker_RateOverTime() {}

	void CheckGenerate(float elapsedTime) override
	{
		// �����܂ł̌o�ߎ��Ԃ��߂�����p�[�e�B�N���𐶐�
		while (elapsedTime >= m_NextBorder)
		{
			m_NextBorder += m_CreateInterval;
			
			// ���R���s���[�g�V�F�[�_�[�Ńp�[�e�B�N��������
			m_ParticleSystem->RunConputeShader_Emit(1);
		}
	}
};

class ParticleEmitChacker_Burst : public ParticleEmitChackerBase
{
private:
	float m_CreateStartTime = 0.0f;	// �����J�n�ҋ@����
	int m_CreateParticles = 0;		// ����1�񂠂���̃p�[�e�B�N����
	int m_CreateCycles = 0;			// ������
	int m_CreateCyclesCount = 0;	// �c�萶����
	float m_CreateInterval = 0.0f;	// �����̃C���^�[�o��
	float m_NextBorder = 0.0f;		// �����܂ł̌o�ߎ���
	float m_Probability = 1.0f;		// ��������������m��

public:
	ParticleEmitChacker_Burst(
		float createStartTime = 1.0f, int createParticles = 1,
		int createCycles = 10, float createInterval = 1.0f, float probability = 1.0f)
		: m_CreateStartTime(createStartTime),
		m_CreateParticles(createParticles),
		m_CreateCycles(createCycles),
		m_CreateCyclesCount(createCycles),
		m_CreateInterval(createInterval),
		m_NextBorder(createStartTime),
		m_Probability(probability)
	{
		assert(m_CreateStartTime >= 0.0f);
		assert(m_CreateCycles >= 0);
		assert(m_CreateInterval > 0.0f);
		assert(m_Probability >= 0.0f);
	}
	~ParticleEmitChacker_Burst() {}

	void CheckGenerate(float elapsedTime) override
	{
		// �����J�n�ҋ@���ԂƐ����܂ł̌o�ߎ��Ԃ��߂�����p�[�e�B�N���𐶐�
		if (elapsedTime > m_CreateStartTime && elapsedTime > m_NextBorder)
		{
			// �����񐔂��܂��c���Ă��邩�w�肵���m���Ńp�[�e�B�N���𐶐�
			if (m_CreateCyclesCount > 0 && m_Probability >= Math::Random(1.0f))
			{
				m_NextBorder += m_CreateInterval;
				m_CreateCyclesCount--;

				// ���R���s���[�g�V�F�[�_�[�Ńp�[�e�B�N��������
				m_ParticleSystem->RunConputeShader_Emit(m_CreateParticles);
			}
		}
	}
};
