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


	WorldTransform worldTransform_;

	ViewProjection viewProjection_;


	//頂点
	Vector3 vertex[8] = 
	{
		// x     y     z
		{0.0f,0.0f, 0.0f},//0
		{20.0f,0.0f, 0.0f},//1
		{20.0f,0.0f, 20.0f},//2
		{0.0f, 0.0f, 20.0f},//3

		{0.0f, 20.0f,0.0f},//4
		{20.0f,20.0f,0.0f},//5
		{20.0f,20.0f,20.0f},//6
		{0.0f, 20.0f,20.0f},//7
	};

	int edgeList[12][2] = 
	{
		{0,1}, 
		{1,2}, 
		{2,3}, 
		{3,0}, 
		{0,4},
		{1,5}, 
		{2,6},
		{3,7},
		{4,5},
		{5,6},
		{6,7},
		{7,4}
	};

	//色				  R    G    B    A
	Vector4 WHITE = {1.0f,1.0f,1.0f,1.0f};

	Vector4 BLUE = {0.0f,0.0f,1.0f,1.0f};

	Vector4 GREEN = {0.0f, 1.0f, 0.0f,1.0f};

	Vector4 RED = {1.0f, 0.0f, 0.0f, 1.0f};

	float afinTranslation[3][3] = {
	  {1.0f, 0.0f, 100.0f}, //拡大、縮小率、X平行移動値
	  {0.0f, 1.0f, 100.0f}, //回転(sin,cos)、Y平行移動値
	  {0.0f, 0.0f, 1.0f  }
    };

	float afinScale[3][3] = {
	 
	};

	// 45度の回転
	float afinRotation[3][3] = {
	  
	};


	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
