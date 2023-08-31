#include "BulletManager.h"

#include <cassert>

#include "ImGuiManager.h"

#include "MyConst.h"
#include "Mymath.h"

#include "BaseBullet.h"
#include "PlayerBullet.h"

BulletManager* BulletManager::GetInstance() {
	static BulletManager instance;
	return &instance;
}

void BulletManager::Initialize(const std::vector<Model*>& models) {
	for (Model* model : models) {
		assert(model);
		model;
	}
	models_ = models;

	// 初期化とタイプ登録
	datas_.clear();
	datas_[BulletType::typePlayer];
	datas_[BulletType::typeEnemy];
}

void BulletManager::Update() {
	for (auto& data : datas_) {
		Bullets& bullets = data.second;

		// 動いていない弾を削除
		bullets.remove_if([](auto& bullet) {
			if (!bullet->GetIsActive()) {
				return true;
			}
			// ついでに更新処理
			bullet->Update();
			return false;
		});

#ifdef _DEBUG

		ImGui::Begin("Bullet");

		ImGui::Text("Bullet Number");

		ImGui::Separator();

		ImGui::Text("%d", bullets.size());

		uint32_t index = 0;
		for (index = 0; index < bullets.size(); index++) {
			ImGui::Bullet();
			ImGui::Text("%d", index);
		}

		ImGui::Separator();

		ImGui::End();

#endif // _DEBUG

		// for (BaseBullet* bullet : bullets) {
		//	bullet->Update();
		// }
	}
}

void BulletManager::Draw() {
	for (const auto& data : datas_) {
		const Bullets& bullets = data.second;

		for (auto& bullet : bullets) {
			// タイプによってモデル変更(または色変更)
			models_[data.first]->Draw(bullet->GetWorldTransform(), *viewProjection_);

			// switch (data.first) {
			// case BulletType::typePlayer:
			//	models_[BulletType::typePlayer]->Draw(bullet->GetWorldTransform(),
			//*viewProjection_); 	break; case BulletType::typeEnemy:
			//	models_[bull]->Draw(bullet->GetWorldTransform(), *viewProjection_);
			//	break;
			// default:
			//	break;
			// }
		}
	}
}

void BulletManager::Clear() {
	for (auto& data : datas_) {
		Bullets& bullets = data.second;
		// 全部の弾を削除
		bullets.clear();
	}
}

void BulletManager::AddBullet(BulletType type, BaseBullet* newBullet) {

	if (datas_.contains(type)) {
		datas_[type].emplace_back(newBullet);
	}
}

bool BulletManager::IsCollisionSphere(BulletType type, const BaseCharacter& chara) {
	// 指定したタイプとの衝突判定を行う
	for (auto& bullet : datas_[type]) {
		if (Mymath::IsCollision(bullet->GetSphere(), chara.GetSphere())) {
			bullet->OnCollision();
			return true;
		}
	}
	return false;
}
