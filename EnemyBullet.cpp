#include "EnemyBullet.h"

#include "TextureManager.h"
#include <cassert>

#include "Mymath.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULL �|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	// �e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black1x1.png");

	// ���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	// ���x��ݒ�
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	// ���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// ���W���ړ�������
	worldTransform_.translation_ += velocity_;

	// ���[���h�g�����X�t�H�[���̍X�V
	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// ���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}