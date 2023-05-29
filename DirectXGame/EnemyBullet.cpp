#include "EnemyBullet.h"

#include "TextureManager.h"
#include <cassert>

#include "Mymath.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULL ポインタチェック
	assert(model);

	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black1x1.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 速度を設定
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 座標を移動させる
	worldTransform_.translation_ += velocity_;

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;

	worldPos = Mymath::TransformNormal(worldTransform_.translation_, worldTransform_.matWorld_);
	//// ワールド座標の平行移動成分を取得
	// worldPos.x = worldTransform_.translation_.x * worldTransform_.matWorld_.m[3][0];
	// worldPos.y = worldTransform_.translation_.y * worldTransform_.matWorld_.m[3][1];
	// worldPos.z = worldTransform_.translation_.z * worldTransform_.matWorld_.m[3][2];
	return worldPos;
}


void EnemyBullet::OnCollision() { isDead_ = true; }
