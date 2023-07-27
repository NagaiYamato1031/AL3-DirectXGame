#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"

class Player {
public:
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
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjectioin">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjectioin);

	Vector3 GetWorldPosition();

public: // ゲッターセッター
	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

	const WorldTransform* GetWorldTransform() { return &worldTransform_; }

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	
private: // メンバ関数
private: // メンバ変数
	// ワールド変換データ
	WorldTransform worldTransform_;
	// 3D モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
};
