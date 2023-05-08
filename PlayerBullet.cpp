#include "PlayerBullet.h"

#include <cassert>
#include "TextureManager.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULL ポインタチェック
	assert(model);
		
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 速度を設定
	velocity_ = velocity;

}

void PlayerBullet::Update() {
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 座標を移動させる
	worldTransform_.translation_ += velocity_;

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}


void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}