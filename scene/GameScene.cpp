#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include "math.h"

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
	worldTransforms_[partId::kRoot].Initialize();

	worldTransforms_[partId::kSpine].Initialize();
	worldTransforms_[partId::kSpine].parent_ = &worldTransforms_[partId::kRoot];
	worldTransforms_[partId::kSpine].translation_ = {0.0f, -3.0f, 0.0f};

	worldTransforms_[partId::kChest].Initialize();
	worldTransforms_[partId::kChest].parent_ = &worldTransforms_[partId::kSpine];
	worldTransforms_[partId::kChest].translation_ = {0.0f, 3.0f, 0.0f};

	worldTransforms_[partId::kHead].Initialize();
	worldTransforms_[partId::kHead].parent_ = &worldTransforms_[partId::kChest];
	worldTransforms_[partId::kHead].translation_ = {0.0f, 3.0f, 0.0f};

	worldTransforms_[partId::kArmL].Initialize();
	worldTransforms_[partId::kArmL].parent_ = &worldTransforms_[partId::kChest];
	worldTransforms_[partId::kArmL].translation_ = {4.5f, 0.0f, 0.0f};
	
	worldTransforms_[partId::kArmR].Initialize();
	worldTransforms_[partId::kArmR].parent_ = &worldTransforms_[partId::kChest];
	worldTransforms_[partId::kArmR].translation_ = {-4.5f, 0.0f, 0.0f};

	worldTransforms_[partId::kHip].Initialize();
	worldTransforms_[partId::kHip].parent_ = &worldTransforms_[partId::kSpine];
	worldTransforms_[partId::kHip].translation_ = {0.0f, 0.0f, 0.0f};

	worldTransforms_[partId::kLegL].Initialize();
	worldTransforms_[partId::kLegL].parent_ = &worldTransforms_[partId::kHip];
	worldTransforms_[partId::kLegL].translation_ = {-3.0f, -3.0f, 0.0f};

	worldTransforms_[partId::kLegR].Initialize();
	worldTransforms_[partId::kLegR].parent_ = &worldTransforms_[partId::kHip];
	worldTransforms_[partId::kLegR].translation_ = {3.0f, -3.0f, 0.0f};

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示の有効化
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	
}

