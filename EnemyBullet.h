#pragma once
#include "model.h"
#include "math.h"
#include <cassert>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin/affin.h"
class EnemyBullet 
{
public:
	/// <summary>
	/// 生成
	/// </summary>

	void Init(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>

	void Update();

	/// <summary>
	/// 描画
	/// </summary>

	//描画
	void Draw(const ViewProjection& viewProjection);
	//ワールド座標を取得
	Vector3 GetWorldPosition();
	bool isDead() const { return isDead_; }
	//衝突の検知したら呼び出されるコールバック関数
	void OnCollision();
	float GetRadius();
  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;


	//アフィン
	affin::AffinMat affinMat;
	//デスフラグ
	bool isDead_ = false;
	float radius = 1;
};