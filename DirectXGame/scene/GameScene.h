#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "DebugCamera.h"


class Skydome;
class Player;
class Enemy;
class RailCamera;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	const float kPlayerRadius = 1.0f;
	const float kPlayerBulletRadius = 1.0f;
	const float kEnemyRadius = 1.0f;
	const float kEnemyBulletRadius = 1.0f;


	// 天球のモデル
	Model* skydomeModel_ = nullptr;

	// プレイヤーのモデル
	Model* playerModel_ = nullptr;
	// エネミーのモデル
	Model* enemyModel_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// エネミーのテクスチャ
	uint32_t enemyTextureHandle_ = 0u;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// レールカメラ
	RailCamera* railCamera_ = nullptr;

	// 天球
	Skydome* skydome_ = nullptr;

	// プレイヤー
	Player* player_ = nullptr;

	// エネミー
	Enemy* enemy_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	// レールカメラ有効
	bool isRailCameraActive_ = true;

};
