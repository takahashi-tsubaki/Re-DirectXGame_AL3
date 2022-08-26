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
#include "afin/afin.h"
#include "WinApp.h"
#include "MathUtility.h"

class Player {
  public:
	Player();
	//
	void Initialize(Model* model, uint32_t textureHandle);
	//
	void Update(const ViewProjection& viewProjection);
	//
	void Draw(ViewProjection viewProjection);
	//
	void DrawUI();

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

	//�e�q�\���̃Z�b�^�[
	void SetParent(WorldTransform* worldTransform);

  private:
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;
	//
	
	//3D���e�B�N���p���[���h�g�����X�t�H�[��
	WorldTransform worldTransform3DReticle_;
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

	//2D���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite> sprite2DReticle_;

	//�F
	Vector4 RED = {1.0f, 0.0f, 0.0f, 1.0f};

	WinApp* winApp_ = nullptr;

};
