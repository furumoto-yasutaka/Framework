/*******************************************************************************
*
*	タイトル：	テクスチャクラス
*	ファイル：	texture.cpp
*	作成者：	古本 泰隆
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
*	ロード
*******************************************************************************/
void Texture::Load()
{
	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
											m_Path,
											NULL,
											NULL,
											&m_TextureResource,
											NULL);
	assert(m_TextureResource);
}

/*******************************************************************************
*	アンロード
*******************************************************************************/
void Texture::Unload()
{
	m_TextureResource->Release();
}
