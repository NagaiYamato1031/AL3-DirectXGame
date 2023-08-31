#pragma once
#include "Model.h"
#include "ViewProjection.h"

#include <map>
#include <vector>

#include "BaseBullet.h"
#include "BaseCharacter.h"

enum BulletType {
	typePlayer = 0,
	typeEnemy,

	typeCount,
};

using Bullets = std::list<std::unique_ptr<BaseBullet>>;

class BulletManager {
public:
	static BulletManager* GetInstance();

	void Initialize(const std::vector<Model*>& models);

	void Update();

	void Draw();

	// シーン場を移動するときに呼ぶ
	void Clear();

public: // ゲッターセッター
	// const WorldTransform& GetWorldTransform() const { return worldTransformBase_; }

	// Vector3 GetWorldPosition() const;

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}
	/// <summary>
	/// タグと一緒に弾を保存
	/// </summary>
	/// <param name="type">タイプ</param>
	/// <param name="newBullet">弾</param>
	void AddBullet(BulletType type, BaseBullet* newBullet);

	bool IsCollisionSphere(BulletType type, const BaseCharacter& chara);

private:
	BulletManager() = default;
	~BulletManager() = default;
	BulletManager(const BulletManager& obj) = delete;
	const BulletManager& operator=(const BulletManager& obj) = delete;

	const ViewProjection* viewProjection_;

	// 弾の種類と弾のクラスを保存
	std::map<BulletType, Bullets> datas_;

	std::vector<Model*> models_;
};
