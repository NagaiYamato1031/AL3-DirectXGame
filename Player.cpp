#include "Player.h"

#include <cassert>

void Player::Initialze(Model* model, uint32_t textureHandle) {
	// NULL ポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
}

void Player::Update() {
	// 行列を定数バッファに転送	
	worldTransform_.TransferMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	// 3D モデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
