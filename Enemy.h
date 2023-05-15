#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private: // メンバ関数

	void Approach();
	void Leave();

private: // メンバ変数
	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	// フェーズ
	Phase phase_ = Phase::Approach;
};
