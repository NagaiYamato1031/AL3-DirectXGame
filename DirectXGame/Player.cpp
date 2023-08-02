#include "Player.h"

#include "ImGuiManager.h"
#include <cassert>

#include "Matrix4x4.h"
#include "Mymath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "GlobalConfigs.h"
#include <numbers>

void Player::Initialize(const std::vector<Model*>& models) {
	// NULL ポインタチェック
	for (Model* model : models) {
		assert(model);
	}

	models_ = models;
	worldTransforms_.clear();

	for (size_t i = 0; i < kPlayerCount; i++) {
		WorldTransform wt = WorldTransform();
		worldTransforms_.push_back(wt);
	}

	// プレイヤー自身のワールド変換データ
	worldTransformBase_.Initialize();
	worldTransformBase_.translation_ = {0.0f, 0.0f, 20.0f};

	// Body のワールド変換データ
	worldTransforms_[kPlayerBody].Initialize();
	worldTransforms_[kPlayerBody].parent_ = &worldTransformBase_;
	// worldTransformBody_.parent_ = &worldTransformBase_;
	//  Head のワールド変換データ
	// worldTransformHead_.Initialize();
	worldTransforms_[kPlayerHead].Initialize();
	worldTransforms_[kPlayerHead].parent_ = &worldTransforms_[kPlayerBody];
	worldTransforms_[kPlayerHead].translation_.y = 3.0f;
	// worldTransformHead_.parent_ = &worldTransformBody_;
	// worldTransformHead_.translation_.y = 3.0f;
	//  L arm のワールド変換データ
	worldTransforms_[kPlayerL_arm].Initialize();
	worldTransforms_[kPlayerL_arm].parent_ = &worldTransforms_[kPlayerBody];
	worldTransforms_[kPlayerL_arm].translation_.x = -1.0f;
	worldTransforms_[kPlayerL_arm].translation_.y = 2.7f;
	// worldTransformL_arm_.Initialize();
	// worldTransformL_arm_.parent_ = &worldTransformBody_;
	// worldTransformL_arm_.translation_.x = -1.0f;
	// worldTransformL_arm_.translation_.y = 2.7f;
	//  R arm のワールド変換データ
	worldTransforms_[kPlayerR_arm].Initialize();
	worldTransforms_[kPlayerR_arm].parent_ = &worldTransforms_[kPlayerBody];
	worldTransforms_[kPlayerR_arm].translation_.x = 1.0f;
	worldTransforms_[kPlayerR_arm].translation_.y = 2.7f;
	// worldTransformR_arm_.Initialize();
	// worldTransformR_arm_.parent_ = &worldTransformBody_;
	// worldTransformR_arm_.translation_.x = 1.0f;
	// worldTransformR_arm_.translation_.y = 2.7f;

	worldTransforms_[kPlayerWeapon].Initialize();
	worldTransforms_[kPlayerWeapon].parent_ = &worldTransforms_[kPlayerBody];
	worldTransforms_[kPlayerWeapon].translation_.y = 1.0f;

	InitializeFloatingGimmick();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	GlobalConfigs* globalConfigs = GlobalConfigs::GetInstance();
	const char* groupName = "Player";
	globalConfigs->CreateGroup(groupName);

	// globalConfigs->AddItem(groupName, "TestInt", 90);

	globalConfigs->AddItem(
	    groupName, "Head Translation", worldTransforms_[kPlayerHead].translation_);
	globalConfigs->AddItem(
	    groupName, "ArmL Translation", worldTransforms_[kPlayerL_arm].translation_);
	globalConfigs->AddItem(
	    groupName, "ArmR Translation", worldTransforms_[kPlayerR_arm].translation_);
	globalConfigs->AddItem(groupName, "floatingCycle", floatingCycle_);
	globalConfigs->AddItem(groupName, "floatingAmplitude", floatingAmplitude_);
}

