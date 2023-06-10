/*******************************************************************************
*
*	�^�C�g���F	�e�N�X�`���Ǘ��p�ÓI�N���X�@�����f���ɕt�����Ă���e�N�X�`���͊Ǘ��Ɋ܂݂܂���
*	�t�@�C���F	textureContainer.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "texture.h"

#include <unordered_map>
#include <memory>

class TextureContainer
{
private:
	// ���v�f
	// ����1	string�^	�e�N�X�`������(�L�[)
	// ����2	Texture*�^	�e�N�X�`�����
	static inline unordered_map<string, Texture*> m_TextureInfo;	// �e�N�X�`�����X�g
	
	static inline int m_ModelTextureCount = 0;
	static inline string m_ModelTexInitial = "ModelTex_";

private:
	TextureContainer() {}
public:
	static void Init();
	static void Uninit();

	// ���f���e�N�X�`���ǉ�
	static Texture* AddModelTexture(string path);

	/////////////////////////////
	//�@�����@�A�N�Z�T�@�����@//

	// �e�N�X�`���擾(���̂Ō���)
	static Texture* GetTexture_InName(string name);

	// �e�N�X�`���擾(�p�X�Ō���)
	static Texture* GetTexture_InPath(string path);

	static inline string GetModelTexInitial() { return m_ModelTexInitial; }
	/////////////////////////////

private:
	// �e�N�X�`���ǉ�
	static void AddTexture(string name, string path,
		int widthDiv, int heightDiv);
};
