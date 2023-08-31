#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <vector>

struct Sphere;
class GameScene;


class BaseCharacter {
public:
	virtual void Initialize(const std::vector<Model*>& models);

	virtual void Update();

	virtual void Draw();

	virtual void OnCollision();

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	virtual void AddlyGlobalConfigs() = 0;
	virtual void AddlyAllGlobalConfigs();

	virtual Sphere GetSphere() const;

public: // ゲッターセッター
	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォーム</param>
	void SetParent(const WorldTransform* parent) { worldTransformBase_.parent_ = parent; }

	const WorldTransform& GetWorldTransform() const { return worldTransformBase_; }

	Vector3 GetWorldPosition() const;

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	void SetGameScene(GameScene* scene);


protected:

	float kSphereSize = 1.0f;

	GameScene* gameScene_ = nullptr;

	std::vector<Model*> models_;
	std::vector<WorldTransform> worldTransforms_;

	WorldTransform worldTransformBase_;

	const ViewProjection* viewProjection_;
};