void Player::Update() {

#ifdef _DEBUG

	ImGui::Begin("Player");

	if (ImGui::Button("LoadConfig")) {
		AddlyGlobalConfigs();
	}

	ImGui::End();

#endif // _DEBUG



	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Player::Behavior::kRoot:
			BehaviorRootInitialize();
			break;
		case Player::Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		default:
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
	switch (behavior_) {
	case Player::Behavior::kRoot:
		BehaviorRootUpdate();
		break;
	case Player::Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	default:
		break;
	}

	worldTransformBase_.UpdateMatrix();
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void Player::Draw() {
	for (size_t i = 0; i < models_.size(); i++) {
		if (i == kPlayerWeapon && behavior_ != Behavior::kAttack) {
			continue;
		}
		models_[i]->Draw(worldTransforms_[i], *viewProjection_);
	}

	//   3D モデルを描画
	// modelBody_->Draw(worldTransformBody_, viewProjection);
	// modelHead_->Draw(worldTransformHead_, viewProjection);
	// modelL_arm_->Draw(worldTransformL_arm_, viewProjection);
	// modelR_arm_->Draw(worldTransformR_arm_, viewProjection);
}

void Player::AddlyGlobalConfigs() {
	GlobalConfigs* globalConfigs = GlobalConfigs::GetInstance();
	const char* groupName = "Player";
	worldTransforms_[kPlayerHead].translation_ = globalConfigs->GetVector3Value(groupName, "Head Translation");
	worldTransforms_[kPlayerL_arm].translation_ = globalConfigs->GetVector3Value(groupName, "ArmL Translation");
	worldTransforms_[kPlayerR_arm].translation_ = globalConfigs->GetVector3Value(groupName, "ArmR Translation");
	floatingCycle_ = globalConfigs->GetIntValue(groupName, "floatingCycle");
	floatingAmplitude_ = globalConfigs->GetFloatValue(groupName, "floatingAmplitude");
}

void Player::BehaviorRootInitialize() {
	worldTransforms_[kPlayerHead].translation_.y = 3.0f;
	worldTransforms_[kPlayerL_arm].translation_.x = -1.0f;
	worldTransforms_[kPlayerL_arm].translation_.y = 2.7f;
	worldTransforms_[kPlayerL_arm].rotation_ = {0, 0, 0};
	worldTransforms_[kPlayerR_arm].translation_.x = 1.0f;
	worldTransforms_[kPlayerR_arm].translation_.y = 2.7f;
	worldTransforms_[kPlayerR_arm].rotation_ = {0, 0, 0};

	InitializeFloatingGimmick();
}

void Player::BehaviorRootUpdate() {

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

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			behaviorRequest_ = Behavior::kAttack;
		}
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
	if (input_->TriggerKey(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kAttack;
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
}

#pragma region Atack

void Player::BehaviorAttackInitialize() {
	attackParameter_ = 0.0f;
	worldTransforms_[kPlayerBody].translation_ = {0, 0, 0};
	worldTransforms_[kPlayerWeapon].translation_.y = 1.0f;
	aBehaviorRequest_ = AttackBehavior::kPreAttack;
}

void Player::BehaviorAttackUpdate() {
	if (aBehaviorRequest_) {
		aBehavior_ = aBehaviorRequest_.value();
		attackParameter_ = 0;
		aBehaviorRequest_ = std::nullopt;
	}
	switch (aBehavior_) {
	case Player::AttackBehavior::kPreAttack:
		PreAttack();
		break;
	case Player::AttackBehavior::kExecuteAttack:
		ExecuteAttack();
		break;
	case Player::AttackBehavior::kPostAttack:
		PostAttack();
		break;
	default:
		break;
	}
}
void Player::PreAttack() {
	// 行動の全体フレーム
	const int kAllFrame = 5;

	// 攻撃の開始と終了位置
	const float kStartRadian = static_cast<float>(std::numbers::pi);
	const float kEndRadian = static_cast<float>(std::numbers::pi) / 3.0f * 2.4f;

	// 1 フレームでのパラメータ加算値
	float step = (kEndRadian - kStartRadian) / static_cast<float>(kAllFrame);

	attackParameter_ += step;

	worldTransforms_[kPlayerL_arm].rotation_.x = kStartRadian + attackParameter_;
	worldTransforms_[kPlayerR_arm].rotation_.x = kStartRadian + attackParameter_;
	worldTransforms_[kPlayerWeapon].rotation_.x = attackParameter_;

	if (std::fabsf(kEndRadian - kStartRadian) < std::fabsf(attackParameter_)) {
		// 実行段階へ
		aBehaviorRequest_ = AttackBehavior::kExecuteAttack;
	}
}

void Player::ExecuteAttack() {

	// 攻撃の全体フレーム<frame>
	const int kAllFrame = 20;
	// 攻撃の開始と終了位置
	const float kStartRadian = static_cast<float>(std::numbers::pi) / 3.0f * 2.4f;
	const float kEndRadian = static_cast<float>(std::numbers::pi) / 4.0f * 6.0f;
	// 1 フレームでのパラメータ加算値
	float step = (kEndRadian - kStartRadian) / static_cast<float>(kAllFrame);

	// 振り上げた位置
	// float wavingAngle = static_cast<float>(std::numbers::pi) - 0.4f;

	// worldTransforms_[kPlayerL_arm].rotation_.z = -wavingAngle;
	// worldTransforms_[kPlayerR_arm].rotation_.z = wavingAngle;
	//  パラメータを 1 ステップ加算
	attackParameter_ += step;
	// attackParameter_ = std::fmodf(attackParameter_, (kEndRadian - kStartRadian));

	worldTransforms_[kPlayerL_arm].rotation_.x = kStartRadian + attackParameter_;
	worldTransforms_[kPlayerR_arm].rotation_.x = kStartRadian + attackParameter_;
	worldTransforms_[kPlayerWeapon].rotation_.x =
	    kStartRadian - static_cast<float>(std::numbers::pi) + attackParameter_;

	if ((kEndRadian - kStartRadian) < attackParameter_) {
		// 後段階へ
		aBehaviorRequest_ = AttackBehavior::kPostAttack;
		// behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::PostAttack() {
	// 攻撃の全体フレーム<frame>
	const int kAllFrame = 5;

	attackParameter_++;

	if (kAllFrame <= attackParameter_) {
		// 通常行動へ
		behaviorRequest_ = Behavior::kRoot;
	}
}

#pragma endregion

#pragma region 浮遊ギミック

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	// 浮遊移動のサイクル<frame>
	// static int floatingCycle = 60;
	// 1 フレームでのパラメータ加算値
	float step = 2.0f * static_cast<float>(std::numbers::pi) / floatingCycle_;

	// パラメータを 1 ステップ分加算
	floatingParameter_ += step;
	// 2 π を超えたら 0 に戻す
	floatingParameter_ =
	    std::fmodf(floatingParameter_, 2.0f * static_cast<float>(std::numbers::pi));

	// 浮遊の振幅<m>
	// static float floatingAmplitude_ = 0.5f;
	// 浮遊を座標に変換
	worldTransforms_[kPlayerBody].translation_.y =
	    std::sin(floatingParameter_) * floatingAmplitude_;
	// worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude;
	float amplitudeArm = 0.2f;
	worldTransforms_[kPlayerL_arm].rotation_.z = std::sin(floatingParameter_) * amplitudeArm;
	worldTransforms_[kPlayerR_arm].rotation_.z = -std::sin(floatingParameter_) * amplitudeArm;
	// worldTransformL_arm_.rotation_.z = std::sin(floatingParameter_) * amplitudeArm;
	// worldTransformR_arm_.rotation_.z = -std::sin(floatingParameter_) * amplitudeArm;

	// ImGui::Begin("Player");

	// ImGui::SliderFloat3(
	//     "Head Translation", &worldTransforms_[kPlayerHead].translation_.x, -10.0f, 10.0f);
	// ImGui::SliderFloat3(
	//     "ArmL Translation", &worldTransforms_[kPlayerL_arm].translation_.x, -10.0f, 10.0f);
	// ImGui::SliderFloat3(
	//     "ArmR Translation", &worldTransforms_[kPlayerR_arm].translation_.x, -10.0f, 10.0f);
	// ImGui::SliderInt("cycle", &cycle, 2, 120);
	// ImGui::SliderFloat("amplitude", &amplitude, 0.0f, 5.0f);

	// ImGui::End();
}

#pragma endregion
