#pragma once
#include "BaseCharacter.h"
#include "Input.h"
#include <optional>

class Player : public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private: // メンバ関数
	// 通常行動初期化
	void BehaviorRootInitialize();

	// 通常行動更新
	void BehaviorRootUpdate();

	// 攻撃行動初期化
	void BehaviorAttackInitialize();

	// 攻撃行動更新
	void BehaviorAttackUpdate();

	// 浮遊ギミック初期化
	void InitializeFloatingGimmick();

	// 浮遊ギミック更新
	void UpdateFloatingGimmick();

	// 攻撃中の動作
	void PreAttack();
	void ExecuteAttack();
	void PostAttack();

private: // メンバ変数
	enum PartsIndex {
		kPlayerBody,
		kPlayerHead,
		kPlayerL_arm,
		kPlayerR_arm,
		kPlayerWeapon,

		kPlayerCount,
	};

	// ふるまい
	enum class Behavior {
		kRoot,   // 通常状態
		kAttack, // 攻撃中
	};
	// 攻撃時の振る舞い
	enum class AttackBehavior {
		kPreAttack,		// 前段階
		kExecuteAttack,	// 実行段階
		kPostAttack,	// 後段階
	};

private:
	// キーボード入力
	Input* input_ = nullptr;

	// 攻撃の媒介変数
	float attackParameter_ = 0.0f;

	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;

	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	AttackBehavior aBehavior_ = AttackBehavior::kPreAttack;
	std::optional<AttackBehavior> aBehaviorRequest_ = std::nullopt;

};
