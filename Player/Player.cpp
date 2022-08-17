#include "Player.h"
Player::Player() {}
void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	
	worldTransform_.Initialize();
	
	worldTransform_.rotation_ = {};

	worldTransform_.translation_ = {};

	//
}


void Player::Update() {

	//�f�X�t���O�������������폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) 
	{ 
			return bullet->isDead();
	});

	Vector3 move = {0, 0, 0};

	const float kCharaSpeed = 0.2f;
	
	Matrix4 matTrans;

	worldTransform_.matWorld_ = {1, 0, 0, 0,  // x
	                             0, 1, 0, 0,  // y
	                             0, 0, 1, 0,  // z
	                             0, 0, 0, 1}; // Tx,Ty,Tz
#pragma region �ړ�����
	//�ړ�����
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharaSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		move.x += kCharaSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += kCharaSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		move.y -= kCharaSpeed;
	
	} 

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	matTrans = affin::generateTransMat(worldTransform_);
#pragma endregion

#pragma region ��]����
	Vector3 Rotation = {0, 0, 0};
	const float kCharaRotY = ConvertToRadians(0.5f);

	Matrix4 matRotY;

	//��]����
	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y += kCharaRotY;
	}
	if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y -= kCharaRotY;
	}
	//(-)2�΂𒴂������]�p��0�ɖ߂�����
	if (worldTransform_.rotation_.y >= (PI * 2) || worldTransform_.rotation_.y <= -(PI * 2)) {
		worldTransform_.rotation_.y = 0;
	}
	

	matRotY = affin::generateRotateYMat(worldTransform_);
#pragma endregion

	worldTransform_.matWorld_.operator*=(matRotY);

	worldTransform_.matWorld_.operator*=(matTrans);
	
	const float kMoveLimitX = 36.0f;
	const float kMoveLimitY = 19.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//�֐��̍���
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();

	//�U���֐�
	Attack();

	//���̍X�V
	for (std::unique_ptr<PlayerBullet>&bullet : bullets_) 
	{
		bullet->Update();
	}
	

	//�f�o�b�N�e�L�X�g
	debugText_->SetPos(20, 100);
	debugText_->Printf(
	  "translation : (%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

	debugText_->SetPos(20, 120);
	debugText_->Printf(
	  "rotation : (%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	  worldTransform_.rotation_.z);
	debugText_->SetPos(20,80);
	debugText_->Printf("dalay : %f",dalayTimer );

}
//�`�揈��
void Player::Draw(ViewProjection viewProjection) 
{
	//�v���C���[�̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//���̕`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) 
	{
		bullet->Draw(viewProjection);
	}

}

float Player::ConvertToRadians(float fDegrees) noexcept { return fDegrees * (PI / 180.0f); }
float Player::ConvertToDegrees(float fRadians) noexcept { return fRadians * (180.0f / PI); }

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE))
	{

		dalayTimer-=0.1f;

		//���̑��x
		const float kBulletSpeed = 0.5f;

		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = bVelocity(velocity, worldTransform_);

		//�N�[���^�C�����O�ɂȂ����Ƃ�
		if (dalayTimer <= 0) 
		{
			//���̐���
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//���̏�����
			newBullet->Init(model_, worldTransform_.translation_, velocity);

			//���̓o�^
			bullets_.push_back(std::move(newBullet));

			dalayTimer = 5.0f;
		}
		
	}
}

Vector3 Player::bVelocity(Vector3& velocity, WorldTransform& worldTransform) {
	
	Vector3 result = {0,0,0};

	//����
	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
	           velocity.y * worldTransform.matWorld_.m[1][2] +
	           velocity.z * worldTransform.matWorld_.m[2][2];

	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
	           velocity.y * worldTransform.matWorld_.m[1][0] +
	           velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
	           velocity.y * worldTransform.matWorld_.m[1][1] +
	           velocity.z * worldTransform.matWorld_.m[2][1];

	return result;
}

Vector3 Player::GetWorldPosition()
{ 
	//���[���h���W�����邽�߂̕ϐ�
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	//�߂�l
	return worldPos;
}