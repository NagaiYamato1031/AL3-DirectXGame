#include "Player.h"

#include "ImGuiManager.h"
#include <cassert>

#include "Matrix4x4.h"
#include "Mymath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "GameScene.h"
#include <numbers>

void Player::Initialze(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm) {
	// NULL ポインタチェック
	assert(modelBody);
	assert(modelHead);
	assert(modelL_arm);
	assert(modelR_arm);

	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;
	// プレイヤー自身のワールド変換データ
	worldTransformBase_.Initialize();
	worldTransformBase_.translation_ = {0.0f, 0.0f, 20.0f};
	// Body のワールド変換データ
	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransformBase_;
	// Head のワールド変換データ
	worldTransformHead_.Initialize();
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformHead_.translation_.y = 3.0f;
	// L arm のワールド変換データ
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.translation_.x = -1.0f;
	worldTransformL_arm_.translation_.y = 2.7f;
	// R arm のワールド変換データ
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.translation_.x = 1.0f;
	worldTransformR_arm_.translation_.y = 2.7f;

	InitializeFloatingGimmick();

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

		move = Mymath::TransformNormal(move, matRotate);
		// 移動
		worldTransformBase_.translation_ += move;

		// 進行方向に向けて回転する
		worldTransformBase_.rotation_.y = std::atan2(move.x, move.z);
	}
	UpdateFloatingGimmick();

	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {

	//   3D モデルを描画
	modelBody_->Draw(worldTransformBody_, viewProjection);
	modelHead_->Draw(worldTransformHead_, viewProjection);
	modelL_arm_->Draw(worldTransformL_arm_, viewProjection);
	modelR_arm_->Draw(worldTransformR_arm_, viewProjection);
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	// worldPos = Mymath::TransformNormal(worldTransform_.translation_, worldTransform_.matWorld_);

	// ワールド座標の平行移動成分を取得
	worldPos.x = worldTransformBase_.matWorld_.m[3][0];
	worldPos.y = worldTransformBase_.matWorld_.m[3][1];
	worldPos.z = worldTransformBase_.matWorld_.m[3][2];
	return worldPos;
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	// 浮遊移動のサイクル<frame>
	static int cycle = 60;
	// 1 フレームでのパラメータ加算値
	float step = 2.0f * static_cast<float>(std::numbers::pi) / cycle;

	// パラメータを 1 ステップ分加算
	floatingParameter_ += step;
	// 2 π を超えたら 0 に戻す
	floatingParameter_ =
	    std::fmodf(floatingParameter_, 2.0f * static_cast<float>(std::numbers::pi));

	// 浮遊の振幅<m>
	static float amplitude = 0.5f;
	// 浮遊を座標に変換
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude;
	float amplitudeArm = 0.2f;
	worldTransformL_arm_.rotation_.z = std::sin(floatingParameter_) * amplitudeArm;
	worldTransformR_arm_.rotation_.z = -std::sin(floatingParameter_) * amplitudeArm;

	ImGui::Begin("Player");

	ImGui::SliderFloat3("Head Translation", &worldTransformHead_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, -10.0f, 10.0f);
	ImGui::SliderInt("cycle", &cycle, 1, 120);
	ImGui::SliderFloat("amplitude", &amplitude, 0.0f, 5.0f);

	ImGui::End();
}
