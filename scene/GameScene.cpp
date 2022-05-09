#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

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

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の有効化
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	for (int i = 0; i < 8; i++) {
		scaleVertex[i] = vertex[i];
		rotaXVertex[i] = vertex[i];
		rotaYVertex[i] = vertex[i];
		rotaZVertex[i] = vertex[i];
		translationVertex[i] = vertex[i];
	}

	//拡大
	for (int i = 0; i < 8; i++) {
		scaleVertex[i].x = afinScale[0][0] * vertex[i].x + afinScale[0][1] * vertex[i].y +
		                   afinScale[0][2] * vertex[i].z + afinScale[0][3] * 1;
		scaleVertex[i].y = afinScale[1][0] * vertex[i].x + afinScale[1][1] * vertex[i].y +
		                   afinScale[1][2] * vertex[i].z + afinScale[1][3] * 1;
		scaleVertex[i].z = afinScale[2][0] * vertex[i].x + afinScale[2][1] * vertex[i].y +
		                   afinScale[2][2] * vertex[i].z + afinScale[2][3] * 1;
	}
	//
	for (int i = 0; i < 8; i++) {
		rotaXVertex[i].x = afinRotationX[0][0] * vertex[i].x + afinRotationX[0][1] * vertex[i].y +
		                   afinRotationX[0][2] * vertex[i].z + afinRotationX[0][3] * 1;
		rotaXVertex[i].y = afinRotationX[1][0] * vertex[i].x + afinRotationX[1][1] * vertex[i].y +
		                   afinRotationX[1][2] * vertex[i].z + afinRotationX[1][3] * 1;
		rotaXVertex[i].z = afinRotationX[2][0] * vertex[i].x + afinRotationX[2][1] * vertex[i].y +
		                   afinRotationX[2][2] * vertex[i].z + afinRotationX[2][3] * 1;
	}

	for (int i = 0; i < 8; i++) {
		rotaYVertex[i].x = afinRotationY[0][0] * vertex[i].x + afinRotationY[0][1] * vertex[i].y +
		                   afinRotationY[0][2] * vertex[i].z + afinRotationY[0][3] * 1;
		rotaYVertex[i].y = afinRotationY[1][0] * vertex[i].x + afinRotationY[1][1] * vertex[i].y +
		                   afinRotationY[1][2] * vertex[i].z + afinRotationY[1][3] * 1;
		rotaYVertex[i].z = afinRotationY[2][0] * vertex[i].x + afinRotationY[2][1] * vertex[i].y +
		                   afinRotationY[2][2] * vertex[i].z + afinRotationY[2][3] * 1;
	}

	for (int i = 0; i < 8; i++) {
		rotaZVertex[i].x = afinRotationZ[0][0] * vertex[i].x + afinRotationZ[0][1] * vertex[i].y +
		                   afinRotationZ[0][2] * vertex[i].z + afinRotationZ[0][3] * 1;
		rotaZVertex[i].y = afinRotationZ[1][0] * vertex[i].x + afinRotationZ[1][1] * vertex[i].y +
		                   afinRotationZ[1][2] * vertex[i].z + afinRotationZ[1][3] * 1;
		rotaZVertex[i].z = afinRotationZ[2][0] * vertex[i].x + afinRotationZ[2][1] * vertex[i].y +
		                   afinRotationZ[2][2] * vertex[i].z + afinRotationZ[2][3] * 1;
	}

	for (int i = 0; i < 8; i++) {
		translationVertex[i].x = afinTranslation[0][0] * vertex[i].x +
		                         afinTranslation[0][1] * vertex[i].y +
		                         afinTranslation[0][2] * vertex[i].z + afinTranslation[0][3] * 1;
		translationVertex[i].y = afinTranslation[1][0] * vertex[i].x +
		                         afinTranslation[1][1] * vertex[i].y +
		                         afinTranslation[1][2] * vertex[i].z + afinTranslation[1][3] * 1;
		translationVertex[i].z = afinTranslation[2][0] * vertex[i].x +
		                         afinTranslation[2][1] * vertex[i].y +
		                         afinTranslation[2][2] * vertex[i].z + afinTranslation[2][3] * 1;
	}
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
	////モデルと連動させるカメラの描画
	// model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	//ラインの描画
	//拡大された立方体の描画

	for (int i = 0; i < _countof(edgeList); i++) {
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
		  translationVertex[edgeList[i][0]], translationVertex[edgeList[i][1]], PINK);
	}

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
