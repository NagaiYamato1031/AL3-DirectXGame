#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Mymath.h"
/// <summary>
/// レールカメラ
/// </summary>
class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& translate,const Vector3& rotate);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; };

	/// <summary>
	/// プロジェクション行列計算用のメンバ設定関数群
	/// </summary>
	void SetFovAngleY(float value) { viewProjection_.fovAngleY = value; }
	void SetAspectRatio(float value) { viewProjection_.aspectRatio = value; }
	void SetNearZ(float value) { viewProjection_.nearZ = value; }
	void SetFarZ(float value) { viewProjection_.farZ = value; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private: // メンバ変数
	// ワールド変換データ
	WorldTransform worldTransform_;

	// ビュープロジェクション
	ViewProjection viewProjection_;
};
