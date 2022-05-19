#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	reticleHandle_ = TextureManager::Load("reticle.png");

	//ランダムシード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	////乱数範囲の指定                              最小値   最大値
	//std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
	////乱数エンジンを渡し、指定範囲からランダムな数値を得る
	//float value = dist(engine);

	//乱数角用の乱数範囲の設定
	std::uniform_real_distribution<float> rot(0.0f, 180.0f);
	//乱数エンジンを渡し、指定範囲からランダムな数値を得る
	float rotate = rot(engine);

	//座標用の乱数範囲の設定
	std::uniform_real_distribution<float> pos(-10.0f, 10.0f);
	//乱数エンジンを渡し、指定範囲からランダムな数値を得る
	float position = pos(engine);

	Matrix4 matScale;

	Matrix4 matRot;

	Matrix4 matRotX, matRotY, matRotZ;

	Matrix4 matTrans = MathUtility::Matrix4Identity();

	for (WorldTransform& worldTransform : worldTransforms_) {
		//ワールドトランスフォームの初期化
		worldTransform.Initialize();
		// X,Y,Z方向のスケーリングを設定
		worldTransform.scale_ = {2.0, 2.0, 2.0};
		//　X,Y,Z軸周りの回転角を設定
		worldTransform.rotation_ = {rotate, rotate, rotate};

		worldTransform.translation_ = {position, position, position};

		//スケーリング倍率を行列に設定
		matScale = {
		  2.0f, 0.0f, 0.0f, 0.0f, 
		  0.0f, 2.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 2.0f, 0.0f, 
		  0.0f, 0.0f, 0.0f, 1.0f,
		};

		//回転行列の各要素を設定

		matRotZ = {
		  rotate, rotate, 0.0f, 0.0f, 
		  rotate, rotate, 0.0f, 0.0f,
		  0.0f,   0.0f,   1.0f, 0.0f,
		  0.0f,   0.0f,   0.0f, 1.0f,
		};

		matRotX = {
		  1.0f, 0.0f, 0.0f, 0.0f, 
		  0.0f, rotate, rotate, 0.0f,
		  0.0f, rotate, rotate, 0.0f,
		  0.0f, 0.0f, 0.0f, 1.0f,
		};

		matRotY = {
		  rotate, 0.0f, rotate, 0.0f,
		  0.0f, 1.0f, 0.0f, 0.0f,
		  rotate, 0.0f, rotate, 0.0f, 
		  0.0f, 0.0f,   0.0f,   1.0f,
		};


		//移動量を行列に設定
		matTrans = {
		  1.0f, 0.0f, 0.0f, 0.0f,
		  0.0f,  1.0f,0.0f, 0.0f,
		  0.0f, 0.0f, 1.0f, 0.0f,
		  position, position, position, 1.0f,
		};

		//単位行列の代入
		worldTransform.matWorld_ = {
		  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		};

		////掛け算して代入
		worldTransform.matWorld_ *= matScale;
		//worldTransform.matWorld_ *= matRot;

	/*	worldTransform.matWorld_ *= matRotZ;
		worldTransform.matWorld_ *= matRotX;
		worldTransform.matWorld_ *= matRotY;*/

		worldTransform.matWorld_ *= matTrans;
		//行列の転送
		worldTransform.TransferMatrix();
	}
	
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の有効化
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	
}

void GameScene::Update() {
	//デバックカメラの更新
	debugCamera_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	////3Dモデルの描画
	// model_->Draw(worldTransform_,viewProjection_,textureHandle_);
	//モデルと連動させるカメラの描画
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, debugCamera_->GetViewProjection(), textureHandle_);
	}
	

	//ラインの描画
	//拡大された立方体の描画

	/*for (int i = 0; i < _countof(edgeList); i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  vertex[edgeList[i][0]], vertex[edgeList[i][1]], WHITE);
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  scaleVertex[edgeList[i][0]], scaleVertex[edgeList[i][1]], BLUE);
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  rotaXVertex[edgeList[i][0]], rotaXVertex[edgeList[i][1]], RED);
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  rotaYVertex[edgeList[i][0]], rotaYVertex[edgeList[i][1]], GREEN);
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  rotaZVertex[edgeList[i][0]], rotaZVertex[edgeList[i][1]], BLACK);
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  translationVertex[edgeList[i][0]], translationVertex[edgeList[i][1]], PURPLE);
	}*/



	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
