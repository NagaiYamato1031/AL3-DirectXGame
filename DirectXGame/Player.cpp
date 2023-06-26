#include "Player.h"

#include "ImGuiManager.h"
#include <cassert>

#include "Matrix4x4.h"
#include "Mymath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "GameScene.h"
#include "PlayerBullet.h"

Player::~Player() { delete sprite2DReticle_; }

void Player::Initialze(Model* model, uint32_t textureHandle, const Vector3& position) {
	// NULL ポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform3DReticle_.Initialize();

	uint32_t textureReticle = TextureManager::Load("reticle.png");

	//worldTransform3DReticle_.parent_ = worldTransform_.parent_;
	sprite2DReticle_ = Sprite::Create(textureReticle, {0, 0}, {1, 1, 1, 1}, {0.5f, 0.5f});

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update(const ViewProjection& viewProjection) {

	// デスフラグの立った弾を削除
	/*bullets_.remove_if([](PlayerBullet* bullet) {
	    if (bullet->IsDead()) {
	        delete bullet;
	        return true;
	    }
	    return false;
	});*/

	// 旋回
	// Rotate();

	Move();

	ReticleUpdate(viewProjection);
	// 攻撃
	Attack();

	// 弾更新
	/*for (PlayerBullet* bullet : bullets_) {
	    bullet->Update();
	}*/
}

void Player::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);
	//   3D モデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	// for (PlayerBullet* bullet : bullets_) {
	//	bullet->Draw(viewProjection);
	//}
}

void Player::DrawUI() { sprite2DReticle_->Draw(); }

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	// worldPos = Mymath::TransformNormal(worldTransform_.translation_, worldTransform_.matWorld_);

	// ワールド座標の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Player::OnCollision() {
	// 何もしない
}

// ゲッターセッター

// プライベート関数

void Player::Rotate() {
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向でベクトルを要求
	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
	// 押した方向でベクトルを要求
	if (input_->PushKey(DIK_UP)) {
		worldTransform_.rotation_.x -= kRotSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		worldTransform_.rotation_.x += kRotSpeed;
	}
}

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity{0, 0, 0};

		Vector3 worldPositionReticle{
		    worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
		    worldTransform3DReticle_.matWorld_.m[3][2]};

		velocity = worldPositionReticle - GetWorldPosition();
		velocity = Mymath::Normalize(velocity) * kBulletSpeed;

		// 速度ベクトルを自機の向きに合わせて回転する
		// velocity = Mymath::TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		gameScene_->AddPlayerBullet(newBullet);
		// bullets_.push_back(newBullet);
	}
}

void Player::Move() {

	// 移動ベクトルの設定
	Vector3 move = {0, 0, 0};

	// キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	// 座標加算(ベクトルの加算)
	worldTransform_.translation_ = Mymath::Add(worldTransform_.translation_, move);

	// 移動限界座標
	const float kMoveLimitX = 32.5f;
	const float kMoveLimitY = 17.5f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Player::ReticleUpdate(const ViewProjection& viewProjection) {
	POINT pMousePosition;
	// マウス座標を取得する
	GetCursorPos(&pMousePosition);

	// クライアントエリア領域に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &pMousePosition);

	Vector2 mousePosition = Vector2(float(pMousePosition.x), float(pMousePosition.y));
	sprite2DReticle_->SetPosition(mousePosition);

	Matrix4x4 matViewport =
	    Mymath::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matVPV = Mymath::Multiply(
	    Mymath::Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	Matrix4x4 matInverseVPV = Mymath::Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(mousePosition.x, mousePosition.y, 0);
	Vector3 posFar = Vector3(mousePosition.x, mousePosition.y, 1);

	// ワールド座標へ
	posNear = Mymath::Transform(posNear, matInverseVPV);
	posFar = Mymath::Transform(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Mymath::Normalize(mouseDirection);
	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 50.0f;
	worldTransform3DReticle_.translation_ = mouseDirection * kDistanceTestObject;
	worldTransform3DReticle_.UpdateMatrix();

	Vector2 spritePosition = sprite2DReticle_->GetPosition();
	float* translate[3] = {
	    &worldTransform3DReticle_.translation_.x, &worldTransform3DReticle_.translation_.y,
	    &worldTransform3DReticle_.translation_.z};
	ImGui::Begin("Player");
	ImGui::Text("2DReticle(%f,%f)", spritePosition.x, spritePosition.y);
	ImGui::Text("Direction(%+.2f,%+.2f,%+.2f)", mouseDirection.x, mouseDirection.y, mouseDirection.z);
	ImGui::Text("posNear(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("posFar(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("translate(%+.2f,%+.2f,%+.2f)", *translate[0], *translate[1], *translate[2]);
	ImGui::End();
}