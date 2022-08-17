#include "enemy.h"
#include "player/Player.h"
#include "MathUtility.h"
using namespace MathUtility;

void Enemy::Init(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	//�����Ŏ󂯂Ƃ����������W���Z�b�g
	worldTransform_.translation_ = {15,0,50};
	
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

	switch (phase_) {
	case Phase::Approach://�ڋ߃t�F�[�Y
	default:
		//�ړ�
		Approach();
		break;

	//case Phase::Leave://���E�t�F�[�Y
	//	Leave();
	//	break;
	}

	//�s��̌v�Z
	affinMat.translate = affin::generateTransMat(worldTransform_);
	//�s��̍���
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();

	//���̔��˂���֐�
	Shot();

	//���̍X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
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
	//���̕`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Approach()
{
	worldTransform_.translation_ += approach_; 
	//����̈ʒu�ɓ��B�����珉���ʒu�ɖ߂�
	if (worldTransform_.translation_.z <= -40.0f)
	{
		worldTransform_.translation_.z = 50.0f;
	}
	////����̈ʒu�ɓ��B�����痣�E
	//if (worldTransform_.translation_.z <= 0.0f)
	//{
	//	phase_ = Phase::Leave;
	//}
}

void Enemy::Leave()
{ 
	worldTransform_.translation_ += leave_; 
}

void Enemy::Shot()
{
	dalayTimer -= 0.1f;

	////���̑��x
	const float kBulletSpeed = 0.5f;

	//-------���@�_���e-------//

	assert(player_);

	/*const float kBulletSpeed = 1.0f;*/

	//�v���C���[�̃��[���h���W�̎擾
	Vector3 playerPosition;
	playerPosition = player_->GetWorldPosition();
	//�G�̃��[���h���W�̎擾
	Vector3 enemyPosition;
	enemyPosition = GetWorldPosition();

	Vector3 distance(0,0,0);

	//�����x�N�g�������߂�
	distance =  enemyPosition - playerPosition;

	//���������߂�
	Vector3Length(distance);
	//���K��
	Vector3Normalize(distance);
	//�x�N�g���̒�����,�����ɍ��킹��
	distance *= kBulletSpeed;//���ꂪ���x�ɂȂ�


	//�N�[���^�C�����O�ɂȂ����Ƃ�
	if (dalayTimer <= 0) {
		//���̐���
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		//���̏�����
		newBullet->Init(model_, worldTransform_.translation_, distance);

		//���̓o�^
		bullets_.push_back(std::move(newBullet));

		dalayTimer = 20.0f;
	}
}
Vector3 Enemy::GetWorldPosition() {
	//���[���h���W�����邽�߂̕ϐ�
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	//�߂�l
	return worldPos;
}
