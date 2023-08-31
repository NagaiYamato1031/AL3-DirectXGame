#include "BaseBullet.h"

#include "MyConst.h"
#include "GlobalConfigs.h"


BaseBullet::BaseBullet() {

}

BaseBullet::~BaseBullet() {

}

void BaseBullet::Initialize() {
	worldTransform_.Initialize();
	velocity_ = {0.0f, 0.0f, 0.0f};
	isActive_ = false;

	
	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Base";
	configs->CreateGroup(groupName);

	configs->AddItem(groupName, "sphereSize", kSphereSize);
}


void BaseBullet::Update() { worldTransform_.UpdateMatrix(); }

Vector3 BaseBullet::GetWorldPosition() const {
	Vector3 worldPos;

	// ワールド座標の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void BaseBullet::OnCollision() { isActive_ = false; }

void BaseBullet::AddlyAllGlobalConfigs() {
	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Base";

	kSphereSize = configs->GetFloatValue(groupName, "sphereSize");
	AddlyGlobalConfigs();
}

Sphere BaseBullet::GetSphere() const {
	Sphere sphere;
	sphere.center = worldTransform_.translation_;
	sphere.radius = kSphereSize * worldTransform_.scale_.x;
	return sphere;
}


void BaseBullet::SetWorldPosition(const Vector3& position) {
	worldTransform_.translation_ = position;
}

void BaseBullet::SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
