#include "Enemy.h"

#include <cassert>

#include "Matrix4x4.h"
#include "Mymath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_.y = 5;
	worldTransform_.translation_.z = 50;
}

void Enemy::Update() {

	Vector3 move{0.0f, 0.0f, 0.0f};

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}

	// 移動限界座標
	const float kMoveLimitX = 50.0f;
	const float kMoveLimitY = 30.0f;
	const float kMoveLimitZ = 50.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	worldTransform_.translation_.z = max(worldTransform_.translation_.z, -kMoveLimitZ);
	worldTransform_.translation_.z = min(worldTransform_.translation_.z, +kMoveLimitZ);

	// 行列の更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// 3D モデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Approach() {
	Vector3 move{0.0f, 0.0f, 0.0f};
	move.z = -0.3f;
	// 移動速度
	worldTransform_.translation_ += move;
	// 既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
void Enemy::Leave() {
	Vector3 move{0.0f, 0.0f, 0.0f};
	move.x = 0.3f;
	move.y = 0.1f;
	// 移動
	worldTransform_.translation_ += move;
}