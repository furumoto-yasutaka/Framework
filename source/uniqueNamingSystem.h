/*******************************************************************************
*
*	�^�C�g���F	��Ӗ��̐����p�ÓI�N���X
*	�t�@�C���F	uniqueNamingSystem.h
*	�쐬�ҁF	�Ö{ �ח�
*
*******************************************************************************/
#pragma once
#include "main.h"

#include <string>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>

class UniqueNamingSystem
{
private:
	static inline const unsigned long long m_MAX_NAME_COUNT = 9999;	// �d�����̂̍ő吔

private:
	UniqueNamingSystem() {}
public:
	// ��Ӗ��̐���(map)
	template<typename T>
	static string CreateUniqueName(string baseName, map<string, T> names)
	{
		unsigned int i;
		string candidate;

		// �g�p���Ă��Ȃ����O��������܂ŒT��
		for (i = 1; i < m_MAX_NAME_COUNT; i++)
		{
			candidate = string(baseName) + " (" + to_string(i) + ")";
			if (names.find(candidate) == names.end()) { return candidate; }
		}

		CreateLimitError();
		return baseName;
	}

	// ��Ӗ��̐���(unordered_map)
	template<typename T>
	static string CreateUniqueName(string baseName, unordered_map<string, T> names)
	{
		unsigned int i;
		string candidate;

		// �g�p���Ă��Ȃ����O��������܂ŒT��
		for (i = 1; i < m_MAX_NAME_COUNT; i++)
		{
			candidate = string(baseName) + " (" + to_string(i) + ")";
			if (names.find(candidate) == names.end()) { return candidate; }
		}

		CreateLimitError();
		return baseName;
	}

	// ��Ӗ��̐���(multimap)
	template<typename T>
	static string CreateUniqueName(string baseName, multimap<string, T> names)
	{
		unsigned int i;
		string candidate;

		// �g�p���Ă��Ȃ����O��������܂ŒT��
		for (i = 1; i < m_MAX_NAME_COUNT; i++)
		{
			candidate = string(baseName) + " (" + to_string(i) + ")";
			if (names.find(candidate) == names.end()) { return candidate; }
		}

		CreateLimitError();
		return baseName;
	}

	// ��Ӗ��̐���(unordered_multimap)
	template<typename T>
	static string CreateUniqueName(string baseName, unordered_multimap<string, T> names)
	{
		unsigned int i;
		string candidate;

		// �g�p���Ă��Ȃ����O��������܂ŒT��
		for (i = 1; i < m_MAX_NAME_COUNT; i++)
		{
			candidate = string(baseName) + " (" + to_string(i) + ")";
			if (names.find(candidate) == names.end()) { return candidate; }
		}

		CreateLimitError();
		return baseName;
	}

private:
	// �����̌��E���ɒB�����ꍇ�̃G���[����
	static void CreateLimitError();
};
