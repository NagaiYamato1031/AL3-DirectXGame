#include "Player.h"

#include "ImGuiManager.h"
#include <cassert>

#include "Matrix4x4.h"
#include "Mymath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "GameScene.h"

void Player::Initialze(Model* model, uint32_t textureHandle, const Vector3& position) {
	// NULL ポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update(const ViewProjection& viewProjection) { viewProjection; }

void Player::Draw(const ViewProjection& viewProjection) {

	//   3D モデルを描画
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	// worldPos = Mymath::TransformNormal(worldTransform_.translation_, worldTransform_.matWorld_);

	// ワールド座標の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
