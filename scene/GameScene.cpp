#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() 
{
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
	debugCamera_ = new DebugCamera(1280,720);

	//軸方向表示の有効化
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	
	for (int i = 0; i < 8; i++) {
		scaleVertex[i] = vertex[i];
	}
	

	for (int i = 0; i < 8; i++) {
		scaleVertex[i].x *= afinScale[0][0];
		scaleVertex[i].y *= afinScale[1][1];
		scaleVertex[i].z *= afinScale[2][2];
	};

}

void GameScene::Update() 
{ 
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
	//model_->Draw(worldTransform_,viewProjection_,textureHandle_);
	////モデルと連動させるカメラの描画
	//model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	
	//ラインの描画
	//拡大された立方体の描画
	for (int i = 0; i < _countof(edgeList); i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		  scaleVertex[edgeList[i][0]], scaleVertex[edgeList[i][1]], BLUE);
	}

	for (int i = 0; i < _countof(edgeList); i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(vertex[edgeList[i][0]], vertex[edgeList[i][1]], WHITE);
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
