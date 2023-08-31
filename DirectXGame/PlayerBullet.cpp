#include "PlayerBullet.h"

#include "GlobalConfigs.h"
#include "MyConst.h"
#include "Mymath.h"

PlayerBullet::PlayerBullet() {}
PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Initialize() {
	worldTransform_.Initialize();
	velocity_ = {0.0f, 0.0f, 0.0f};
	isActive_ = true;
	activeTime_ = 120;

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "PlayerBullet";
	configs->CreateGroup(groupName);

	configs->AddItem(groupName, "activeTime", activeTime_);
}

void PlayerBullet::Update() {

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();

	// フレームで消す
	activeTime_--;

	if (activeTime_ <= 0) {
		isActive_ = false;
	}
}

void PlayerBullet::OnCollision() { isActive_ = false; }

void PlayerBullet::AddlyGlobalConfigs() {
	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "PlayerBullet";

	activeTime_ = configs->GetIntValue(groupName, "activeTime");
}