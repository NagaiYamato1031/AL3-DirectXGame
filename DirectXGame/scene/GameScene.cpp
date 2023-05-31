#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"

#include <cassert>

#include "Skydome.h"

#include "Enemy.h"
#include "Player.h"
#include "EnemyBullet.h"
#include "PlayerBullet.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete skydome_;
	delete skydomeModel_;
	delete player_;
	delete playerModel_;
	delete enemy_;
	delete enemyModel_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャを読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");
	// エネミーのテクスチャ
	enemyTextureHandle_ = TextureManager::Load("uvChecker.png");

	// 3D モデルの生成
	skydomeModel_ = Model::CreateFromOBJ("skydome", true);

	// プレイヤーのモデル
	playerModel_ = Model::Create();
	// エネミーのモデル
	enemyModel_ = Model::Create();

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 天球の作成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(skydomeModel_);

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialze(playerModel_, textureHandle_);

	// エネミーの生成
	enemy_ = new Enemy();
	// エネミーの初期化
	enemy_->Initialize(enemyModel_, enemyTextureHandle_);

	enemy_->SetPlayer(player_);

	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_BACKSPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG

	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクションの転送
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}

	// エネミーの更新
	if (enemy_ != nullptr) {
		enemy_->Update();
	}
	// プレイヤーの更新
	player_->Update();

	CheckAllCollision();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>


	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	skydome_->Draw(viewProjection_);

	if (enemy_ != nullptr) {
		enemy_->Draw(viewProjection_);
	}
	player_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {
	// 対象対象 A と B の座標
	Vector3 posA, posB;
	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		float distance = Mymath::Length(posA - posB);
		if (distance <= kPlayerRadius + kEnemyBulletRadius) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}



#pragma endregion
#pragma region 敵キャラと自弾の当たり判定
	
	
	posA = enemy_->GetWorldPosition();
	for (PlayerBullet* bullet : playerBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		float distance = Mymath::Length(posA - posB);
		if (distance <= kEnemyRadius + kPlayerBulletRadius) {
			enemy_-> OnCollision();
			bullet->OnCollision();
		}
	}


#pragma endregion
#pragma region 自弾と敵弾の当たり判定

	for (EnemyBullet* bulletA : enemyBullets) {
		posA = bulletA->GetWorldPosition();
		for (PlayerBullet* bulletB : playerBullets) {
			// 敵弾の座標
			posB = bulletB->GetWorldPosition();

			float distance = Mymath::Length(posA - posB);
			if (distance <= kPlayerBulletRadius + kEnemyBulletRadius) {
				bulletA->OnCollision();
				bulletB->OnCollision();
			}
		}
	}


#pragma endregion
}