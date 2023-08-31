#include "Enemy.h"
#include <cassert>
#include <imgui.h>
#include <numbers>

#include "Mymath.h"

#include "BulletManager.h"
#include "EnemyBullet.h"
#include "GameScene.h"
#include "GlobalConfigs.h"

#include "Player.h"

void Enemy::Initialize(const std::vector<Model*>& models) {

	for (Model* model : models) {
		assert(model);
		model;
	}
	models_ = models;

	// モデルのワールド情報
	for (size_t i = 0; i < kEnemyCount; i++) {
		WorldTransform wt = WorldTransform();
		worldTransforms_.push_back(wt);
	}

	worldTransformBase_.Initialize();
	worldTransformBase_.translation_ = {0.0f, 0.0f, 0.0f};

	worldTransforms_[kEnemyBody].Initialize();
	worldTransforms_[kEnemyBody].parent_ = &worldTransformBase_;
	worldTransforms_[kEnemyBody].translation_ = {0.0f, 0.0f, 0.0f};

	worldTransforms_[kEnemyBody].scale_ = {10.0f, 10.0f, 10.0f};

	bulletManager_ = BulletManager::GetInstance();

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Enemy";
	configs->CreateGroup(groupName);

	configs->AddItem(groupName, "health", health_);
	configs->AddItem(groupName, "coolTime", attackCoolTime);

	// float halfpi = static_cast<float>(std::numbers::pi) / 4.0f;

	// worldTransforms_[kEnemyWeapon].Initialize();
	// worldTransforms_[kEnemyWeapon].parent_ = &worldTransforms_[kEnemyBody];
	// worldTransforms_[kEnemyWeapon].translation_ = {1.9f, 0.0f, 0.0f};
	// worldTransforms_[kEnemyWeapon].rotation_ = {halfpi, halfpi, halfpi};

	worldTransformBase_.UpdateMatrix();

	for (WorldTransform& wt : worldTransforms_) {
		wt.UpdateMatrix();
	}
}

