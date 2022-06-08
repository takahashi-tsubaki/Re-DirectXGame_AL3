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

	//アフィン変換関数
	changeAffin();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示の有効化
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
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
	
	//モデルと連動させるカメラの描画
	 model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

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
void GameScene::changeAffin()
{
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = {5.0, 2.0, 5.0};
	//　X,Y,Z軸周りの回転角を設定
	worldTransform_.rotation_ = {0.0f, 45.0f, 0.0f};

	worldTransform_.translation_ = {10, 10, 10};

	Matrix4 matScale;

	Matrix4 matRot,matRotX,matRotY,matRotZ;

	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//スケーリング倍率を行列に設定
	matScale = {
	  5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f,
	  0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	//回転行列の各要素を設定
	matRotZ = {
	  cos(PI / 4), sin(PI / 4), 0.0f, 0.0f,
	  -sin(PI / 4), cos(PI / 4), 0.0f, 0.0f,
	  0.0f, 0.0f, 1.0f, 0.0f,
	  0.0f, 0.0f, 0.0f, 1.0f,
	};

	matRotX = {
	  1.0f, 0.0f, 0.0f, 0.0f,
	  0.0f, cos(PI / 4), sin(PI / 4), 0.0f,
	  0.0f, -sin(PI / 4), cos(PI / 4), 0.0f,
	  0.0f, 0.0f,0.0f, 1.0f,
	};

	matRotY = {
	  cos(PI / 4), 0.0f, -sin(PI / 4), 0.0f,
	  0.0f, 1.0f, 0.0,  0.0f,
	  sin(PI / 4), 0.0f, cos(PI / 4), 0.0f, 
	  0.0f, 0.0f, 0.0f, 1.0f,
	};

	//移動量を行列に設定
	matTrans = {
	  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	  0.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f, 10.0f, 1.0f,
	};

	//単位行列の代入
	worldTransform_.matWorld_ = {
	  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	//掛け算して代入
	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRotZ;
	worldTransform_.matWorld_ *= matRotX;
	worldTransform_.matWorld_ *= matRotY;
	worldTransform_.matWorld_ *= matTrans;
	//行列の転送
	worldTransform_.TransferMatrix();
}