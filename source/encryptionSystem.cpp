/*******************************************************************************
*
*	�^�C�g���F	�Í����p�ÓI�N���X
*	�t�@�C���F	encryptionSystem.cpp
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#include "encryptionSystem.h"

void EncryptionSystem::Init()
{

}

void EncryptionSystem::Uninit()
{

}

vector<uint8_t> EncryptionSystem::Encryption(vector<uint8_t> binary)
{
	//// �����F�Í�������
	//vector<uint8_t> secret(16, 1);

	//// ���������p
	//random_device rd;
	//uniform_int_distribution<int> uid(0, 0xff);

	//// �Í��𕡎G�ɂ��邽�߂̃����_���ȃo�C�g(�������x�N�g��)
	//vector<uint8_t> iv0, iv1;
	//for (int i = 0; i < 16; i++)
	//{
	//	int temp = uid(rd);
	//	iv0.push_back(temp);
	//	//temp = uid(rd);
	//	iv1.push_back(temp);
	//}

	//vector<uint8_t> encrypted; // �Í�����f�[�^
	//vector<uint8_t> decrypted; // ��������f�[�^

	//// �Í����A���S���Y���v���o�C�_���J��
	//BCRYPT_ALG_HANDLE alg = NULL;
	//(void)BCryptOpenAlgorithmProvider(&alg, L"AES", NULL, 0);

	//if (alg != NULL)
	//{
	//	ULONG keyLength = 0;	// �L�[�̃T�C�Y(�A���S���Y���ɂ���ăT�C�Y���ω�����)
	//	ULONG temp;				// �l�͎g��Ȃ����G���[���o�邽�߈ꎞ�ϐ������

	//	// �L�[�̃T�C�Y���擾
	//	(void)BCryptGetProperty(alg, L"ObjectLength", reinterpret_cast<PUCHAR>(&keyLength), sizeof(keyLength), &temp, 0);

	//	// �L�[�I�u�W�F�N�g���쐬
	//	BCRYPT_KEY_HANDLE key = NULL;			// �Í����Ɏg�p����L�[�n���h��
	//	vector<uint8_t> keyObject(keyLength);	// �L�[�I�u�W�F�N�g
	//	(void)BCryptGenerateSymmetricKey(alg, &key, &keyObject[0], keyLength, &secret[0], 16, 0);

	//	if (key != NULL)
	//	{
	//		// �Í�������f�[�^������
	//		vector<uint8_t> temp = json::to_msgpack(j);	// Json����U�o�C�i���ɕϊ�
	//		size_t textLen = temp.size() + 1;			// �o�C�g�����擾(+1��NULL����)

	//		// �Í�����̃T�C�Y���m�F���ė̈���m�ۂ��Ă���Í������s��
	//		uint8_t* plainText = temp.data();	// �^�̓s���������̂�vector����|�C���^���擾
	//		ULONG encryptedSize;
	//		(void)BCryptEncrypt(key, plainText, textLen, NULL, &iv0[0], 16, NULL, 0, &encryptedSize, 0x00000001);
	//		encrypted.resize(encryptedSize);
	//		(void)BCryptEncrypt(key, plainText, textLen, NULL, &iv0[0], 16, &encrypted[0], encryptedSize, &encryptedSize, 0x00000001);

	//		// ��������̃T�C�Y���m�F���ė̈���m�ۂ��Ă��畜�����s��
	//		uint8_t* encryptedText = encrypted.data();	// �^�̓s���������̂�vector����|�C���^���擾
	//		ULONG decryptedSize;
	//		(void)BCryptDecrypt(key, encryptedText, encryptedSize, NULL, &iv1[0], 16, NULL, 0, &decryptedSize, 0x00000001);
	//		decrypted.resize(decryptedSize);
	//		(void)BCryptDecrypt(key, encryptedText, encryptedSize, NULL, &iv1[0], 16, &decrypted[0], decryptedSize, &decryptedSize, 0x00000001);

	//		(void)BCryptDestroyKey(key);	// �g���I������Í����L�[��j������
	//	}
	//	// �Í����A���S���Y���v���o�C�_�����
	//	(void)BCryptCloseAlgorithmProvider(alg, 0);
	//}
	//else
	//{
	//	return;
	//}


	////// �����F���̂܂�Json�o��
	////ofstream ofs("asset\\json\\savedata.json");
	////
	////ofs << setw(4) << j << endl;


	////// �����F�o�C�i���o��
	////ofstream ofs("asset\\json\\savedata.bin");
	////vector<uint8_t> v_msgpack = json::to_msgpack(j);
	////for (uint8_t t : v_msgpack)
	////{
	////	ofs << t;
	////}


	////// �����F�o�C�i���ɕϊ�����json�ɂ܂��߂��Ă���o��
	////ofstream ofs("asset\\json\\savedata.json");
	////vector<uint8_t> v_msgpack = json::to_msgpack(j);
	////json j2 = json::from_msgpack(v_msgpack);
	////ofs << setw(4) << j2 << endl;


	////// �����F�Í�����Ԃŏo��
	////ofstream ofs("asset\\json\\savedata.bin");
	////for (uint8_t t : encrypted)
	////{
	////	ofs << t;
	////}


	//// �����F�Í�����ɕ������Ă���o��
	//ofstream ofs("asset\\json\\savedata.json");
	//json j2 = json::from_msgpack(decrypted, false);
	//ofs << setw(4) << j2 << endl;
	
	return vector<uint8_t>();
}

vector<uint8_t> EncryptionSystem::Decryption(vector<uint8_t> binary)
{
	return vector<uint8_t>();
}
