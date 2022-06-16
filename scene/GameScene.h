#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "math.h"

#include "Player/Player.h"

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

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	Model* model_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t reticleHandle_ = 0;

	//
	Sprite* sprite_ = nullptr;

	ViewProjection viewProjection_;
	ViewProjection debugViewProjection_;

	bool isDebugCameraActive_ = false;

	Player* player;

	affin::AffinMat affinMat;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
