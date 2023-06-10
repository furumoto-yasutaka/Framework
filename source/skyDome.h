/*******************************************************************************
*
*	�^�C�g���F	�X�J�C�h�[���p�ÓI�N���X
*	�t�@�C���F	skyDome.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"

#include <string>

class Model;

class SkyDome
{
private:
	static inline string				m_ModelName = "SkyDome";	// ���f������(�R���e�i��̖��O)
	static inline Model*				m_Model = NULL;			// ���f���̏ڍ׏��
	static inline ID3D11VertexShader*	m_VertexShader = NULL;		// ���_�V�F�[�_�[
	static inline ID3D11PixelShader*	m_PixelShader = NULL;		// �s�N�Z���V�F�[�_�[
	static inline ID3D11InputLayout*	m_VertexLayout = NULL;		// ���̓��C�A�E�g
	static inline float					m_Size;						// �T�C�Y

private:
	~SkyDome() {}
public:
	static void Init();
	static void Uninit();
	static void Draw();

	static void SetModel(string name);
};
