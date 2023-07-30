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

class FollowCamera;
class Player;
class Enemy;

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

	// プレイヤー追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	// 天球のモデル
	std::unique_ptr<Model> skydomeModel_;
	// 天球
	std::unique_ptr<Skydome> skydome_;

	// 地面のモデル
	std::unique_ptr<Model> groundModel_;
	// 地面
	std::unique_ptr<Ground> ground_;

	// プレイヤーのモデル
	std::unique_ptr<Model> modelPlayerBody_;
	std::unique_ptr<Model> modelPlayerHead_;
	std::unique_ptr<Model> modelPlayerL_arm_;
	std::unique_ptr<Model> modelPlayerR_arm_;
	std::unique_ptr<Model> modelPlayerWeapon_;
	// プレイヤー
	std::unique_ptr<Player> player_;

	// エネミーのモデル
	std::unique_ptr<Model> modelEnemyBody_;
	std::unique_ptr<Model> modelEnemyWeapon_;
	// エネミー
	std::unique_ptr<Enemy> enemy_;
};
