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
	
	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//攻撃
	void Attack();
	//キャラの向きに応じた方向に球を出す
	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

	//衝突の検知したら呼び出されるコールバック関数
	void OnCollision();
	//
	//弾リスト
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	float GetRadius();

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//インプット
	Input* input_ = nullptr;
	//デバックテキスト
	DebugText* debugText_ = nullptr;

	const float PI = 3.141592654f;
	//打ち出すまでの時間
	float dalayTimer = 0.0f;
	//Affin関数の構造体
	affin::AffinMat affinMat;

	////球
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	float radius = 1;
};
