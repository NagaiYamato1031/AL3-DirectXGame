#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <vector>

class BaseCharacter {
public:
	virtual void Initialize(const std::vector<Model*>& models);

	virtual void Update();

	virtual void Draw(const ViewProjection& viewProjection);

public: // ゲッターセッター
	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

	const WorldTransform* GetWorldTransform() const { return &worldTransform_; }

	const Vector3& GetWorldPosition() const;

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

protected:
	std::vector<Model*> models_;

	WorldTransform worldTransform_;

	const ViewProjection* viewProjection_;
};
