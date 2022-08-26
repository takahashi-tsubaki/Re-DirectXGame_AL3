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

	//親子構造のセッター
	void SetParent(WorldTransform* worldTransform);

  private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//
	
	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
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

	//2Dレティクル用スプライト
	std::unique_ptr<Sprite> sprite2DReticle_;

	//色
	Vector4 RED = {1.0f, 0.0f, 0.0f, 1.0f};

	WinApp* winApp_ = nullptr;

};
