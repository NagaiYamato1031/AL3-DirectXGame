#include "Enemy.h"
#include <cassert>
#include <imgui.h>
#include <numbers>

#include "Mymath.h"

void Enemy::Initialize(const std::vector<Model*>& models) {

	for (Model* model : models) {
		assert(model);
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
	worldTransforms_[kEnemyBody].translation_ = {0.0f, 2.0f, 0.0f};

	float halfpi = static_cast<float>(std::numbers::pi) / 4.0f;

	worldTransforms_[kEnemyWeapon].Initialize();
	worldTransforms_[kEnemyWeapon].parent_ = &worldTransforms_[kEnemyBody];
	worldTransforms_[kEnemyWeapon].translation_ = {1.9f, 0.0f, 0.0f};
	worldTransforms_[kEnemyWeapon].rotation_ = {halfpi, halfpi, halfpi};
}

void Enemy::Update() {

	if (0) {
		ImGui::Begin("Enemy");

		ImGui::SliderFloat3("translate", &worldTransformBase_.translation_.x, -3.0f, 3.0f);
		ImGui::SliderFloat3(
		    "body translate", &worldTransforms_[kEnemyBody].translation_.x, -3.0f, 3.0f);
		ImGui::SliderFloat3(
		    "weapon translate", &worldTransforms_[kEnemyWeapon].translation_.x, -3.0f, 3.0f);
		ImGui::SliderFloat3("weapon scale", &worldTransforms_[kEnemyWeapon].scale_.x, -3.0f, 3.0f);
		ImGui::SliderFloat3(
		    "weapon rotate", &worldTransforms_[kEnemyWeapon].rotation_.x, -3.0f, 3.0f);

		ImGui::End();
	}

	// 円運動
	worldTransformBase_.rotation_.y += 0.01f;
	// オーバーフロー対策
	worldTransformBase_.rotation_.y =
	    std::fmodf(worldTransformBase_.rotation_.y, 2.0f * static_cast<float>(std::numbers::pi));

	Matrix4x4 matRotate = Mymath::MakeRotateYMatrix(worldTransformBase_.rotation_.y);

	const float kMoveMax = 0.2f;

	Vector3 velocity{0.0f, 0.0f, kMoveMax};

	velocity = Mymath::TransformNormal(velocity, matRotate);

	worldTransformBase_.translation_ += velocity;

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