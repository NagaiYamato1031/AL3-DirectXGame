#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "TextureManager.h"

#include <cassert>

#include "Ground.h"
#include "Skydome.h"

#include "Enemy.h"
#include "FollowCamera.h"
#include "Player.h"

#include "GlobalConfigs.h"

#include "BulletManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャを読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	viewProjection_.translation_.y = 5;

	// テクスチャ
	textureMove_ = TextureManager::Load("Move.png");
	textureShot_ = TextureManager::Load("Shot.png");
	textureEnter_ = TextureManager::Load("Enter.png");
	textureB_ = TextureManager::Load("B.png");

	texturePlayer_ = TextureManager::Load("player.png");

	spriteMove_ = Sprite::Create(textureMove_, {140, 520}, {1, 1, 1, 1}, {0.5f, 0.5f});
	spriteShot_ = Sprite::Create(textureShot_, {1100, 600}, {1, 1, 1, 1}, {0.5f, 0.5f});
	spriteEnter_ = Sprite::Create(textureEnter_, {600, 600}, {1, 1, 1, 1} ,{0.5f, 0.5f});
	spriteB_ = Sprite::Create(textureB_, {700, 600}, {1, 1, 1, 1}, {0.5f, 0.5f});

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	// 天球のモデル
	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));
	// 天球の初期化
	skydome_ = std::make_unique<Skydome>();

	skydome_->Initialize(skydomeModel_.get());

	//// 地面のモデル
	// groundModel_.reset(Model::CreateFromOBJ("ground", true));
	//// 地面の初期化
	// ground_ = std::make_unique<Ground>();
	// ground_->Initialize(groundModel_.get());

	// プレイヤーのモデル
	modelPlayerBody_.reset(Model::CreateFromOBJ("sphere", true));
	// modelPlayerHead_.reset(Model::CreateFromOBJ("player_Head", true));
	// modelPlayerL_arm_.reset(Model::CreateFromOBJ("player_L_arm", true));
	// modelPlayerR_arm_.reset(Model::CreateFromOBJ("player_R_arm", true));
	// modelPlayerWeapon_.reset(Model::CreateFromOBJ("player_Weapon", true));

	std::vector<Model*> playerModels = {
	    modelPlayerBody_.get(),
	};

	/*
	{modelPlayerHead_.get(), modelPlayerL_arm_.get(),
	modelPlayerR_arm_.get(), modelPlayerWeapon_.get()};
	*/
	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize(playerModels);

	player_->SetViewProjection(&followCamera_->GetViewProjection());
	followCamera_->SetTarget(&player_->GetWorldTransform());

	// エネミー
	modelEnemyBody_.reset(Model::CreateFromOBJ("enemy", true));
	// modelEnemyWeapon_.reset(Model::CreateFromOBJ("enemyWeapon", true));

	std::vector<Model*> enemyModels = {modelEnemyBody_.get()};

	// エネミーの生成
	enemy_ = std::make_unique<Enemy>();
	// エネミーの初期化
	enemy_->Initialize(enemyModels);
	enemy_->SetViewProjection(&followCamera_->GetViewProjection());

	modelPlayerBullet_.reset(Model::CreateFromOBJ("sphere", true));
	modelEnemyBullet_.reset(Model::CreateFromOBJ("sphere", true));

	enemy_->SetPlayer(player_.get());

	std::vector<Model*> bulletModles = {modelPlayerBullet_.get(), modelEnemyBullet_.get()};

	// 弾管理
	bulletManager_ = BulletManager::GetInstance();
	bulletManager_->Initialize(bulletModles);
	bulletManager_->SetViewProjection(&followCamera_->GetViewProjection());

	player_->SetGameScene(this);
	enemy_->SetGameScene(this);

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Base";
	configs->CreateGroup(groupName);

	configs->AddItem(groupName, "sphereSize", 1.0f);

	srand((unsigned)time(NULL));

#ifdef _DEBUG

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

#endif // _DEBUG
}

