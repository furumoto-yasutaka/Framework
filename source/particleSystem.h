/*******************************************************************************
*
*	タイトル：	パーティクル生成コンポーネント
*	ファイル：	particleSystem.h
*	作成者：	古本 泰隆
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
		World = 0,	// ワールドマトリクスをデフォルトに設定してパーティクルのパラメータに代入する
		Local,		// ワールドマトリクスをいつも通り設定する
	};
	enum class EmitterStopActionId
	{
		ComponentDisable = 0,
		ObjectDestroy,
		ComponentDestroy,
	};
	enum class ParticleDirectionModeId
	{
		BillboardDirection = 0,	// カメラの方向に向ける
		RandomizeDirection,		// パーティクルをランダムな方向に向ける
		AlignToDirection,		// 最初に設定した方向を軸に回転してカメラへ向ける
	};

	struct EmitParam
	{
		bool					IsLooping;			// ループフラグ
		float					Duration;			// エミッターの1サイクルの時間
		float					DurationCount;		// エミッターのサイクルのカウント
		unsigned int			MaxParticles;		// 同時に存在可能な最大パーティクル数を指定する
		unsigned int			ParticleCount;		// 現在存在するパーティクル数
		D3DXVECTOR3				Offset;				// エミッターの座標差異(パーティクル生成時にパーティクルの座標に足し合わせる)
		SimulationSpaceId		SimulationSpace;	// エミッターの座標基準
		EmitterStopActionId		StopAction;			// エフェクト終了時の挙動
		CallbackBase<>*			StopCallback;		// コールバック関数

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
		bool			IsActive;	// 使用中かどうか
		float			Lifetime;	// パーティクル発生から消滅までの時間
		float			Size;		// パーティクルのサイズ
		D3DXVECTOR3		Position;	// パーティクルの座標
		D3DXVECTOR3		Velocity;	// パーティクルの初速
		D3DXVECTOR3		Gravity;	// パーティクルにかかる重力
		D3DXVECTOR4		Color;		// パーティクルの色
	};

	struct ParticleDefaultParam
	{
		float						Lifetime;		// パーティクル発生から消滅までの時間
		Math::MinMax<float>			StartSize;		// パーティクルのサイズ
		D3DXVECTOR3					Position;		// パーティクルの初期座標(ゲームオブジェクトの座標とオフセットを足したもの※毎フレーム更新の必要あり)
		Math::MinMax<D3DXVECTOR3>	StartVelocity;	// パーティクルの初速
		D3DXVECTOR3					Gravity;		// パーティクルにかかる重力
		string						TexName;		// テクスチャ名
		Texture*					Tex;			// パーティクルのテクスチャ
		D3DXVECTOR4					Color;			// パーティクルの色

		ParticleDefaultParam();
		ParticleDefaultParam(float lifetime, Math::MinMax<float> startSize,
			Math::MinMax<D3DXVECTOR3> startVelocity,
			D3DXVECTOR3 gravity, string texName, D3DXVECTOR4 color);
	};

	struct ParticleDefaultParamSRV
	{
		float			Lifetime;			// パーティクル発生から消滅までの時間
		float			StartSizeMax;		// パーティクルのサイズ
		float			StartSizeMin;		// パーティクルのサイズ
		D3DXVECTOR3		Position;			// パーティクルの初期座標(ゲームオブジェクトの座標とオフセットを足したもの※毎フレーム更新の必要あり)
		D3DXVECTOR3		StartVelocityMax;	// パーティクルの初速
		D3DXVECTOR3		StartVelocityMin;	// パーティクルの初速
		D3DXVECTOR3		Gravity;			// パーティクルにかかる重力
		D3DXVECTOR4		Color;				// パーティクルの色
	};

private:
	bool						m_IsPlay = false;
	EmitParam					m_EmitterParam;				// エミッターのパラメータ
	ParticleDefaultParam		m_ParticleDefaultParam;		// パーティクルのデフォルトパラメータ
	ParticleEmitChackerBase*	m_EmitChacker = NULL;		// パーティクル生成通知クラス
	
	// コンピュートシェーダー
	ID3D11ComputeShader*		m_ComputeShader_Init = NULL;
	ID3D11ComputeShader*		m_ComputeShader_Emit = NULL;
	ID3D11ComputeShader*		m_ComputeShader_Update = NULL;

	// コンピュートシェーダーリソース
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

	// 描画用リソース
	ID3D11Buffer*				m_VertexBuffer = NULL;		// 頂点バッファ
	ID3D11VertexShader*			m_VertexShader = NULL;		// 頂点シェーダー
	ID3D11PixelShader*			m_PixelShader = NULL;		// ピクセルシェーダー
	ID3D11InputLayout*			m_VertexLayout = NULL;		// 入力レイアウト

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
