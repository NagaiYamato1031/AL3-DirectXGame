#pragma once
#include "BaseCharacter.h"
#include "Input.h"
#include <optional>

// 前方宣言
class BulletManager;

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

	// タイトルでの移動
	void CappedUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(uint32_t texture);


	void OnCollision() override;


	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void AddlyGlobalConfigs() override;

	Sphere GetSphere() const override;

	void ResetDistance() { distance_ = 100.0f; }

	Vector3 GetTheta() const { return theta_;
	}

private: // メンバ関数

	// 移動判定
	void MovementUpdate();

	// 攻撃判定
	void AttackiingUpdate();

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

	// 浮遊ギミック初期化
	void InitializeFloatingGimmick();

	// 浮遊ギミック更新
	void UpdateFloatingGimmick();

private: // メンバ変数
	enum PartsIndex {
		kPlayerBody,
		// kPlayerHead,
		// kPlayerL_arm,
		// kPlayerR_arm,
		// kPlayerWeapon,

		kPlayerCount,
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


	// キーボード入力
	Input* input_ = nullptr;

	// 弾管理クラス
	BulletManager* bulletManager_ = nullptr;



	// 原点からの距離
	float distance_ = 20.0f;

	// 原点からの角度 ( x , y )
	Vector3 theta_ = {0.0f, 0.0f, 0.0f};

	// 縦方向の移動制限
	float limitMoveRadianY_ = 0.6f;


	// 攻撃の媒介変数
	float stateParameter_ = 0.0f;

	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	// 浮遊時のサイクルフレーム
	int floatingCycle_ = 60;

	// 浮遊時の振幅
	float floatingAmplitude_ = 0.5f;

	// 浮遊時の浮いてる距離
	float floatingPadding_ = 2.0f;

	// 弾の進む速度
	float bulletSpeed_ = 2.0f;

	// 体力
	int32_t health_ = 3;


	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;

	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	StateBehavior stateBehavior_ = StateBehavior::kPreState;
	std::optional<StateBehavior> aBehaviorRequest_ = std::nullopt;
};
