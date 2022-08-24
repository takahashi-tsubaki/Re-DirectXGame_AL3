#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "math.h"

#include "Player/Player.h"
#include "RailCamera.h"
#include "enemy/Enemy.h"
#include "skydome.h"

#include "sstream"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	      /// <summary>
	      /// コンストクラタ
	      /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	///
	/// </summary>

	void Move(ViewProjection viewProjection_);
	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision();

	//弾リスト
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	////敵リスト
	// const std::list<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet);

	/// <summary>
	/// 敵弾の更新
	/// </summary>
	void EnemyBulletUpdate();

	/// <summary>
	/// 敵発生
	/// </summary>
	void EnemyOcurrence(const Vector3& v);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	//モデルの生成
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;

	//デバックカメラの生成
	DebugCamera* debugCamera_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t reticleHandle_ = 0;
	uint32_t enemyHandle_ = 0;

	//スプライト
	Sprite* sprite_ = nullptr;

	ViewProjection viewProjection_;
	ViewProjection debugViewProjection_;

	bool isDebugCameraActive_ = false;

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	Enemy* enemy_ = nullptr;

	//天球
	skydome* skydome_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	////敵弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//敵
	std::list<std::unique_ptr<Enemy>> enemys_;

	//敵の打ち出すまでの時間
	float enemyDalayTimer = 0.0f;

	bool isWait_ = false;

	int waitTimer = 300;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//
	GameScene* gameScene_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
