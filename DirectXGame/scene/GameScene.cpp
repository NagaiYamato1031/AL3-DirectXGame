#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "TextureManager.h"

#include <cassert>

#include "Ground.h"
#include "Skydome.h"

#include "FollowCamera.h"
#include "Player.h"

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

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	// 天球のモデル
	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));
	// 天球の初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	// 地面のモデル
	groundModel_.reset(Model::CreateFromOBJ("ground", true));
	// 地面の初期化
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());

	// プレイヤーのモデル
	modelPlayerBody_.reset(Model::CreateFromOBJ("player_Body", true));
	modelPlayerHead_.reset(Model::CreateFromOBJ("player_Head", true));
	modelPlayerL_arm_.reset(Model::CreateFromOBJ("player_L_arm", true));
	modelPlayerR_arm_.reset(Model::CreateFromOBJ("player_R_arm", true));
	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialze(
	    modelPlayerBody_.get(), modelPlayerHead_.get(), modelPlayerL_arm_.get(),
	    modelPlayerR_arm_.get());

	player_->SetViewProjection(&followCamera_->GetViewProjection());
	followCamera_->SetTarget(player_->GetWorldTransform());

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	AxisIndicator::GetInstance()->Update();

	//  プレイヤーの更新
	player_->Update();
	// 追従カメラの更新
	followCamera_->Update();

	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	viewProjection_.TransferMatrix();
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
	ground_->Draw(viewProjection_);

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
