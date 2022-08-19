#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include "bullet/PlayerBullet.h"
#include <cassert>
#include <memory>
#include <list>
class Player {
  public:
	Player();
	//
	void Initialize(Model* model, uint32_t textureHandle);
	//
	void Update();
	//
	void Move();
	//
	void Draw(ViewProjection viewProjection);

	float ConvertToRadians(float fDegrees) noexcept;
	float ConvertToDegrees(float fRadians) noexcept;
	
	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�U��
	void Attack();
	//�L�����̌����ɉ����������ɋ����o��
	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

	//�Փ˂̌��m������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();
	//
	//�e���X�g
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	float GetRadius();

private:
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//�C���v�b�g
	Input* input_ = nullptr;
	//�f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;

	const float PI = 3.141592654f;
	//�ł��o���܂ł̎���
	float dalayTimer = 0.0f;
	//Affin�֐��̍\����
	affin::AffinMat affinMat;

	////��
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	float radius = 1;
};
