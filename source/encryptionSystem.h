/*******************************************************************************
*
*	�^�C�g���F	�Í����p�ÓI�N���X
*	�t�@�C���F	encryptionSystem.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"

#include <vector>

#include <nlohmann/json.hpp>

using namespace nlohmann;

class EncryptionSystem
{
private:
	static inline vector<uint8_t> m_Vector;
	static inline BCRYPT_KEY_HANDLE m_KeyHandle;
	static inline vector<uint8_t> m_KeyObject;

private:
	EncryptionSystem() {}
public:
	static void Init();
	static void Uninit();

	// �Í���
	static vector<uint8_t> Encryption(vector<uint8_t> binary);
	// ������
	static vector<uint8_t> Decryption(vector<uint8_t> binary);
};
