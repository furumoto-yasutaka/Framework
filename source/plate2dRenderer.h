/*******************************************************************************
*
*	�^�C�g���F	2D�|���S���`��R���|�[�l���g
*	�t�@�C���F	plate2dRenderer.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "draw2dComponent.h"
#include "renderer.h"

#include <vector>
#include <string>

class Texture;
class TextureAnimation;

class Plate2DRenderer : public Draw2DComponent
{
public:
	// �s�{�b�g�ʒu
	enum class AnchorPosition
	{
		Left_Top,		// ����
		Center_Middle,	// ����
	};

private:
	string				m_TextureName;			// �e�N�X�`���̖���(�R���e�i��̖��O)
	Texture*			m_Texture = NULL;		// �e�N�X�`���̏ڍ׏��
	ID3D11Buffer*		m_VertexBuffer = NULL;	// ���_�o�b�t�@
	ID3D11VertexShader* m_VertexShader = NULL;	// ���_�V�F�[�_�[
	ID3D11PixelShader*	m_PixelShader = NULL;	// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout*	m_VertexLayout = NULL;	// ���̓��C�A�E�g

	AnchorPosition				m_AnchorPosition = AnchorPosition::Left_Top;		// �s�{�b�g�ʒu
	D3DXVECTOR2					m_Size;					// �`��T�C�Y
	D3DXVECTOR2					m_TexCoordBegin;		// �e�N�X�`�����W(�n�_)
	D3DXVECTOR2					m_TexCoordDistance;		// �e�N�X�`�����W(�I�_)
	D3DXCOLOR					m_TexColor;				// �e�N�X�`���J���[
	TextureAnimation*			m_Animation = NULL;		// �ڑ����̃e�N�X�`���A�j���[�V�����R���|�[�l���g

public:
	Plate2DRenderer(GameObject* attachObject)
		: Draw2DComponent(attachObject)
	{}
	~Plate2DRenderer() {}

	void Init(const char* textureName, D3DXVECTOR2 size,
		AnchorPosition anchorPos = AnchorPosition::Left_Top);
	void Uninit() override;
	void Update() override {}
	void Draw2d() override;

	// �e�N�X�`���A�j���[�V�����Ɛڑ�
	void Connect(TextureAnimation* animation,
		D3DXVECTOR2*& texCoordBegin, D3DXVECTOR2*& texCoordDistance,
		int& widthDiv, int& heightDiv);

	// �e�N�X�`���A�j���[�V�����Ƃ̐ڑ�������
	void DisConnect();

	// ���g�̍폜��\��
	void SetDestroy() override;

private:
	// ���_�����擾
	void GetVertex(VERTEX_3D* vertex);

	// ���_���𐶐�
	void CreateVertex(VERTEX_3D* vertex, D3DXVECTOR2 offset);

public:
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	string GetTextureName() { return m_TextureName; }

	Texture* GetTexture() { return m_Texture; }
	void SetTexture(string name);

	AnchorPosition GetAnchorPosition() { return m_AnchorPosition; }
	void SetAnchorPosition(AnchorPosition anchorPos) { m_AnchorPosition = anchorPos; }

	D3DXVECTOR2 GetSize() { return m_Size; }
	void SetSize(D3DXVECTOR2 size) { m_Size = size; }
	void SetSizeX(float x) { m_Size.x = x; }
	void SetSizeY(float y) { m_Size.y = y; }

	D3DXVECTOR2 GetTexCoordBegin() { return m_TexCoordBegin; }
	void SetTexCoordBegin(D3DXVECTOR2 coord) { m_TexCoordBegin = coord; }
	void SetTexCoordBeginX(float x) { m_TexCoordBegin.x = x; }
	void SetTexCoordBeginY(float y) { m_TexCoordBegin.y = y; }

	D3DXVECTOR2 GetTexCoordDistance() { return m_TexCoordDistance; }
	void SetTexCoordDistance(D3DXVECTOR2 coord) { m_TexCoordDistance = coord; }
	void SetTexCoordDistanceX(float x) { m_TexCoordDistance.x = x; }
	void SetTexCoordDistanceY(float y) { m_TexCoordDistance.y = y; }

	D3DXCOLOR GetTexColor() { return m_TexColor; }
	void SetTexColor(D3DXCOLOR color) { m_TexColor = color; }
	void SetTexAlphaColor(float alpha) { m_TexColor.a = alpha; }
	void SetTexRGBColor(float red, float green, float blue)
	{
		m_TexColor.r = red;
		m_TexColor.g = green;
		m_TexColor.b = blue;
	}

	TextureAnimation* GetTextureAnimation() { return m_Animation; }
	/////////////////////////////

#ifdef _DEBUG
private:
	const vector<string> m_ANCHOR_POSITION_STR = {
		"Left_Top",
		"Center_Middle",
	};

public:
	void DrawInspector() override;
#endif
};