/*******************************************************************************
*
*	�^�C�g���F	�e�N�X�`���Ǘ��p�ÓI�N���X�@�����f���ɕt�����Ă���e�N�X�`���͊Ǘ��Ɋ܂݂܂���
*	�t�@�C���F	textureContainer.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "textureContainer.h"
#include "manager.h"
#include "scene.h"

#include <algorithm>

void TextureContainer::Init()
{
	// �f���p�A�Z�b�g
	{
		// �e���b�v
		AddTexture("Telop_StreamingAudio", "Telop\\telop1.png", 1, 1);
		AddTexture("Telop_CollisionSplit", "Telop\\telop2.png", 1, 1);
		AddTexture("Telop_GpuParticle", "Telop\\telop3.png", 1, 1);
		AddTexture("Telop_Invoke", "Telop\\telop4.png", 1, 1);
	}

	// ���f���Ƀe�N�X�`���ݒ肪�����������p�e�N�X�`��
	AddModelTexture("asset\\model\\Default\\modelTexDefault.png");

	// �ėp�A�Z�b�g
	AddTexture("Black", "Default\\black.png", 1, 1);
	AddTexture("White", "Default\\white.png", 1, 1);

	AddTexture("DevelopLogo", "Logo\\Logo.png", 1, 1);
	//AddTexture("GameLogo", "Game\\GameLogo.png", 1, 1);

	//AddTexture("Title_PromptText", "Title\\PromptText.png", 1, 1);
}

void TextureContainer::Uninit()
{
	// ���ׂẴe�N�X�`�������
	auto itr = m_TextureInfo.begin();
	while (itr != m_TextureInfo.end())
	{
		delete itr->second;
		itr = m_TextureInfo.erase(itr);
	}
}

/*******************************************************************************
*	���f���e�N�X�`���ǉ�
*******************************************************************************/
Texture* TextureContainer::AddModelTexture(string path)
{
	Texture* tex = GetTexture_InPath(path);

	if (tex == NULL)
	{
		m_ModelTextureCount++;
		string name = "ModelTex_" + to_string(m_ModelTextureCount);

		tex = DBG_NEW Texture(path.c_str());
		tex->SetWidthDiv(1);
		tex->SetHeightDiv(1);
		m_TextureInfo[name] = tex;
	}

	return tex;
}

/*******************************************************************************
*	�e�N�X�`���擾(���̂Ō���)
*******************************************************************************/
Texture* TextureContainer::GetTexture_InName(string name)
{
	auto itr = m_TextureInfo.find(name);

	if (itr != m_TextureInfo.end())
	{
		return itr->second;
	}
	else
	{
		string str = string("���݂��Ȃ����̂̃e�N�X�`���̎擾���v������܂����B\n") + name;
		MessageBox(GetWindow(),
			TEXT(str.c_str()),
			TEXT("textureContainer:Error"),
			MB_OK | MB_ICONERROR);
		return NULL;
	}
}

/*******************************************************************************
*	�e�N�X�`���擾(�p�X�Ō���)
*******************************************************************************/
Texture* TextureContainer::GetTexture_InPath(string path)
{
	auto itr = find_if(m_TextureInfo.begin(), m_TextureInfo.end(),
		[&](pair<string, Texture*> pair) { return strcmp(pair.second->GetPath(), path.c_str()) == 0; });

	if (itr != m_TextureInfo.end())
	{
		return itr->second;
	}
	else
	{
		return NULL;
	}
}

/*******************************************************************************
*	�e�N�X�`���ǉ�
*******************************************************************************/
void TextureContainer::AddTexture(string name, string path, int widthDiv, int heightDiv)
{
	string fullPath = "asset\\texture\\" + path;

	// �v�f���}�b�v�ɑ��݂��邩�m�F
	if (m_TextureInfo.count(string(name)))
	{
		string str = string("���łɑ��݂��閼�̂Ńe�N�X�`���̒ǉ����v������܂����B\n") + fullPath + "\n���̃e�N�X�`���͓ǂݍ��܂�܂���B";
		MessageBox(GetWindow(),
			TEXT(str.c_str()),
			TEXT("textureContainer:Warning"),
			MB_OK | MB_ICONERROR);
	}
	else
	{
		Texture* tex = DBG_NEW Texture(fullPath.c_str());
		tex->SetWidthDiv(widthDiv);
		tex->SetHeightDiv(heightDiv);
		m_TextureInfo[name] = tex;
	}
}
