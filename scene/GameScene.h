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

	WorldTransform worldTransforms_[100];

	ViewProjection viewProjection_;

	float PI = 3.141592f;

	//頂点
	Vector3 vertex[8] = 
	{
		// x     y     z
		{0.0f,0.0f, 0.0f},//0
		{10.0f,0.0f, 0.0f},//1
		{10.0f,0.0f, 10.0f},//2
		{0.0f, 0.0f, 10.0f},//3

		{0.0f, 10.0f,0.0f},//4
		{10.0f,10.0f,0.0f},//5
		{10.0f,10.0f,10.0f},//6
		{0.0f, 10.0f,10.0f},//7
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

	Vector4 BLACK = {0.0f, 0.0f, 0.0f, 1.0f};

	Vector4 PURPLE = {1.0f, 0.0f, 1.0f, 1.0f};

	Vector3 translationVertex[8] = {};
	Vector3 scaleVertex[8] = {};
	Vector3 rotaXVertex[8] = {};
	Vector3 rotaYVertex[8] = {};
	Vector3 rotaZVertex[8] = {};


	float afinTranslation[4][4] =
	{
	  {1.0f, 0.0f, 0.0f, 10.0f},
	  {0.0f, 1.0f, 0.0f, 10.0f},
	  {0.0f, 0.0f, 1.0f, 10.0f},
	  {0.0f, 0.0f, 0.0f, 1.0f},//Tx,Ty,Tz,1
    };

	float afinScale[4][4] = 
	{
		{2.0f,0.0f,0.0f,0.0f},//x軸
		{0.0f,2.0f,0.0f,0.0f},//y軸
		{0.0f,0.0f,2.0f,0.0f},//z軸
		{0.0f,0.0f,0.0f,1.0f},//？
	};

	// 45度の回転
	float afinRotationX[4][4] = 
	{
		{1.0f,0.0f, 0.0f, 0.0f}, //x = x
		{0.0f,cos(PI/4),sin(PI/4),0.0f},// y=ycosΘ-zsinΘ
		{0.0f,-sin(PI/4),cos(PI/4),0.0f},//z=ysinΘ+zcosΘ
		{0.0f,0.0f,0.0f,1.0f},//
	  
	};

	float afinRotationY[4][4] = {
	  {cos(PI / 4),0.0f,-sin(PI / 4),0.0f},//x=xcosΘ+zsinΘ
	  {0.0f,1.0f,0.0,0.0f},//y=y
	  {sin(PI / 4),0.0f,cos(PI / 4),0.0f},//z=xsinΘ+zcosΘ
	  {0.0f,0.0f,0.0f,1.0f},//
	};
	float afinRotationZ[4][4] = {
	  {cos(PI / 4),sin(PI / 4),0.0f,0.0f},//x=cosΘ-ysinΘ
	  {-sin(PI / 4),cos(PI / 4),0.0f,0.0f},//y=xsinΘ+ycosΘ
	  {0.0f,0.0f,1.0f,0.0f},//z=z
	  {0.0f,0.0f,0.0f,1.0f},//
	};

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
