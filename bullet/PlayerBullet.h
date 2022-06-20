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
	void Init(Model* model, const Vector3& position);
	//�X�V
	void Update();
	//�`��
	void Draw(const ViewProjection& viewProjection);

  private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//
	// ViewProjection viewProjection_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//�A�t�B��
	affin::AffinMat affinMat;
};