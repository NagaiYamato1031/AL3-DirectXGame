#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"

class GameScene;

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
	/// <param name="position">カメラから前にずらす量</param>
	void Initialze(Model* model, uint32_t textureHandle, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const ViewProjection& viewProjectioin);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjectioin">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjectioin);

	/// <summary>
	/// UI 描画
	/// </summary>
	void DrawUI();

	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

public: // ゲッターセッター
	// 弾リストを取得
	// const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent) {
		worldTransform_.parent_ = parent;
		worldTransform3DReticle_.parent_ = parent;
	}

	/// <summary>
	/// ゲームシーンの設定
	/// </summary>
	/// <param name="gameScene">ゲームシーン</param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private: // メンバ関数
	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// レティクルの更新
	/// </summary>
	void ReticleUpdate(const ViewProjection& viewProjection);

private: // メンバ変数
	// ワールド変換データ
	WorldTransform worldTransform_;
	// 3D モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;

	// 3D レティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	Sprite* sprite2DReticle_ = nullptr;

};
