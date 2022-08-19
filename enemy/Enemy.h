#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include <cassert>
#include <memory>
#include <list>

#include "EnemyBullet.h"


//���@�N���X�̑O���錾
class Player;

class Enemy
{
	enum class Phase
	{
		None,
		Approach,//�ڋ߂���
		Leave,   //���E����
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
	/// ���E
	/// </summary>
	
	void Leave();

	/// <summary>
	/// ����
	/// </summary>

	void Shot();

	/// <summary>
	/// 
	/// </summary>

	void SetPlayer(Player* player){player_ = player;}

	/// <summary>
	/// 
	/// </summary>
	
	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂̌��m������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();
	//�e���X�g
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }
	//
	float GetRadius();
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
	Vector3 approach_ = {0.0f, 0.0f, -0.05f};
	//���E���x
	Vector3 leave_ = {-0.15f,0.1f,0.1f};
	
	// Affin�֐��̍\����
	affin::AffinMat affinMat;
	//�ł��o���܂ł̎���
	float dalayTimer = 0.0f;
	//�G�̍s���p�^�[��
	Phase phase_ = Phase::None;

	////��
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	
	//���L����
	Player* player_ = nullptr;
	float radius = 1;
};
