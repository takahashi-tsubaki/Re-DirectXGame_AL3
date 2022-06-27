#include "enemy.h"
void Enemy::Init(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	//�����Ŏ󂯂Ƃ����������W���Z�b�g
	worldTransform_.translation_ = {0,3,50};
	
	//�����o�֐��|�C���^�Ɋ֐��̃A�h���X��������
	void (Enemy::*Enemy::spFuncTable[])() = {
	  &Enemy::Approach, //�v�f�ԍ�0
	  &Enemy::Leave,    //�v�f�ԍ�1
	};
	
}

void Enemy::Update() 
{
	//�P�ʍs���ݒ�
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	////���W���ړ�������
	//worldTransform_.translation_ -= velocity_;
	
	////�G����ʊO�ɍs�����ۂɍ��W�������l�֖߂�����
	//if (worldTransform_.translation_.z <= -50)
	//{
	//	worldTransform_.translation_.z = 50;
	//}

	//switch (phase_) {
	//case Phase::Approach://�ڋ߃t�F�[�Y
	//default:
	//	//�ړ�
	//	Approach();
	//	break;

	//case Phase::Leave://���E�t�F�[�Y
	//	Leave();
	//	break;
	//}
	
	//�����o�֐��|�C���^�ɓ����Ă���֐����Ăяo��
	(this->*spFuncTable[0])();

	//�s��̌v�Z
	affinMat.translate = affin::generateTransMat(worldTransform_);
	//�s��̍���
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();
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

void Enemy::Approach()
{
	worldTransform_.translation_ += approach_; 
	//����̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z <= 0.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave()
{ 
	worldTransform_.translation_ += leave_; 
}