void GameScene::Update() {
	//デバックカメラの更新
	/*debugCamera_->Update();*/

	/*Move(viewProjection_);*/
	//視点ベクトル
	Vector3 move = {0, 0, 0};

	const float kEyeSpeed = 0.2f;

	const float kRotY = ConvertToRadians(359.0f);

	if (input_->PushKey(DIK_W)) {
		move.y += kEyeSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		move.y -= kEyeSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		move.x += kEyeSpeed;
	}
	if (input_->PushKey(DIK_A)) {
		move.x -= kEyeSpeed;
	}
	worldTransforms_[partId::kRoot].translation_ += move; // 0

	if (input_->PushKey(DIK_U)) {
		worldTransforms_[partId::kChest].rotation_.y += kRotY;
	}
	if (input_->PushKey(DIK_I)) {
		worldTransforms_[partId::kChest].rotation_.y -= kRotY;
	}
	if (input_->PushKey(DIK_J)) {
		worldTransforms_[partId::kHip].rotation_.y += kRotY;
	}
	if (input_->PushKey(DIK_K)) {
		worldTransforms_[partId::kHip].rotation_.y -= kRotY;
	}
	Matrix4 matTrans[9];
	Matrix4 matRotY[9];
	//移動量を行列に設定
	for (int i = 0; i < 9; i++)
	{
		
		matTrans[i] = {
		  1.0f,0.0f,0.0f,0.0f,
		  0.0f,1.0f,0.0f,0.0f,
		  0.0f,0.0f,1.0f,0.0f,
		  worldTransforms_[i].translation_.x,
		  worldTransforms_[i].translation_.y,
		  worldTransforms_[i].translation_.z,
		  1.0f
		};
		matRotY[i] = {
		  cos(worldTransforms_[i].rotation_.y), 0.0f, -sin(worldTransforms_[i].rotation_.y),0.0f,
		  0.0f, 1.0f, 0.0f,0.0f,
		  sin(worldTransforms_[i].rotation_.y), 0.0f, cos(worldTransforms_[i].rotation_.y),0.0f,
		  0.0f, 0.0f, 0.0f, 1.0f,
		};
		worldTransforms_[i].matWorld_ =
		{
			1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		};
		worldTransforms_[i].matWorld_ *= matTrans[i];
		worldTransforms_[i].matWorld_ *= matRotY[i];

	}


	//子の座標を親の座標
	worldTransforms_[partId::kSpine].matWorld_ *= worldTransforms_[partId::kRoot].matWorld_;
	worldTransforms_[partId::kChest].matWorld_ *= worldTransforms_[partId::kSpine].matWorld_;
	worldTransforms_[partId::kHead].matWorld_ *= worldTransforms_[partId::kChest].matWorld_;
	worldTransforms_[partId::kArmL].matWorld_ *= worldTransforms_[partId::kChest].matWorld_;
	worldTransforms_[partId::kArmR].matWorld_ *= worldTransforms_[partId::kChest].matWorld_;
	worldTransforms_[partId::kHip].matWorld_ *= worldTransforms_[partId::kSpine].matWorld_;
	worldTransforms_[partId::kLegL].matWorld_ *= worldTransforms_[partId::kHip].matWorld_;
	worldTransforms_[partId::kLegR].matWorld_ *= worldTransforms_[partId::kHip].matWorld_;


	//行列の転送
	worldTransforms_[partId::kRoot].TransferMatrix();
	worldTransforms_[partId::kSpine].TransferMatrix();
	worldTransforms_[partId::kChest].TransferMatrix();
	worldTransforms_[partId::kHead].TransferMatrix();
	worldTransforms_[partId::kArmL].TransferMatrix();
	worldTransforms_[partId::kArmR].TransferMatrix();
	worldTransforms_[partId::kHip].TransferMatrix();
	worldTransforms_[partId::kLegL].TransferMatrix();
	worldTransforms_[partId::kLegR].TransferMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "translation:%f,%f,%f", worldTransforms_[kChest].translation_.x,
	  worldTransforms_[kChest].translation_.y, worldTransforms_[kChest].translation_.z);

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
	
	/*model_->Draw(worldTransforms_[partId::kRoot], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[partId::kSpine], viewProjection_, textureHandle_);*/
	model_->Draw(worldTransforms_[partId::kChest], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[partId::kHead], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[partId::kArmL], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[partId::kArmR], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[partId::kHip], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[partId::kLegL], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[partId::kLegR], viewProjection_, textureHandle_);

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

float GameScene::ConvertToRadians(float fDegrees) noexcept {
	return fDegrees * (PI / 180.0f);
}
float GameScene::ConvertToDegrees(float fRadians) noexcept {
	return fRadians * (180.0f / PI);
}
//void GameScene::UpdateMatrix(WorldTransform worldTransform_[8], int num,Vector3 move) { 
//	Matrix4 matTrans[8];
//	for (int i = 0;i<num;i++) {
//
//		matTrans[i] = {
//		  1.0f, 0.0f, 0.0f, 0.0f,
//		  0.0f, 1.0f, 0.0f, 0.0f,
//		  0.0f, 0.0f, 1.0f, 0.0f,
//		  worldTransform_[i].translation_.x,
//		  worldTransform_[i].translation_.y,
//		  worldTransform_[i].translation_.z,
//		  1.0f
//		};
//
//		worldTransform_[i].matWorld_ = {
//		  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//		  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
//		};
//		if (Discrimination(num)) {
//			worldTransforms_[i].translation_ += move;
//			worldTransform_[i].matWorld_ *= matTrans[i];
//		} 
//		else 
//		{
//			if (num ==3) 
//			{
//				worldTransform_[num].matWorld_ *= worldTransform_[2].matWorld_;
//			} 
//			else if (num ==4 ||num==5) {
//			
//			}
//		}
//	}
//	
//	
//	
//	
//}
//int GameScene::Discrimination(int num) 
//{ 
//
//	while (true) 
//	{
//		if (num == 1 ||num ==8)
//		{
//			return true;
//			break;
//		} 
//		else if (num %2 == 1) {
//			return false;
//			break;
//		} 
//		else {
//			num /= 2;
//		}
//	}
//}