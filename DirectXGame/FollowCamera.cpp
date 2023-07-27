#include "FollowCamera.h"
#include "Input.h"
#include "Mymath.h"

void FollowCamera::Initialize() { viewProjection_.Initialize(); }

void FollowCamera::Update() {
	XINPUT_STATE joyState;

	// 回転速度
	const float kRotate = 0.03f;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		viewProjection_.rotation_.y += static_cast<float>(joyState.Gamepad.sThumbRX) / SHRT_MAX * kRotate;
		viewProjection_.rotation_.x -= static_cast<float>(joyState.Gamepad.sThumbRY) / SHRT_MAX * kRotate;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		viewProjection_.rotation_.y += kRotate;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		viewProjection_.rotation_.y -= kRotate;
	}


	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset{0.0f, 4.0f, -20.0f};

		Matrix4x4 matRotate = Mymath::MakeRotateXYZMatrix(viewProjection_.rotation_);

		offset = Mymath::TransformNormal(offset, matRotate);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}
	// ビュー行列の更新
	viewProjection_.UpdateMatrix();
}
