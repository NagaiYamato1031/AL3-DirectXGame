#pragma once

#include "BaseBullet.h"

class EnemyBullet : public BaseBullet {
public:
	EnemyBullet();
	~EnemyBullet() override;

	void Initialize() override;

	void Update() override;

	void OnCollision() override;

	
	void AddlyGlobalConfigs() override;

private:

	int32_t activeTime_ = 0;

};
