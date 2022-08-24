#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include <cassert>
#include <list>
#include <memory>

#include "EnemyBullet.h"

//���@�N���X�̑O���錾
class Player;

//�Q�[���V�[���̑O���錾
class GameScene;

class Enemy {
	enum class Phase {
		None,
		Approach, //�ڋ߂���
		Leave,    //���E����
	};

  public:
	/// <summary>
	/// ����
	/// </summary>

	void Init(Model* model, uint32_t textureHandle, const Vector3& v);

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

	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	///
	/// </summary>

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂̌��m������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//
	float GetRadius();
	//�G���X�g
	const std::list<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }

	bool isDead() const { return isDead_; }

  private:
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	//���f��
	Model* model_ = nullptr;
	Model* modelBullet_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//
	DebugText* debugText_ = nullptr;
	//���x
	Vector3 velocity_ = {0.0f, 0.0f, 0.1f};
	//�ڋߑ��x
	Vector3 approach_ = {0.0f, 0.0f, -0.05f};
	//���E���x
	Vector3 leave_ = {-0.15f, 0.1f, 0.1f};

	// Affin�֐��̍\����
	affin::AffinMat affinMat;
	//�ł��o���܂ł̎���
	float dalayTimer = 0.0f;
	//�G�̍s���p�^�[��
	Phase phase_ = Phase::None;

	//�f�X�t���O
	bool isDead_ = false;

	//�G���X�g
	std::list<std::unique_ptr<Enemy>> enemys_;

	//���L����
	Player* player_ = nullptr;
	float radius = 1;

	//�Q�[���V�[��
	GameScene* gameScene_ = nullptr;
};
