#include "BaseCharacter.h"

void BaseCharacter::Initialize(const std::vector<Model*>& models) {
	models_ = models;
	worldTransformBase_.Initialize();
	worldTransforms_.clear();
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

Vector3 BaseCharacter::GetWorldPosition() const {
	Vector3 worldPos;

	// worldPos = Mymath::TransformNormal(worldTransform_.translation_, worldTransform_.matWorld_);

	// ワールド座標の平行移動成分を取得
	worldPos.x = worldTransformBase_.matWorld_.m[3][0];
	worldPos.y = worldTransformBase_.matWorld_.m[3][1];
	worldPos.z = worldTransformBase_.matWorld_.m[3][2];
	return worldPos;
}