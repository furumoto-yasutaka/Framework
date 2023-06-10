/*******************************************************************************
*
*	�^�C�g���F	���f���N���X
*	�t�@�C���F	model.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"
#include "renderer.h"
#include "texture.h"

// �}�e���A���\����
struct MODEL_MATERIAL
{
	char			Name[256];
	MATERIAL		Material;
	Texture*		Tex;
};

// �`��T�u�Z�b�g�\����
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};

// ���f���\����
struct MODEL
{
	VERTEX_3D*		VertexArray;
	unsigned int	VertexNum;

	unsigned int*	IndexArray;
	unsigned int	IndexNum;

	SUBSET*			SubsetArray;
	unsigned int	SubsetNum;
};

class Model
{
private:
	char			m_Path[256];			// ���΃p�X
	float			m_Scale = 1.0f;			// �X�P�[��

	ID3D11Buffer*	m_VertexBuffer = NULL;	// ���_�o�b�t�@
	ID3D11Buffer*	m_IndexBuffer = NULL;	// �C���f�b�N�X�o�b�t�@
	SUBSET*			m_SubsetArray = NULL;	// �T�u�Z�b�g
	unsigned int	m_SubsetNum = NULL;		// �T�u�Z�b�g��

public:
	Model(const char* Path, float Scale);
	~Model();

	void Draw();

	// ���[�h
	void Load();
	// �A�����[�h
	void Unload();
	
	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	char* GetPath() { return m_Path; }

	float GetScale() { return m_Scale; }
	/////////////////////////////
private:
	// obj�t�@�C���ǂݍ���
	void LoadObj(const char* fileName, MODEL* model);
	// mtl�t�@�C���ǂݍ���
	void LoadMaterial(const char* fileName, MODEL_MATERIAL** materialArray, unsigned int* materialNum);
};
