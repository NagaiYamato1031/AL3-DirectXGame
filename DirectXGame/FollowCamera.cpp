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
		viewProjection_.rotation_.y -= kRotate;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		viewProjection_.rotation_.y += kRotate;
	}
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		viewProjection_.rotation_.x -= kRotate;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		viewProjection_.rotation_.x += kRotate;
	}

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		viewProjection_.rotation_.x = 0;
		viewProjection_.rotation_.y = 0;
	}


	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset{0.0f, 6.0f, -30.0f};

		viewProjection_.rotation_ = target_->rotation_;
		viewProjection_.rotation_.y += 3.14f;
		viewProjection_.rotation_.x *= -1;
		viewProjection_.rotation_.x += 0.01f;

		//Matrix4x4 matRotate = Mymath::MakeRotateXYZMatrix(viewProjection_.rotation_);
		Matrix4x4 matRotate = Mymath::MakeRotateXYZMatrix(viewProjection_.rotation_);

		offset = Mymath::TransformNormal(offset, matRotate);

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}
	// ビュー行列の更新
	viewProjection_.UpdateMatrix();
}
