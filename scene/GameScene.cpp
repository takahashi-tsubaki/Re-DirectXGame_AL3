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
	delete skydome_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	model_ = Model::Create();
	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome",true);
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	reticleHandle_ = TextureManager::Load("reticle.png");
	enemyHandle_ = TextureManager::Load("kuribo-.jpg");

	//レールカメラの生成
	railCamera_ = new RailCamera();
	railCamera_->Init(Vector3(0, 0, -50), Vector3(0, 0, 0));

	//自キャラの生成
	player_ = new Player();
	player_->Initialize(model_,textureHandle_);
	//敵キャラの生成
	enemy_ = new Enemy();
	enemy_->Init(model_, enemyHandle_);

	enemy_->SetPlayer(player_);

	skydome_ = new skydome();
	//天球の生成
	skydome_->Init(modelSkydome_);


	
	player_->SetParent(railCamera_->GetWorldPosition());
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	////軸方向表示の有効化
	//AxisIndicator::GetInstance()->SetVisible(true);
	////軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	////ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

}

void GameScene::Update() {

	//
	railCamera_->Update();
	//自キャラの更新
	player_->Update();

	//敵キャラの更新
	enemy_->Update();
	//天球の更新
	skydome_->Update();

	//デバックカメラの更新
	debugCamera_->Update();
	
	

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
	CheckAllCollision();
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
	
	/*model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);*/
	
	//天球の描画
	skydome_->Draw(railCamera_->GetViewProjection());
	//自キャラの描画
	player_->Draw(railCamera_->GetViewProjection());
	//敵キャラの描画
	enemy_->Draw(railCamera_->GetViewProjection());
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
void GameScene::CheckAllCollision() {
	//判定対象AとBの座標
	Vector3 posA, posB;
	float pRadius = player_->GetRadius();
	float eRadius = enemy_->GetRadius();
	//自機リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets_ = player_->GetBullets();
	//敵リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets_ = enemy_->GetBullets();

#pragma region 自キャラと敵の弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();
	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) 
	{
		posB = bullet->GetWorldPosition();
		float ebRadius=bullet->GetRadius();
		//座標Aと座標Bの距離を求める
		//弾と弾の交差判定
		if (
		  ((posB.x - posA.x) * (posB.x - posA.x)) + 
		  ((posB.y - posA.y) * (posB.y - posA.y)) +
		  ((posB.z - posA.z) * (posB.z - posA.z)) <=
		  ((pRadius + ebRadius) * (pRadius + ebRadius)))
		{
			//コールバックを呼び出すS
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
	
#pragma endregion

#pragma region 敵キャラと自機の弾の当たり判定
	//敵キャラの座標
	posA = enemy_->GetWorldPosition();
	//敵キャラと自弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		posB = bullet->GetWorldPosition();
		float pbRadius = bullet->GetRadius();
		//座標Aと座標Bの距離を求める
		//球と球
		if (
		  ((posB.x - posA.x) * (posB.x - posA.x)) + 
		  ((posB.y - posA.y) * (posB.y - posA.y)) +
		  ((posB.z - posA.z) * (posB.z - posA.z)) <=
		  ((eRadius + pbRadius) * (eRadius + pbRadius))) {
			//コールバックを呼び出す
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}


#pragma endregion

#pragma region 敵の弾と自機の弾の当たり判定
	//自弾の座標
	for (const std::unique_ptr<PlayerBullet>& pbullet : playerBullets_) {
		posA = pbullet->GetWorldPosition();
		float pbRadius = pbullet->GetRadius();
		//敵弾の座標
		for (const std::unique_ptr<EnemyBullet>& ebullet : enemyBullets_) {
			posB = ebullet->GetWorldPosition();
			float ebRadius = ebullet->GetRadius();
			//球と球
			if (
			  ((posB.x - posA.x) * (posB.x - posA.x)) +
			  ((posB.y - posA.y) * (posB.y - posA.y)) +
			  ((posB.z - posA.z) * (posB.z - posA.z)) <=
			  ((ebRadius + pbRadius) * (ebRadius + pbRadius))) 
			{
				//コールバックを呼び出す
				pbullet->OnCollision();
				ebullet->OnCollision();
			}
		}
	}
	
#pragma endregion

}