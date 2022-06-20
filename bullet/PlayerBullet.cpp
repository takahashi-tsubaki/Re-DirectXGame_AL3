#include "PlayerBullet.h"
//����������
void PlayerBullet::Init(Model* model, const Vector3& position) {
	// NULL�|�C���^
	assert(model);

	//
	model_ = model;
	//�e�N�X�`���n���h���̓ǂݍ���
	textureHandle_ = TextureManager::Load("bullet.png");

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	//�����Ŏ󂯂Ƃ����������W���Z�b�g
	worldTransform_.translation_ = {position.x, position.y, position.z};
	affinMat.translate = affin::generateTransMat(worldTransform_);
}
//�X�V����
void PlayerBullet::Update() {
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	affin::setTransformationWolrdMat(affinMat, worldTransform_);
	worldTransform_.TransferMatrix();
}
//�`�揈��
void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}