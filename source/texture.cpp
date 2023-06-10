/*******************************************************************************
*
*	�^�C�g���F	�e�N�X�`���N���X
*	�t�@�C���F	texture.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "texture.h"
#include "renderer.h"

Texture::Texture(const char* path)
{
	strcpy_s(m_Path, path);
	Load();
}

Texture::~Texture()
{
	Unload();
}

/*******************************************************************************
*	���[�h
*******************************************************************************/
void Texture::Load()
{
	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
											m_Path,
											NULL,
											NULL,
											&m_TextureResource,
											NULL);
	assert(m_TextureResource);
}

/*******************************************************************************
*	�A�����[�h
*******************************************************************************/
void Texture::Unload()
{
	m_TextureResource->Release();
}
