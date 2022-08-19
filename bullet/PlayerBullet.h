#pragma once
#include "model.h"
#include "math.h"
#include <cassert>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin/affin.h"
class PlayerBullet {
  public:
	//初期化
	void Init(Model* model, const Vector3& position, const Vector3& velocity);
	//更新
	void Update();
	//描画
	void Draw(const ViewProjection& viewProjection);

	bool isDead() const {return isDead_;}

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突の検知したら呼び出されるコールバック関数
	void OnCollision();

	float GetRadius();
  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//
	// ViewProjection viewProjection_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;

	//
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
	//アフィン
	affin::AffinMat affinMat;

	float radius = 1;
};