#include "RailCamera.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(const Vector3& translate, const Vector3& rotate) {
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = translate;
	worldTransform_.rotation_ = rotate;

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() {

	worldTransform_.translation_.z += 0.05f;

	worldTransform_.matWorld_ = Mymath::MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Mymath::Inverse(worldTransform_.matWorld_);

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");

	ImGui::DragFloat3("CameraTranslation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("CameraRotate", &worldTransform_.rotation_.x, 0.01f);

	ImGui::End();
}
