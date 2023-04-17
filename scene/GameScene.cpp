#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");
	sprite_ = Sprite::Create(textureHandle_, {40, 50});
	// 3D モデルの生成
	model_ = Model::Create();
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// サウンドデータハンドルの初期化
	soundDataHandle_ = audio_->LoadWave("mokugyo.wav");

	// 音声再生
	// voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
	// ライン描画が参照する
	//PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向が参照するビュープロジェクションを指定する（
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

}

void GameScene::Update() {
	Vector2 position = sprite_->GetPosition();
	// { 2 , 1 } ずつ移動させる
	position.x += 2.0f;
	position.y += 1.0f;
	sprite_->SetPosition(position);

	// スペースキーを押した瞬間
	if (input_->TriggerKey(DIK_SPACE)) {
		// 音声停止
		audio_->StopWave(voiceHandle_);
	}
	ImGui::ShowDemoWindow();

	ImGui::Begin("Debug1");
	// float3 入力ボックス
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	// float3 スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 0.1f);

	// デバッグテキストの表示
	// ImGui::Text("Kamata Tarou %d %d %d", 2050, 12, 31);
	ImGui::End();
	// デバッグカメラ
	debugCamera_->Update();
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
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// ラインを描画する
	for (int i = 0; i < 17; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		    {(8.0f - i) * padding_, 0, 8.0f * padding_},
		    {(8.0f - i) * padding_, 0, -8.0f * padding_}, {1.0f, 0.2f, 0.0f, 1.0f});
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		    {8.0f * padding_, 0, (8.0f - i) * padding_},
		    {-8.0f * padding_, 0, (8.0f - i) * padding_}, {1.0f, 0.2f, 0.0f, 1.0f});
	}
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
