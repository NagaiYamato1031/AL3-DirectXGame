#pragma once
#include "WorldTransform.h"

#include <stdint.h>

struct Sphere;

class BaseBullet {
public:
	BaseBullet();
	virtual ~BaseBullet();

	virtual void Initialize();

	virtual void Update();

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
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	Vector3 GetWorldPosition() const;
	void SetWorldPosition(const Vector3& position);

	void SetVelocity(const Vector3& velocity);


	uint16_t GetIsActive() const { return isActive_; }

protected:
	
	float kSphereSize = 1.0f;

	WorldTransform worldTransform_;

	Vector3 velocity_;

	// 判定するか
	uint16_t isActive_;

};
