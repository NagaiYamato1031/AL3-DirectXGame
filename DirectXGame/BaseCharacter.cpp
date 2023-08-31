#include "BaseCharacter.h"

#include "MyConst.h"
#include "GameScene.h"

#include "GlobalConfigs.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models) {
	models_ = models;
	worldTransformBase_.Initialize();
	worldTransforms_.clear();

	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Base";
	configs->CreateGroup(groupName);

	configs->AddItem(groupName, "sphereSize", kSphereSize);

	
	worldTransformBase_.UpdateMatrix();

	for (WorldTransform& wt : worldTransforms_) {
		wt.UpdateMatrix();
	}
}

void BaseCharacter::Update() {
	worldTransformBase_.UpdateMatrix();
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void BaseCharacter::Draw() {
	for (Model* model : models_) {
		model->Draw(worldTransformBase_, *viewProjection_);
	}
}

void BaseCharacter::OnCollision() {

}

void BaseCharacter::AddlyAllGlobalConfigs() {
	GlobalConfigs* configs = GlobalConfigs::GetInstance();
	const char* groupName = "Base";

	kSphereSize = configs->GetFloatValue(groupName, "sphereSize");
	AddlyGlobalConfigs();
}



Sphere BaseCharacter::GetSphere() const {
	Sphere sphere;
	sphere.center = worldTransformBase_.translation_;
	sphere.radius = kSphereSize * worldTransformBase_.scale_.x;
	return sphere;
}


Vector3 BaseCharacter::GetWorldPosition() const {
	Vector3 worldPos;

	// worldPos = Mymath::TransformNormal(worldTransform_.translation_, worldTransform_.matWorld_);

	// ワールド座標の平行移動成分を取得
	worldPos.x = worldTransformBase_.matWorld_.m[3][0];
	worldPos.y = worldTransformBase_.matWorld_.m[3][1];
	worldPos.z = worldTransformBase_.matWorld_.m[3][2];
	return worldPos;
}

void BaseCharacter::SetGameScene(GameScene* scene) { gameScene_ = scene; }