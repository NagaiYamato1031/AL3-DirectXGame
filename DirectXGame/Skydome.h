#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// 天球
/// </summary>
class Skydome {
public:
	/// <summary>
	///  初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	///	更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private: // メンバ変数
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;
};