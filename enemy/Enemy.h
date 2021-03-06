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

class Enemy
{
	enum class Phase
	{
		None,
		Approach,//接近する
		Leave,   //離脱する
	};
public:
	/// <summary>
	/// 生成
	/// </summary>
  
	void Init(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
  
	void Draw(ViewProjection viewProjection);

	/// <summary>
	/// 接近
	/// </summary>

	void Approach();

	/// <summary>
	/// 離脱
	/// </summary>
	
	void Leave();

	/// <summary>
	/// 発射
	/// </summary>

	void Shot();

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//
	DebugText* debugText_ = nullptr;
	//速度
	Vector3 velocity_ = {0.0f,0.0f,0.1f};
	//接近速度
	Vector3 approach_ = {0.0f, 0.0f, -0.05f};
	//離脱速度
	Vector3 leave_ = {-0.15f,0.1f,0.1f};
	// Affin関数の構造体
	affin::AffinMat affinMat;
	//打ち出すまでの時間
	float dalayTimer = 0.0f;
	//敵の行動パターン
	Phase phase_ = Phase::None;

	////球
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

};