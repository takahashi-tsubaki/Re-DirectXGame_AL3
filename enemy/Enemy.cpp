#include "enemy.h"
void Enemy::Init(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	//�����Ŏ󂯂Ƃ����������W���Z�b�g
	worldTransform_.translation_ = {0,5,50};
	
}

void Enemy::Update() 
{
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	//���W���ړ�������
	worldTransform_.translation_ -= velocity_;
	affinMat.translate = affin::generateTransMat(worldTransform_);
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();

	//�G����ʊO�ɍs�����ۂɍ��W�������l�֖߂�����
	if (worldTransform_.translation_.z <= -50)
	{
		worldTransform_.translation_.z = 50;
	}

	//�f�o�b�N�e�L�X�g
	debugText_->SetPos(20, 140);
	debugText_->Printf(
	  "enemyTranslation : (%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

void Enemy::Draw(ViewProjection viewProjection) 
{ 
	model_->Draw(worldTransform_,viewProjection,textureHandle_); 
}