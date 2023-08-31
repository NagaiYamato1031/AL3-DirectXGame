#pragma once
#include "BaseCharacter.h"
#include <optional>

class BulletManager;
class Player;

class Enemy : public BaseCharacter {
public:
	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void CappedUpdate();

	void Draw() override;

	void OnCollision() override;

	void AddlyGlobalConfigs() override;

	void SetPlayer(Player* player);

private:
	// 弾を撃つ処理とか
	void AttackCircle();

	void AttackBlock();

	
	// 通常行動初期化
	void BehaviorRootInitialize();

	// 通常行動更新
	void BehaviorRootUpdate();


	// 攻撃行動初期化
	void BehaviorAttackInitialize();

	// 攻撃行動更新
	void BehaviorAttackUpdate();

	// 攻撃中の動作
	void PreAttack();
	void ExecuteAttack();
	void PostAttack();

	// 被弾行動
	void BehaviorDamageInitialize();

	// 被弾行動更新
	void BehaviorDamageUpdate();

	// 被弾中の動作
	void ExecuteDamage();
	void PostDamage();

private:
	enum EnemyPartsIndex {
		kEnemyBody,

		kEnemyCount,
	};

	// 状態
	enum class Behavior {
		kRoot,   // 通常状態
		kAttack, // 攻撃
		kDamage, // 被弾
	};
	// 状態の振る舞い
	enum class StateBehavior {
		kPreState,     // 前段階
		kExecuteState, // 実行段階
		kPostState,    // 後段階
	};

private:

	BulletManager* bulletManager_ = nullptr;

	Player* player_ = nullptr;

	int32_t health_ = 60;

	int32_t kMaxHealth_ = 60;

	// 行動時のパラメーター
	float stepParameter_ = 0;

	int32_t attackCoolTime = 60;

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;

	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	StateBehavior stateBehavior_ = StateBehavior::kPreState;
	std::optional<StateBehavior> aBehaviorRequest_ = std::nullopt;
};
