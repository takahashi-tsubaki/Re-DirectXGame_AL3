#include "RailCamera.h"
using namespace MathUtility;
void RailCamera::Init(const Vector3& position, const Vector3& rotation) { 

	input_ = Input::GetInstance();
	//�V���O���C���X�^���X���擾����
	debugText_ = DebugText::GetInstance();

	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	worldTransform_.Initialize();
	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.farZ = 300.0f;
	viewProjection_.Initialize();
	
}

void RailCamera::Update() 
{ 
	Vector3 move = {0, 0, 0};

	const float kCharaSpeed = 0.2f;

	Matrix4 matTrans;

	worldTransform_.matWorld_ = {1, 0, 0, 0,  // x
	                             0, 1, 0, 0,  // y
	                             0, 0, 1, 0,  // z
	                             0, 0, 0, 1}; // Tx,Ty,Tz
#pragma region �ړ�����
	if (input_->PushKey(DIK_UP)) {
		move.z+= kCharaSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.z -= kCharaSpeed;
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	matTrans = affin::generateTransMat(worldTransform_);
#pragma endregion

#pragma region ��]����
	Vector3 Rotation = {0, 0, 0};
	const float kCharaRotX = ConvertToRadians(0.5f);

	Matrix4 matRotX;

	//��]����
	if (input_->PushKey(DIK_O)) {
		worldTransform_.rotation_.x += kCharaRotX;
	}
	if (input_->PushKey(DIK_P)) {
		worldTransform_.rotation_.x -= kCharaRotX;
	}
	//(-)2�΂𒴂������]�p��0�ɖ߂�����
	if (worldTransform_.rotation_.x >= (PI * 2) || worldTransform_.rotation_.x <= -(PI * 2)) {
		worldTransform_.rotation_.x = 0;
	}

	matRotX = affin::generateRotateXMat(worldTransform_);
#pragma endregion

	worldTransform_.matWorld_.operator*=(matRotX);

	worldTransform_.matWorld_.operator*=(matTrans);

	//�s��̓]��
	worldTransform_.TransferMatrix();

	//
	viewProjection_.eye = worldTransform_.translation_;
	//�O���x�N�g��
	Vector3 forward(0,0,1);
	//���[���J�����̉�]�𔽉f
	forward = affin::matVector(forward,worldTransform_.matWorld_);
	//���_����O���ɓK���ɐi�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye + forward;

	//����x�N�g��
	Vector3 up(0, 1, 0);
	//
	viewProjection_.up = affin::matVector(up, worldTransform_.matWorld_);

	//�r���[�v���W�F�N�V�����̍X�V�ƍs��o�b�t�@�ւ̓]��
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	//eye�̕\��
	debugText_->SetPos(20, 160);
	debugText_->Printf(
	  "rail : (%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y,
	  viewProjection_.eye.z);

}
float RailCamera::ConvertToRadians(float fDegrees) noexcept { return fDegrees * (PI / 180.0f); }
float RailCamera::ConvertToDegrees(float fRadians) noexcept { return fRadians * (180.0f / PI); }

WorldTransform* RailCamera::GetWorldPosition() {
	//���[���h���W�����邽�߂̕ϐ�
	WorldTransform worldPos;

	//�߂�l
	return &worldPos;
}