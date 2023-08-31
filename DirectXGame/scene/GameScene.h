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

class BulletManager;

enum class Scene {
	kTitle,
	kPlaying,
	kEnd,
};

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

	void TitleInitialize();
	void PlayingInitialize();

	void ClearInitialize();
	void FailInitialize();

private: // メンバ関数
	void TitleUpdate();
	void Title3DDraw();
	void TitleFSpriteDraw();

	void PlayingUpdate();
	void Playing3DDraw();
	void PlayingFSpriteDraw();

	void ClearUpdate();
	void Clear3DDraw();
	void ClearFSpriteDraw();
	void FailUpdate();
	void Fail3DDraw();
	void FailFSpriteDraw();

	void CheckAllCollision();

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

	// スプライトのハンドル
	uint32_t textureMove_ = 0;
	uint32_t textureShot_ = 0;
	// uint32_t textureSpace_ = 0;
	uint32_t textureEnter_ = 0;
	uint32_t textureB_ = 0;

	uint32_t texturePlayer_ = 0;

	Sprite* spriteMove_ = nullptr;
	Sprite* spriteShot_ = nullptr;
	// Sprite* spriteSpace_ = nullptr;
	Sprite* spriteEnter_ = nullptr;
	Sprite* spriteB_ = nullptr;

	Scene nowScene = Scene::kTitle;

	std::optional<Scene> nextScene = std::nullopt;

	// 0 クリアしていない(使わない)
	// 1 成功
	// 2 失敗
	int32_t isEnd = 0;

	// プレイヤー追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	// 天球のモデル
	std::unique_ptr<Model> skydomeModel_;
	// 天球
	std::unique_ptr<Skydome> skydome_;

	// プレイヤーのモデル
	std::unique_ptr<Model> modelPlayerBody_;
	// プレイヤー
	std::unique_ptr<Player> player_;

	// エネミーのモデル
	std::unique_ptr<Model> modelEnemyBody_;
	// エネミー
	std::unique_ptr<Enemy> enemy_;

	// 弾のモデル
	std::unique_ptr<Model> modelPlayerBullet_;
	std::unique_ptr<Model> modelEnemyBullet_;

	// 弾管理
	BulletManager* bulletManager_ = nullptr;
};
