#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include <cassert>
#include <memory>
#include <list>
class Enemy
{
	enum class Phase {
		None,
		Approach, //�ڋ߂���
		Leave,    //���E����
	};

public:
	/// <summary>
	/// ����
	/// </summary>
  
	void Init(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
  
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// �ڋ�
	/// </summary>

	void Approach();

	/// <summary>
	/// 
	/// </summary>
	
	void Leave();

private:
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//
	DebugText* debugText_ = nullptr;
	//���x
	Vector3 velocity_ = {0.0f,0.0f,0.1f};
	//�ڋߑ��x
	Vector3 approach_ = {0.0f, 0.0f, -0.1f};
	//���E���x
	Vector3 leave_ = {-0.15f,0.1f,0.1f};
	// Affin�֐��̍\����
	affin::AffinMat affinMat;

	//�G�̍s���p�^�[��
	Phase phase_ = Phase::None;

	//�����o�֐��|�C���^
	static void (Enemy::*spFuncTable[])();
	
};