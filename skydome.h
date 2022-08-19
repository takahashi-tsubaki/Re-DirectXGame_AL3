#pragma once
#include <cassert>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "affin/affin.h"
class skydome 
{
public:

	/// <summary>
	/// ������
	/// </summary>
  void Init(Model* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection_);

private:
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;
	//
	ViewProjection viewProjection_;
	//���f��
	Model* model_ = nullptr;
	// Affin�֐��̍\����
	affin::AffinMat affinMat;
};