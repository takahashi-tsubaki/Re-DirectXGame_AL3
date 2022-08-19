#pragma once
#include "model.h"
#include "math.h"
#include <cassert>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin/affin.h"
class EnemyBullet 
{
public:
	/// <summary>
	/// ����
	/// </summary>

	void Init(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>

	void Update();

	/// <summary>
	/// �`��
	/// </summary>

	//�`��
	void Draw(const ViewProjection& viewProjection);
	//���[���h���W���擾
	Vector3 GetWorldPosition();
	bool isDead() const { return isDead_; }
	//�Փ˂̌��m������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();
	float GetRadius();
  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;


	//�A�t�B��
	affin::AffinMat affinMat;
	//�f�X�t���O
	bool isDead_ = false;
	float radius = 1;
};