#include "EnemyBullet.h"
void EnemyBullet::Init(Model* model, const Vector3& position, const Vector3& velocity) 
{
	// NULL�|�C���^
	assert(model);

	//
	model_ = model;
	//�e�N�X�`���n���h���̓ǂݍ���
	textureHandle_ = TextureManager::Load("EnemyBullet.png");

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	//�����Ŏ󂯂Ƃ����������W���Z�b�g
	worldTransform_.translation_ = {position.x, position.y, position.z};
	/*affinMat.translate = affin::generateTransMat(worldTransform_);*/

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
}

void EnemyBullet::Update() 
{
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	//���W���ړ�������
	worldTransform_.translation_ -= velocity_;
	affinMat.translate = affin::generateTransMat(worldTransform_);
	affin::setTransformationWolrdMat(affinMat, worldTransform_);
	//�s��̍���
	worldTransform_.TransferMatrix();


}

void EnemyBullet::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}