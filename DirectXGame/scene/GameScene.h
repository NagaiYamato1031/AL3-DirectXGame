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

#include <memory>

class Skydome;
class Ground;

class Player;

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

private: // メンバ関数
private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	const float kPlayerRadius = 1.0f;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// デバッグカメラのフラグ
	bool isDebugCameraActive_ = false;

	// 天球のモデル
	std::unique_ptr<Model> skydomeModel_;
	// 天球
	std::unique_ptr<Skydome> skydome_;

	// 地面のモデル
	std::unique_ptr<Model> groundModel_;
	// 地面
	std::unique_ptr<Ground> ground_;

	// プレイヤーのモデル
	std::unique_ptr<Model> playerModel_;
	// プレイヤー
	std::unique_ptr<Player> player_;
};
