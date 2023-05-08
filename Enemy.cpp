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
	// 移動ベクトルの設定
	Vector3 move = {0, 0, 0};

	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	move.z -= kCharacterSpeed;

	// 座標加算(ベクトルの加算)
	worldTransform_.translation_ += move;

	// 移動限界座標
	const float kMoveLimitX = 32.5f;
	const float kMoveLimitY = 17.5f;
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