#pragma once
#include "model.h"
#include "math.h"
#include <cassert>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin/affin.h"
class PlayerBullet {
  public:
	//������
	void Init(Model* model, const Vector3& position, const Vector3& velocity);
	//�X�V
	void Update();
	//�`��
	void Draw(const ViewProjection& viewProjection);

	bool isDead() const {return isDead_;}

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂̌��m������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	float GetRadius();
  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//
	// ViewProjection viewProjection_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_;

	//
	static const int32_t kLifeTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
	//�A�t�B��
	affin::AffinMat affinMat;

	float radius = 1;
};