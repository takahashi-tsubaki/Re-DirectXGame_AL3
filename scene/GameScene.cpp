#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include "math.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	//解放
	delete model_;
	delete debugCamera_;
	delete player_;
	delete enemy_;
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
	enemyHandle_ = TextureManager::Load("kuribo-.jpg");

	
	//自キャラの生成
	player_ = new Player();
	player_->Initialize(model_,textureHandle_);
	//敵キャラの生成
	enemy_ = new Enemy();
	enemy_->Init(model_, enemyHandle_);

	enemy_->SetPlayer(player_);
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();


	////デバックカメラの生成
	//debugCamera_ = new DebugCamera(1280, 720);

	////軸方向表示の有効化
	//AxisIndicator::GetInstance()->SetVisible(true);
	////軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	////ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

}

void GameScene::Update() {

	//自キャラの更新
	player_->Update();
	//敵キャラの更新
	enemy_->Update();
	//デバックカメラの更新
	/*debugCamera_->Update();*/

#ifdef DEBUG
	if (input_->TriggerKey(DIK_TAB)) 
	{
		if (isDebugCameraActive==false) {
			isDebugCameraActive = true;
		} 
		else {
			isDebugCameraActive = false;
		}
	}
#endif // DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		debugViewProjection_ = debugCamera_->GetViewProjection();
		viewProjection_.matView = debugViewProjection_.matView;
		viewProjection_.matProjection = debugViewProjection_.matProjection;
		viewProjection_.UpdateMatrix();
	} 
	else {
		viewProjection_.UpdateMatrix();
	}

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
	
	//model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	
	//自キャラの描画
	player_->Draw(viewProjection_);
	//敵キャラの描画
	enemy_->Draw(viewProjection_);
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
