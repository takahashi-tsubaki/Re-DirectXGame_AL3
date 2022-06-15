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

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数

	  enum partId
	  {
		kRoot,  //大元
		kSpine, //脊髄
		kChest, //胸
		kHead,  //頭
		kHip,   //ケツ
		kArmL,  //左腕
		kArmR,  //右腕
		kLegL,  //左足
		kLegR  //右足
	  };

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

	void UpdateMatrix(WorldTransform worldTransform_[8], int num,Vector3 move);

	int Discrimination(int num);

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

	WorldTransform worldTransforms_[100];

	ViewProjection viewProjection_;

	float PI = 3.141592654f;

	float viewAngle = 0.0f;

	float ConvertToRadians(float fDegrees) noexcept; 
	float ConvertToDegrees(float fRadians) noexcept;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
