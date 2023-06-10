/*******************************************************************************
*
*	タイトル：	パーティクル生成通知クラス
*	ファイル：	particleEmitChecker.h
*	作成者：	古本 泰隆
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "particleSystem.h"

// 基底クラス
class ParticleEmitChackerBase
{
protected:
	ParticleSystem* m_ParticleSystem = NULL;

public:
	virtual ~ParticleEmitChackerBase() {}

	virtual void CheckGenerate(float elapsedTime) = 0;
	void SetParticleSystem(ParticleSystem* system) { m_ParticleSystem = system; }
};

// 一定時間ごとにパーティクルを生成する
class ParticleEmitChacker_RateOverTime : public ParticleEmitChackerBase
{
private:
	float m_CreateInterval = 0.0f;	// 生成のインターバル
	float m_NextBorder = 0.0f;		// 生成までの経過時間

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
		// 生成までの経過時間を過ぎたらパーティクルを生成
		while (elapsedTime >= m_NextBorder)
		{
			m_NextBorder += m_CreateInterval;
			
			// ↓コンピュートシェーダーでパーティクル生成↓
			m_ParticleSystem->RunConputeShader_Emit(1);
		}
	}
};

class ParticleEmitChacker_Burst : public ParticleEmitChackerBase
{
private:
	float m_CreateStartTime = 0.0f;	// 生成開始待機時間
	int m_CreateParticles = 0;		// 生成1回あたりのパーティクル数
	int m_CreateCycles = 0;			// 生成回数
	int m_CreateCyclesCount = 0;	// 残り生成回数
	float m_CreateInterval = 0.0f;	// 生成のインターバル
	float m_NextBorder = 0.0f;		// 生成までの経過時間
	float m_Probability = 1.0f;		// 生成が発生する確率

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
		// 生成開始待機時間と生成までの経過時間を過ぎたらパーティクルを生成
		if (elapsedTime > m_CreateStartTime && elapsedTime > m_NextBorder)
		{
			// 生成回数がまだ残っているかつ指定した確率でパーティクルを生成
			if (m_CreateCyclesCount > 0 && m_Probability >= Math::Random(1.0f))
			{
				m_NextBorder += m_CreateInterval;
				m_CreateCyclesCount--;

				// ↓コンピュートシェーダーでパーティクル生成↓
				m_ParticleSystem->RunConputeShader_Emit(m_CreateParticles);
			}
		}
	}
};
