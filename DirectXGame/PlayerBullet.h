#pragma once

#include "BaseBullet.h"


class PlayerBullet : public BaseBullet {
public:

	PlayerBullet();
	~PlayerBullet() override;

	void Initialize() override;

	void Update() override;

	void OnCollision() override;

	/// <summary>
	/// 調整項目の適用
	/// </summary>
	void AddlyGlobalConfigs() override;


private:
	// 何フレームで消えるか
	int32_t activeTime_ = 0;
};
