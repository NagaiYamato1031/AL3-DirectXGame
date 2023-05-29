#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"

#include <list>

class PlayerBullet;

class Player {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialze(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjectioin">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjectioin);

	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

public: // ゲッターセッター

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

private: // メンバ関数
	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

private: // メンバ変数
	// ワールド変換データ
	WorldTransform worldTransform_;
	// 3D モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;
};
