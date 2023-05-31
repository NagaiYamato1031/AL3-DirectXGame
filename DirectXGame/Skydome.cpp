#include "Skydome.h"

void Skydome::Initialize(Model* model) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {300.0f, 300.0f, 300.0f};

	worldTransform_.UpdateMatrix();
}

void Skydome::Update() {}

void Skydome::Draw(const ViewProjection& viewProjeciton) {
	model_->Draw(worldTransform_, viewProjeciton);
}