void Enemy::Update() {

#ifdef _DEBUG

	ImGui::Begin("Enemy");

	ImGui::SliderFloat3("translate", &worldTransformBase_.translation_.x, -3.0f, 3.0f);
	ImGui::SliderFloat3(
	    "body translate", &worldTransforms_[kEnemyBody].translation_.x, -3.0f, 3.0f);
	/*ImGui::SliderFloat3(
	    "weapon translate", &worldTransforms_[kEnemyWeapon].translation_.x, -3.0f, 3.0f);
	ImGui::SliderFloat3("weapon scale", &worldTransforms_[kEnemyWeapon].scale_.x, -3.0f, 3.0f);
	ImGui::SliderFloat3(
	    "weapon rotate", &worldTransforms_[kEnemyWeapon].rotation_.x, -3.0f, 3.0f);*/

	ImGui::End();

#endif // _DEBUG

	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Enemy::Behavior::kRoot:
			BehaviorRootInitialize();
			break;
		case Enemy::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Enemy::Behavior::kDamage:
			BehaviorDamageInitialize();
			break;
		default:
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
	switch (behavior_) {
	case Enemy::Behavior::kRoot:
		BehaviorRootUpdate();
		break;
	case Enemy::Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Enemy::Behavior::kDamage:
		BehaviorDamageUpdate();
		break;
	default:
		break;
	}

	worldTransformBase_.UpdateMatrix();
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void Enemy::CappedUpdate() {

	// 円運動
	worldTransformBase_.rotation_.y += 0.004f;
	// オーバーフロー対策
	worldTransformBase_.rotation_.y =
	    std::fmodf(worldTransformBase_.rotation_.y, 2.0f * static_cast<float>(std::numbers::pi));

	worldTransformBase_.UpdateMatrix();

	for (WorldTransform& wt : worldTransforms_) {
		wt.UpdateMatrix();
	}
}

void Enemy::Draw() {
	for (size_t i = 0; i < models_.size(); i++) {
		models_[i]->Draw(worldTransforms_[i], *viewProjection_);
	}
}

void Enemy::OnCollision() {
	health_--;

	if (health_ <= 0) {
		gameScene_->ClearInitialize();
	}
}

void Enemy::AddlyGlobalConfigs() {
	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Enemy";

	health_ = configs->GetIntValue(groupName, "health");
	kMaxHealth_ = configs->GetIntValue(groupName, "health");
	attackCoolTime = configs->GetIntValue(groupName, "coolTime");
}

void Enemy::SetPlayer(Player* player) { player_ = player; }

// 通常行動初期化
void Enemy::BehaviorRootInitialize() { stepParameter_ = 0; }

// 通常行動更新
void Enemy::BehaviorRootUpdate() {
	// 円運動
	worldTransformBase_.rotation_.y += 0.01f;
	// オーバーフロー対策
	worldTransformBase_.rotation_.y =
	    std::fmodf(worldTransformBase_.rotation_.y, 2.0f * static_cast<float>(std::numbers::pi));

	attackCoolTime--;
	if (attackCoolTime <= 0) {
		behaviorRequest_ = Behavior::kAttack;
		attackCoolTime = GlobalConfigs::GetInstance()->GetIntValue("Enemy", "coolTime");
	}
}

// 攻撃行動初期化
void Enemy::BehaviorAttackInitialize() {
	aBehaviorRequest_ = StateBehavior::kPreState;
	stepParameter_ = 0;
}

// 攻撃行動更新
void Enemy::BehaviorAttackUpdate() {

	if (aBehaviorRequest_) {
		stateBehavior_ = aBehaviorRequest_.value();
		stepParameter_ = 0;
		aBehaviorRequest_ = std::nullopt;
	}
	switch (stateBehavior_) {
	case Enemy::StateBehavior::kPreState:
		PreAttack();
		break;
	case Enemy::StateBehavior::kExecuteState:
		ExecuteAttack();
		break;
	case Enemy::StateBehavior::kPostState:
		PostAttack();
		break;
	default:
		break;
	}
}

// 攻撃中の動作
void Enemy::PreAttack() {
	// 行動の全体フレーム
	const int kAllFrame = 15;

	// 攻撃前段階の開始と終了位置
	const float kStartRadian = static_cast<float>(std::numbers::pi);
	const float kEndRadian = static_cast<float>(std::numbers::pi) / 3.0f * 2.4f;

	// 1 フレームでのパラメータ加算値
	float step = (kEndRadian - kStartRadian) / static_cast<float>(kAllFrame);

	stepParameter_ += step;
	// worldTransforms_[kEnemyBody].rotation_.y -= 0.0001f;

	// worldTransforms_[kPlayerL_arm].rotation_.x = kStartRadian + attackParameter_;
	// worldTransforms_[kPlayerR_arm].rotation_.x = kStartRadian + attackParameter_;
	// worldTransforms_[kPlayerWeapon].rotation_.x = attackParameter_;

	// worldTransforms_[kEnemyBody].rotation_.y += step;

	if (std::fabsf(kEndRadian - kStartRadian) < std::fabsf(stepParameter_)) {

		// 実行段階へ
		aBehaviorRequest_ = StateBehavior::kExecuteState;
	}
}
void Enemy::ExecuteAttack() {
	// 攻撃の全体フレーム<frame>
	const int kAllFrame = 20;

	stepParameter_++;

	worldTransforms_[kEnemyBody].scale_.x -= 0.04f;
	worldTransforms_[kEnemyBody].scale_.y -= 0.04f;
	worldTransforms_[kEnemyBody].scale_.z -= 0.04f;

	if (kAllFrame <= stepParameter_) {

		// 円状に弾を撃ちたい
		AttackCircle();

		// 体力が減っていたら追加で打つ
		if (health_ <= kMaxHealth_ * 0.7) {
			AttackCircle();
		}


		// float kBulletSpeed = 1.0f;

		// Vector3 theta = player_->GetTheta();
		// theta.x *= -1;
		// theta.y += static_cast<float>(std::numbers::pi);
		// theta.z *= -1;

		// Matrix4x4 matRotate = Mymath::MakeRotateXYZMatrix(theta);

		// Vector3 position = GetWorldPosition();

		// EnemyBullet* newBullet = new EnemyBullet();
		// newBullet->Initialize();
		// newBullet->SetWorldPosition(position);

		// Vector3 velocity{0.0f, 0.0f, -kBulletSpeed};

		// velocity = Mymath::TransformNormal(velocity, matRotate);
		// newBullet->SetVelocity(velocity);

		// bulletManager_->AddBullet(typeEnemy, newBullet);

		// 後段階へ
		aBehaviorRequest_ = StateBehavior::kPostState;
	}
}
void Enemy::PostAttack() {
	// 攻撃の全体フレーム<frame>
	const int kAllFrame = 40;
	stepParameter_++;

	worldTransforms_[kEnemyBody].scale_.x += 0.02f;
	worldTransforms_[kEnemyBody].scale_.y += 0.02f;
	worldTransforms_[kEnemyBody].scale_.z += 0.02f;

	if (kAllFrame <= stepParameter_) {

		if (health_ < kMaxHealth_ * 0.4) {
			AttackCircle();
		}
		if (health_ < kMaxHealth_ * 0.2) {
			AttackCircle();
		}
		// 通常行動へ
		behaviorRequest_ = Behavior::kRoot;
	}
}

// 被弾行動
void Enemy::BehaviorDamageInitialize() {}

// 被弾行動更新
void Enemy::BehaviorDamageUpdate() {}

// 被弾中の動作
void Enemy::ExecuteDamage() {}
void Enemy::PostDamage() {}

void Enemy::AttackCircle() {

	int kBulletMax = 64;

	float kBulletSpeed = 1.0f;

	Vector3 theta{0.0f, 0.0f, 0.0f};

	theta = player_->GetTheta();
	theta.x *= -1;
	theta.y += static_cast<float>(std::numbers::pi);

	int rnd = rand() % 181 - 90;

	float rndTheta = rnd * (static_cast<float>(std::numbers::pi) / 180);

	float pi32 = static_cast<float>(std::numbers::pi) / kBulletMax * 2;

	Matrix4x4 matZ = Mymath::MakeRotateZMatrix(rndTheta);

	Matrix4x4 matRotateXYZ = Mymath::MakeRotateXYZMatrix(theta);

	for (size_t i = 0; i < kBulletMax; i++) {

		float buf = pi32 * i;

		Vector3 zPlus{0.0f, 0.0f, kBulletSpeed};

		Matrix4x4 matX = Mymath::MakeRotateXMatrix(buf);

		matX = Mymath::Multiply(matX, matZ);

		Vector3 position = GetWorldPosition();

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize();
		newBullet->SetWorldPosition(position);

		Vector3 velocity = Mymath::TransformNormal(zPlus, Mymath::Multiply(matX, matRotateXYZ));

		// velocity = Mymath::TransformNormal(velocity, matRXYZ);
		newBullet->SetVelocity(velocity);

		bulletManager_->AddBullet(typeEnemy, newBullet);
	}
}

void Enemy::AttackBlock() {}