#include "EnemyBullet.h"

#include "GlobalConfigs.h"
#include "MyConst.h"
#include "Mymath.h"

EnemyBullet::EnemyBullet() {}
EnemyBullet::~EnemyBullet() {}

void EnemyBullet::Initialize() {
	worldTransform_.Initialize();
	velocity_ = {0.0f, 0.0f, 0.0f};
	isActive_ = true;
	activeTime_ = 120;

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "EnemyBullet";
	configs->CreateGroup(groupName);

	configs->AddItem(groupName, "activeTime", activeTime_);
}

void EnemyBullet::Update() {

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();

	// フレームで消す
	activeTime_--;

	if (activeTime_ <= 0) {
		isActive_ = false;
	}
}

void EnemyBullet::OnCollision() { isActive_ = false; }

void EnemyBullet::AddlyGlobalConfigs() {
	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "EnemyBullet";

	activeTime_ = configs->GetIntValue(groupName, "activeTime");
}