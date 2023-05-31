#include "Enemy.h"

#include <cassert>

#include "Matrix4x4.h"
#include "Mymath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include "Player.h"

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_.x = 10;
	worldTransform_.translation_.y = 5;
	worldTransform_.translation_.z = 50;

	// Fire();
	InitApproach();
}

void Enemy::Update() {

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Vector3 move{0.0f, 0.0f, 0.0f};

	switch (phase_) {
	case Phase::Approach:
	default:
		Approach();
		break;
	case Phase::Leave:
		Leave();
		break;
	}

	// 移動限界座標
	const float kMoveLimitX = 50.0f;
	const float kMoveLimitY = 30.0f;
	const float kMoveLimitZ = 50.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	worldTransform_.translation_.z = max(worldTransform_.translation_.z, -kMoveLimitZ);
	worldTransform_.translation_.z = min(worldTransform_.translation_.z, +kMoveLimitZ);

	// 行列の更新
	worldTransform_.UpdateMatrix();
	// 発射タイマーをデクリメント
	fireTimer--;
	if (fireTimer <= 0) {
		Fire();
		fireTimer = kFireInterval;
	}

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// 3D モデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	//worldPos = Mymath::TransformNormal(worldTransform_.translation_, worldTransform_.matWorld_);
	
	// ワールド座標の平行移動成分を取得
	 worldPos.x = worldTransform_.matWorld_.m[3][0];
	 worldPos.y = worldTransform_.matWorld_.m[3][1];
	 worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {
	// 何もしない
}


void Enemy::InitApproach() {
	// 発射タイマーの設定
	fireTimer = 30;
}

void Enemy::Approach() {
	Vector3 move{0.0f, 0.0f, 0.0f};
	move.z = -0.2f;
	// 移動速度
	worldTransform_.translation_ += move;
	// 既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
void Enemy::Leave() {
	Vector3 move{0.0f, 0.0f, 0.0f};
	move.x = 0.2f;
	move.y = 0.1f;
	// 移動
	worldTransform_.translation_ += move;
}

void Enemy::Fire() {

	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 1.0f;

	Vector3 playerPosition = player_->GetWorldPosition();
	Vector3 enemyPosition = this->GetWorldPosition();

	Vector3 enemyToPlayer = playerPosition - enemyPosition;
	Vector3 normalize = Mymath::Normalize(enemyToPlayer);

	Vector3 vel = normalize * kBulletSpeed;

	// Vector3 velocity{0, 0, kBulletSpeed};

	// 速度ベクトルを自機の向きに合わせて回転する
	// vel = Mymath::TransformNormal(vel, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, vel);

	// 弾を登録する
	bullets_.push_back(newBullet);
}