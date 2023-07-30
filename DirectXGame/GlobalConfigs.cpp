#include "GlobalConfigs.h"

#include <imgui.h>

GlobalConfigs* GlobalConfigs::GetInstance() {
	static GlobalConfigs instance;
	return &instance;
}

void GlobalConfigs::CreateGroup(const std::string& groupName) {
	// 指定枚のオブジェクトが無ければ追加する
	datas_[groupName];
}
void GlobalConfigs::SetValue(const std::string& groupName, const std::string& key, int32_t value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい高極のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目を std::map に追加
	group.items[key] = newItem;
}

void GlobalConfigs::SetValue(const std::string& groupName, const std::string& key, float value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい高極のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目を std::map に追加
	group.items[key] = newItem;
}
void GlobalConfigs::SetValue(const std::string& groupName, const std::string& key, Vector3 value) {
	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい高極のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目を std::map に追加
	group.items[key] = newItem;
}

void GlobalConfigs::Update() {
	if (!ImGui::Begin("Global Configs", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar()) {
		return;
	}

	// 各グループについて
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end();
	     ++itGroup) {
		// グループ名を取得
		const std::string& groupName = itGroup->first;
		// グループの参照を取得
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str())) {
			continue;
		}

		// 各項目について
		for (std::map<std::string, Item>::iterator itItem = group.items.begin();
		     itItem != group.items.end(); ++itItem) {
			// 項目名を取得
			const std::string& itemName = itItem->first;
			// 項目の参照を取得
			Item& item = itItem->second;

			// int32_t 型の値を保持していれば
			if (std::holds_alternative<int32_t>(item.value)) {
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			}
			// float 型の値を保持していれば
			if (std::holds_alternative<float>(item.value)) {
				float* ptr = std::get_if<float>(&item.value);
				ImGui::SliderFloat(itemName.c_str(), ptr, -10.0f, 10.0f);
			}
			// Vector3 型の値を保持していれば
			if (std::holds_alternative<Vector3>(item.value)) {
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::SliderFloat3(itemName.c_str(), &ptr->x, -10.0f, 10.0f);
			}


		}

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();
}