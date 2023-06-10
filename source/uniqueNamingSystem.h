/*******************************************************************************
*
*	タイトル：	一意名称生成用静的クラス
*	ファイル：	uniqueNamingSystem.h
*	作成者：	古本 泰隆
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
	static inline const unsigned long long m_MAX_NAME_COUNT = 9999;	// 重複名称の最大数

private:
	UniqueNamingSystem() {}
public:
	// 一意名称生成(map)
	template<typename T>
	static string CreateUniqueName(string baseName, map<string, T> names)
	{
		unsigned int i;
		string candidate;

		// 使用していない名前が見つかるまで探索
		for (i = 1; i < m_MAX_NAME_COUNT; i++)
		{
			candidate = string(baseName) + " (" + to_string(i) + ")";
			if (names.find(candidate) == names.end()) { return candidate; }
		}

		CreateLimitError();
		return baseName;
	}

	// 一意名称生成(unordered_map)
	template<typename T>
	static string CreateUniqueName(string baseName, unordered_map<string, T> names)
	{
		unsigned int i;
		string candidate;

		// 使用していない名前が見つかるまで探索
		for (i = 1; i < m_MAX_NAME_COUNT; i++)
		{
			candidate = string(baseName) + " (" + to_string(i) + ")";
			if (names.find(candidate) == names.end()) { return candidate; }
		}

		CreateLimitError();
		return baseName;
	}

	// 一意名称生成(multimap)
	template<typename T>
	static string CreateUniqueName(string baseName, multimap<string, T> names)
	{
		unsigned int i;
		string candidate;

		// 使用していない名前が見つかるまで探索
		for (i = 1; i < m_MAX_NAME_COUNT; i++)
		{
			candidate = string(baseName) + " (" + to_string(i) + ")";
			if (names.find(candidate) == names.end()) { return candidate; }
		}

		CreateLimitError();
		return baseName;
	}

	// 一意名称生成(unordered_multimap)
	template<typename T>
	static string CreateUniqueName(string baseName, unordered_multimap<string, T> names)
	{
		unsigned int i;
		string candidate;

		// 使用していない名前が見つかるまで探索
		for (i = 1; i < m_MAX_NAME_COUNT; i++)
		{
			candidate = string(baseName) + " (" + to_string(i) + ")";
			if (names.find(candidate) == names.end()) { return candidate; }
		}

		CreateLimitError();
		return baseName;
	}

private:
	// 生成の限界数に達した場合のエラー処理
	static void CreateLimitError();
};
