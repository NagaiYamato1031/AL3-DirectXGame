#pragma once
#include <map>
#include <string>
#include <variant>

#include "Vector3.h"

// 項目
struct Item {
	// 項目の値
	std::variant<int32_t, float, Vector3> value;
};

// グループ
struct Group {
	std::map<std::string, Item> items;
};

class GlobalConfigs {
public:
	static GlobalConfigs* GetInstance();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	void SetValue(const std::string& groupName, const std::string& key, float value);
	void SetValue(const std::string& groupName, const std::string& key, Vector3 value);

	void Update();

private:
	GlobalConfigs() = default;
	~GlobalConfigs() = default;
	GlobalConfigs(const GlobalConfigs& obj) = delete;
	const GlobalConfigs& operator=(const GlobalConfigs& obj) = delete;

	// 全データ
	std::map<std::string, Group> datas_;

};
