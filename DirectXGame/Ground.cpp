#include "Ground.h"

void Ground::Initialize(Model* model) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {500.0f, 500.0f, 500.0f};

	worldTransform_.UpdateMatrix();
}

void Ground::Update() {}

void Ground::Draw(const ViewProjection& viewProjeciton) {
	model_->Draw(worldTransform_, viewProjeciton);
}