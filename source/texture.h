/*******************************************************************************
*
*	�^�C�g���F	�e�N�X�`���N���X
*	�t�@�C���F	texture.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"

class Texture
{
private:
	char						m_Path[256];				// ���΃p�X
	ID3D11ShaderResourceView*	m_TextureResource = NULL;	// �e�N�X�`�����\�[�X
	int							m_WidthDiv = 1;				// ��������
	int							m_HeightDiv = 1;			// �c������

public:
	Texture(const char* path);
	~Texture();
	
	// ���[�h
	void Load();
	// �A�����[�h
	void Unload();

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//
	char* GetPath() { return m_Path; }

	ID3D11ShaderResourceView* GetResource() { return m_TextureResource; }

	int GetWidthDiv() { return m_WidthDiv; }
	void SetWidthDiv(int widthDiv) { m_WidthDiv = widthDiv; }

	int GetHeightDiv() { return m_HeightDiv; }
	void SetHeightDiv(int heightDiv) { m_HeightDiv = heightDiv; }
	/////////////////////////////
};