void GameScene::Update() {

#ifdef _DEBUG

	AxisIndicator::GetInstance()->Update();

#endif // _DEBUG

	if (nextScene) {
		// シーンを変更
		nowScene = nextScene.value();
		// 振る舞いごとの初期化
		switch (nowScene) {
		case Scene::kTitle:
			TitleInitialize();
			break;
		case Scene::kPlaying:
			PlayingInitialize();
			break;
		case Scene::kEnd:
			if (isEnd == 1) {
				ClearInitialize();
			} else if (isEnd == 2) {
				FailInitialize();
			}
			break;
		default:
			break;
		}
		// リセット
		nextScene = std::nullopt;
		bulletManager_->Clear();
	}

	switch (nowScene) {
	case Scene::kTitle:
		TitleUpdate();
		break;
	case Scene::kPlaying:
		PlayingUpdate();
		break;
	case Scene::kEnd:
		if (isEnd == 1) {
			ClearUpdate();
		} else if (isEnd == 2) {
			FailUpdate();
		}
		break;
	default:
		break;
	}
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
	///

	switch (nowScene) {
	case Scene::kTitle:
		Title3DDraw();
		break;
	case Scene::kPlaying:
		Playing3DDraw();
		break;
	case Scene::kEnd:
		if (isEnd == 1) {
			Clear3DDraw();
		} else if (isEnd == 2) {
			Fail3DDraw();
		}
		break;
	default:
		break;
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	switch (nowScene) {
	case Scene::kTitle:
		TitleFSpriteDraw();
		break;
	case Scene::kPlaying:
		PlayingFSpriteDraw();
		break;
	case Scene::kEnd:
		if (isEnd == 1) {
			ClearFSpriteDraw();
		} else if (isEnd == 2) {
			FailFSpriteDraw();
		}
		break;
	default:
		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::TitleInitialize() {
	nextScene = Scene::kTitle;
	isEnd = 0;
	player_->ResetDistance();
}
void GameScene::TitleUpdate() {
	player_->CappedUpdate();
	enemy_->CappedUpdate();
	followCamera_->Update();
	// カメラの更新
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	viewProjection_.TransferMatrix();

	XINPUT_STATE joyState;
	// ゲームパッドが有効なら
	if (input_->GetJoystickState(0, joyState)) {
		// 攻撃
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			PlayingInitialize();
		}
	}
	if (input_->TriggerKey(DIK_SPACE)) {
		PlayingInitialize();
	}
}
void GameScene::Title3DDraw() {
	skydome_->Draw(viewProjection_);
	enemy_->Draw();
	player_->Draw(texturePlayer_);
}
void GameScene::TitleFSpriteDraw() {
	spriteMove_->Draw();
	spriteShot_->Draw();
}

void GameScene::PlayingInitialize() {
	nextScene = Scene::kPlaying;
	player_->AddlyAllGlobalConfigs();
	enemy_->AddlyAllGlobalConfigs();
}
void GameScene::PlayingUpdate() {

	// エネミーの更新
	enemy_->Update();
	//  プレイヤーの更新
	player_->Update();
	// 追従カメラの更新
	followCamera_->Update();

	// 弾を管理
	bulletManager_->Update();

	// 当たり判定を取る
	CheckAllCollision();

	// カメラの更新
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	viewProjection_.TransferMatrix();
}
void GameScene::Playing3DDraw() {

	skydome_->Draw(viewProjection_);

	enemy_->Draw();
	player_->Draw(texturePlayer_);

	bulletManager_->Draw();
}
void GameScene::PlayingFSpriteDraw() {
	spriteMove_->Draw();
	spriteShot_->Draw();
}

void GameScene::ClearInitialize() {
	nextScene = Scene::kEnd;
	isEnd = 1;
}

void GameScene::ClearUpdate() {
	//  プレイヤーの更新
	player_->Update();
	// 追従カメラの更新
	followCamera_->Update();
	// カメラの更新
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	viewProjection_.TransferMatrix();

	XINPUT_STATE joyState;
	// ゲームパッドが有効なら
	if (input_->GetJoystickState(0, joyState)) {
		// 攻撃
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			TitleInitialize();
		}
	}
	if (input_->TriggerKey(DIK_RETURN)) {
		TitleInitialize();
	}
}
void GameScene::Clear3DDraw() {
	skydome_->Draw(viewProjection_);

	player_->Draw(texturePlayer_);
}
void GameScene::ClearFSpriteDraw() {
	spriteEnter_->Draw();
	spriteB_->Draw();
}

void GameScene::FailInitialize() {
	nextScene = Scene::kEnd;
	isEnd = 2;
}

void GameScene::FailUpdate() {
	//  プレイヤーの更新
	// player_->Update();
	// 追従カメラの更新
	// followCamera_->Update();
	// カメラの更新
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	viewProjection_.TransferMatrix();

	XINPUT_STATE joyState;
	// ゲームパッドが有効なら
	if (input_->GetJoystickState(0, joyState)) {
		// 攻撃
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			TitleInitialize();
		}
	}
	if (input_->TriggerKey(DIK_RETURN)) {
		TitleInitialize();
	}
}
void GameScene::Fail3DDraw() {
	skydome_->Draw(viewProjection_);

	enemy_->Draw();
	// player_->Draw();
}
void GameScene::FailFSpriteDraw() {
	spriteEnter_->Draw();
	spriteB_->Draw();
}

void GameScene::CheckAllCollision() {
	if (bulletManager_->IsCollisionSphere(typeEnemy, *player_.get())) {
		player_->OnCollision();
	}
	if (bulletManager_->IsCollisionSphere(typePlayer, *enemy_.get())) {
		enemy_->OnCollision();
	}
}