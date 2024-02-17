/*******************************************************************************
*
*	�^�C�g���F	�e�N�X�`���u���[�R���|�[�l���g
*	�t�@�C���F	plate2dBlur.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "draw2dComponent.h"
#include "renderer.h"

#include <list>

class Plate2DRenderer;

class Plate2DBlur : public Draw2DComponent
{
private:
	Plate2DRenderer*	m_Plate2DRenderer = NULL;		// �A�g����2D�`��R���|�[�l���g
	ID3D11Buffer*		m_VertexBuffer = NULL;			// ���_�o�b�t�@
	ID3D11VertexShader* m_VertexShader = NULL;			// ���_�V�F�[�_�[
	ID3D11PixelShader*	m_PixelShader = NULL;			// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout*	m_VertexLayout = NULL;			// ���̓��C�A�E�g

	int					m_BulrLayer = 0;				// �u���[����
	int					m_BulrInterval = 0;				// �u���[�����Ԋu(�t���[��)
	int					m_InterpolationLayer = 0;		// �u���[��Ԗ���
	float				m_BulrAlphaDecayRate = 0.0f;	// 1��������̃A���t�@������
	int					m_FrameCount = 0;				// �t���[���J�E���^

	list<D3DXVECTOR2>	m_BulrPosition;					// �u���[�����ʒu
	list<float>			m_BulrRotationZ;				// �u���[�̉�]

public:
	Plate2DBlur(GameObject* AttachObject)
		: Draw2DComponent(AttachObject, -100)
	{}
	~Plate2DBlur() {}

	void Init(Plate2DRenderer* plate2D, unsigned int bulrLayer, unsigned int bulrInterval, unsigned int interpolationBlur = 0);
	void Uninit() override;
	void Update() override;
	void Draw2d() override;

private:
	void CreateBuffer();
	void CreateShader();

	// �ŏ��ɐ��������u���[���폜����
	void ClearBackBulr();
	// �u���[��ǉ�
	void CreateBulr();

	// ���_�����擾
	void GetVertex(VERTEX_3D* vertex, D3DXVECTOR2 pos, float alpha, float rot);

	// ���_���𐶐�
	void CreateVertex(VERTEX_3D* vertex, D3DXVECTOR2 pos, D3DXVECTOR2 offset, float alpha, float rot);

public:
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	int GetBlurLayer() { return m_BulrLayer; }
	void SetBlurLayer(int Layer) { m_BulrLayer = Layer; }

	int GetBulrInterval() { return m_BulrInterval; }
	void SetBlurInterval(int Interval) { m_BulrInterval = Interval; }

	int GetInterpolationLayer() { return m_InterpolationLayer; }
	void SetInterpolationLayer(int Layer) { m_InterpolationLayer = Layer; }

	float GetBulrAlphaDecayRate() { return m_BulrAlphaDecayRate; }
	void SetBulrAlphaDecayRate(float Rate) { m_BulrAlphaDecayRate = Rate; }

	int GetFrameCount() { return m_FrameCount; }
	/////////////////////////////

#ifdef _DEBUG
public:
	void DrawInspector() override;
#endif
};
