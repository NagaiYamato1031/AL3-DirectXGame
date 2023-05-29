#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <list>
#include "EnemyBullet.h"

class Player;

enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

public: // ゲッターセッター
	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

private: // メンバ関数

	void InitApproach();

	void Approach();
	void Leave();
	void Fire();

private: // メンバ変数

	// 弾の発射間隔
	static const int kFireInterval = 60;


	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	// フェーズ
	Phase phase_ = Phase::Approach;

	// 弾
	std::list<EnemyBullet*> bullets_;

	// 発射タイマー
	int32_t fireTimer = 0;

	Player* player_;

};
