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

//自機クラスの前方宣言
class Player;

//ゲームシーンの前方宣言
class GameScene;

class Enemy {
	enum class Phase {
		None,
		Approach, //接近する
		Leave,    //離脱する
	};

  public:
	/// <summary>
	/// 生成
	/// </summary>

	void Init(Model* model, uint32_t textureHandle, const Vector3& v);

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

	/// <summary>
	///
	/// </summary>

	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	///
	/// </summary>

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突の検知したら呼び出されるコールバック関数
	void OnCollision();

	//
	float GetRadius();
	//敵リスト
	const std::list<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }

	bool isDead() const { return isDead_; }

  private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//モデル
	Model* model_ = nullptr;
	Model* modelBullet_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//
	DebugText* debugText_ = nullptr;
	//速度
	Vector3 velocity_ = {0.0f, 0.0f, 0.1f};
	//接近速度
	Vector3 approach_ = {0.0f, 0.0f, -0.05f};
	//離脱速度
	Vector3 leave_ = {-0.15f, 0.1f, 0.1f};

	// Affin関数の構造体
	affin::AffinMat affinMat;
	//打ち出すまでの時間
	float dalayTimer = 0.0f;
	//敵の行動パターン
	Phase phase_ = Phase::None;

	//デスフラグ
	bool isDead_ = false;

	//敵リスト
	std::list<std::unique_ptr<Enemy>> enemys_;

	//自キャラ
	Player* player_ = nullptr;
	float radius = 1;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;
};
