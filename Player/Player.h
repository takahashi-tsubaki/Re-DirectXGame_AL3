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

	//攻撃
	void Attack();
	//キャラの向きに応じた方向に球を出す
	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

  private:
	//
	WorldTransform worldTransform_;
	//
	Model* model_ = nullptr;
	//
	uint32_t textureHandle_ = 0u;
	//
	Input* input_ = nullptr;
	//
	DebugText* debugText_ = nullptr;

	const float PI = 3.141592654f;

	affin::AffinMat affinMat;

	////球
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};
