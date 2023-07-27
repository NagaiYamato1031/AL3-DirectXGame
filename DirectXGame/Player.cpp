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

void Player::Update() {
	XINPUT_STATE joyState;

	// 速さ
	const float kSpeed = 0.3f;
	// 移動成分
	Vector3 move = {0.0f, 0.0f, 0.0f};
	// ゲームパッドが有効なら
	if (input_->GetJoystickState(0, joyState)) {
		// 移動量
		move.x = static_cast<float>(joyState.Gamepad.sThumbLX);
		move.z = static_cast<float>(joyState.Gamepad.sThumbLY);
		// 移動量に速さを反映
		move = Mymath::Normalize(move) * kSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		move.z = kSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		move.z = -kSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		move.x = kSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		move.x = -kSpeed;
	}
	if (move.x != 0.0f || move.y != 0.0f || move.z != 0.0f) {
		// 回転方向に合わせる
		Matrix4x4 matRotate = Mymath::MakeRotateYMatrix(viewProjection_->rotation_.y);

		move =
		    Mymath::TransformNormal(move, matRotate);
		// 移動
		worldTransform_.translation_ += move;

		// 進行方向に向けて回転する
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}
	worldTransform_.UpdateMatrix();
}

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
