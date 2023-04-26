#include "WorldTransform.h"

#include "Mymath.h"

void WorldTransform::UpdateMatrix() {
	// スケール、回転、平行移動
	matWorld_ = Mymath::MakeAffineMatrix4x4(scale_, rotation_, translation_);
	// 定数バッファに転送する
	TransferMatrix();
}
