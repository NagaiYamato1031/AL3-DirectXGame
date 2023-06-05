#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "TextureManager.h"

#include <cassert>

#include "Skydome.h"

#include "Enemy.h"
#include "EnemyBullet.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "RailCamera.h"

#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
	for (Enemy* enemy : enemyLists_) {
		delete enemy;
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	delete skydome_;
	delete skydomeModel_;
	delete player_;
	delete playerModel_;
	//delete enemy_;
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

	// レールカメラの作成
	railCamera_ = new RailCamera();
	// レールカメラの初期化
	railCamera_->SetFarZ(2000.0f);
	railCamera_->Initialize({0.0f, 0.0f, -10.0f}, {0.0f, 0.0f, 0.0f});

	// 天球の作成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(skydomeModel_);

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	Vector3 playerPosition{0.0f, 0.0f, 20.0f};
	player_->Initialze(playerModel_, textureHandle_, playerPosition);
	player_->SetParent(&railCamera_->GetWorldTransform());
	player_->SetGameScene(this);

	// エネミーの生成
	//enemy_ = new Enemy();
	// エネミーの初期化
	//enemy_->Initialize(enemyModel_, enemyTextureHandle_);

	//enemy_->SetPlayer(player_);
	//enemy_->SetGameScene(this);
	/*Enemy* enemy = new Enemy();
	enemy->Initialize(enemyModel_, enemyTextureHandle_);
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
	enemyLists_.push_back(enemy);*/

	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	LoadEnemyPopData();

}

void GameScene::Update() {
	

	// デスフラグの立った弾を削除
	playerBullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	enemyLists_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
#ifdef _DEBUG

	ImGui::Begin("Debug");
	if (ImGui::RadioButton("RailCamera", isRailCameraActive_)) {
		isRailCameraActive_ = true;
		isDebugCameraActive_ = false;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("DebugCamera", isDebugCameraActive_)) {
		isDebugCameraActive_ = true;
		isRailCameraActive_ = false;
	}
	ImGui::End();
	/*if (input_->TriggerKey(DIK_BACKSPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}*/
#endif // _DEBUG

	if (isRailCameraActive_) {
		// レールカメラの更新
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		// ビュープロジェクションの転送
		viewProjection_.TransferMatrix();
	} else if (isDebugCameraActive_) {
		// レールカメラの更新
		railCamera_->Update();
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクションの転送
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
	UpdateEnemyPopCommands();
	// エネミーの更新
	for (Enemy* enemy : enemyLists_) {
		enemy->Update();
	}
	// プレイヤーの更新
	player_->Update();
	// 弾更新
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

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

	for (Enemy* enemy : enemyLists_) {
		enemy->Draw(viewProjection_);
	}
	player_->Draw(viewProjection_);
	// 弾描画
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Draw(viewProjection_);
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
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

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {
	// 対象対象 A と B の座標
	Vector3 posA, posB;
	// 自弾リストの取得
	// const std::list<PlayerBullet*>& playerBullets_ = player_->GetBullets();
	// 敵弾リストの取得
	// const std::list<EnemyBullet*>& enemyBullets_ = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemyBullets_) {
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

	for (Enemy* enemy : enemyLists_) {
		posA = enemy->GetWorldPosition();
		for (PlayerBullet* bullet : playerBullets_) {
			// 敵弾の座標
			posB = bullet->GetWorldPosition();

			float distance = Mymath::Length(posA - posB);
			if (distance <= kEnemyRadius + kPlayerBulletRadius) {
				enemy->OnCollision();
				bullet->OnCollision();
			}
		}
	}

#pragma endregion
#pragma region 自弾と敵弾の当たり判定

	for (EnemyBullet* bulletA : enemyBullets_) {
		posA = bulletA->GetWorldPosition();
		for (PlayerBullet* bulletB : playerBullets_) {
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

void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) {
	// プレイヤーの弾を追加
	playerBullets_.push_back(playerBullet);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// エネミーの弾を追加
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();

}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (isWaitPopEnemy) {
		waitPopEnemyTimer--;
		if (waitPopEnemyTimer <= 0) {
			// 待機官僚
			isWaitPopEnemy = false;
		}
		return;
	}

	// 1 行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (std::getline(enemyPopCommands,line)) {
		// 1 行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// , 区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');
		// "//" から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}
		// POP コマンド
		if (word.find("POP") == 0) {
			// x 座標
			std::getline(line_stream, word, ',');
			float x = (float)std::atoi(word.c_str());
			// y 座標
			std::getline(line_stream, word, ',');
			float y  = (float)std::atoi(word.c_str());
			// z 座標
			std::getline(line_stream, word, ',');
			float z = (float)std::atoi(word.c_str());
			// 敵を発生させる
			PopEnemy(Vector3(x, y, z));
		}
		// WAIT コマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');
			// 待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			// 待機開始
			isWaitPopEnemy = true;
			waitPopEnemyTimer = waitTime;
			// コマンドループを抜ける
			break;
		}

	}

}

void GameScene::PopEnemy(const Vector3& position) {
	Enemy* enemy = new Enemy();
	enemy->Initialize(enemyModel_, enemyTextureHandle_);
	enemy->SetPlayer(player_);
	enemy->SetGameScene(this);
	enemy->SetPosition(position);
	enemyLists_.push_back(enemy);
}