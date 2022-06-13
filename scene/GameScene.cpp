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

	//ランダムシード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスター
	std::mt19937_64 engine(seed_gen());
	////乱数範囲の指定                              最小値   最大値
	//std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
	////乱数エンジンを渡し、指定範囲からランダムな数値を得る
	//float value = dist(engine);

	//乱数角用の乱数範囲の設定
	std::uniform_real_distribution<float> rot(0.0f, 6.28f);
	
	//座標用の乱数範囲の設定
	std::uniform_real_distribution<float> pos(-10.0f, 10.0f);

	Matrix4 matScale;

	Matrix4 matRot;

	Matrix4 matRotX, matRotY, matRotZ;

	Matrix4 matTrans = MathUtility::Matrix4Identity();

	for (WorldTransform& worldTransform : worldTransforms_) {
		
		//乱数エンジンを渡し、指定範囲からランダムな数値を得る
		float rotate = rot(engine);

		//乱数エンジンを渡し、指定範囲からランダムな数値を得る
		float positionX = pos(engine);
		float positionY = pos(engine);
		float positionZ = pos(engine);

		//ワールドトランスフォームの初期化
		worldTransform.Initialize();
		// X,Y,Z方向のスケーリングを設定
		worldTransform.scale_ = {1.0, 1.0, 1.0};
		//　X,Y,Z軸周りの回転角を設定
		worldTransform.rotation_ = {rotate, rotate, rotate};

		worldTransform.translation_ = {positionX, positionY, positionZ};

		//スケーリング倍率を行列に設定
		matScale = {
		  1.0f, 0.0f, 0.0f, 0.0f, 
		  0.0f, 1.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 1.0f, 0.0f, 
		  0.0f, 0.0f, 0.0f, 1.0f,
		};

		//回転行列の各要素を設定
		matRotZ = {
		  cos(rotate),  sin(rotate), 0.0f, 0.0f,
		  -sin(rotate), cos(rotate), 0.0f, 0.0f,
		  0.0f,         0.0f,        1.0f, 0.0f, 
		  0.0f,         0.0f,        0.0f, 1.0f,

		};
		matRotX = {
		  1.0f, 0.0f,  0.0f, 0.0f,
		  0.0f,cos(rotate), sin(rotate),  0.0f,
		  0.0f,-sin(rotate), cos(rotate), 0.0f,         
		  0.0f, 0.0f,0.0f, 1.0f,

		};
		matRotY = {
		  cos(rotate), 0.0f, -sin(rotate), 0.0f,
		  0.0f, 1.0f, 0.0f, 0.0f,
		  sin(rotate),0.0f,cos(rotate), 0.0f, 
		  0.0f,0.0f, 0.0f, 1.0f,

		};


		//移動量を行列に設定
		matTrans = {
		  1.0f, 0.0f, 0.0f, 0.0f,
		  0.0f,  1.0f,0.0f, 0.0f,
		  0.0f, 0.0f, 1.0f, 0.0f,
		  positionX, positionY, positionZ, 1.0f,
		};

		//単位行列の代入
		worldTransform.matWorld_ = {
		  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		};

		////掛け算して代入
		worldTransform.matWorld_ *= matScale;
		//worldTransform.matWorld_ *= matRot;
		worldTransform.matWorld_ *= matRotZ;
		worldTransform.matWorld_ *= matRotX;
		worldTransform.matWorld_ *= matRotY;
		worldTransform.matWorld_ *= matTrans;
		//行列の転送
		worldTransform.TransferMatrix();
	}


	/*viewProjection_.target = {10.0f, 0.0f, 0.0f};
	viewProjection_.up = {cosf(PI / 4.0f), sinf(PI/4.0f),0.0f};*/
	//カメラ垂直方向視野角を設定
	
	viewProjection_.fovAngleY = ConvertToRadians(10.0f);

	//アスペクト比を設定
	viewProjection_.aspectRatio = 1.0f;

	//ニアクリップ距離を設定
	viewProjection_.nearZ = 52.0f;

	//ファークリップ距離を設定
	viewProjection_.farZ = 53.0f;
	
	
	
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

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
	//Vector3 move = {0, 0, 0};

	//const float kEyeSpeed = 0.2f;

	//if (input_->PushKey(DIK_W)) {
	//	move.z += kEyeSpeed;
	//}
	//if (input_->PushKey(DIK_S)) {
	//	move.z -= kEyeSpeed;
	//}
	//viewProjection_.eye += move;

	//viewProjection_.UpdateMatrix();

	////注視点ベクトル

	//const float kTargetSpeed = 0.2f;

	//if (input_->PushKey(DIK_A)) {
	//	move.x -= kTargetSpeed;
	//}
	//if (input_->PushKey(DIK_D)) {
	//	move.x += kTargetSpeed;
	//}
	//viewProjection_.target += move;

	//viewProjection_.UpdateMatrix();

	////
	//const float kUpRotSpeed = 0.05f;
	//if (input_->PushKey(DIK_SPACE)) {
	//	viewAngle += kUpRotSpeed;
	//	//2π
	//	viewAngle = fmodf(viewAngle,PI *2.0f);
	//}
	//viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};

	//上キーで視野角が広がる
	if (input_->PushKey(DIK_UP)) 
	{
		viewProjection_.fovAngleY += 0.01f;
		viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, PI);
	}
	else if (input_->PushKey(DIK_DOWN)) 
	{
		viewProjection_.fovAngleY -= 0.01f;
		viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
	}

	debugText_->SetPos(50,70);
	debugText_->Printf("fovAngleY(Degree) : %f", ConvertToRadians(viewProjection_.fovAngleY));
	//行列の再計算
	viewProjection_.UpdateMatrix();

	//上下キーでニアクリップ距離を増減
	if (input_->PushKey(DIK_W)) {
		viewProjection_.nearZ += 0.1f;
	} 
	else if (input_->PushKey(DIK_S)) {
		viewProjection_.nearZ -= 0.1f;
	}

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f", viewProjection_.nearZ);



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
	/*for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, debugCamera_->GetViewProjection(), textureHandle_);
	}*/
	//for (int i = 0; i < 100; i++)
	//{
	//	model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	//}
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
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
//void GameScene::Move(ViewProjection viewProjection) 
//{
//	//視点ベクトル
//	Vector3 move = {0,0,0};
//
//	const float kEyeSpeed = 0.2f;
//
//	if (input_->PushKey(DIK_W)) {
//		move.z += kEyeSpeed;
//	}
//	if (input_->PushKey(DIK_S)) {
//		move.z -= kEyeSpeed;
//	}
//	viewProjection.eye += move;
//
//	viewProjection.UpdateMatrix();
//}
float GameScene::ConvertToRadians(float fDegrees) noexcept {
	return fDegrees * (PI / 180.0f);
}
float GameScene::ConvertToDegrees(float fRadians) noexcept {
	return fRadians * (180.0f / PI);
}