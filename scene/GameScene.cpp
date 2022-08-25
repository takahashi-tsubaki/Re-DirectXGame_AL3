﻿#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "fstream"
#include "math.h"
#include <cassert>
#include <random>

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
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	enemyHandle_ = TextureManager::Load("kuribo-.jpg");
	//2Dレティクル用テクスチャ
	TextureManager::Load("reticle.png");

	//レールカメラの生成
	railCamera_ = new RailCamera();
	railCamera_->Init(Vector3(0, 0, -50), Vector3(0, 0, 0));

	//自キャラの生成
	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

	skydome_ = new skydome();
	//天球の生成
	skydome_->Init(modelSkydome_);
	player_->SetParent(railCamera_->GetWorldPosition());
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//敵の情報の初期化
	LoadEnemyPopData();

	////軸方向表示の有効化
	// AxisIndicator::GetInstance()->SetVisible(true);
	////軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	// AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	////ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	// PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {

	//デスフラグが立った敵を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy_) { return enemy_->isDead(); });
	//
	railCamera_->Update();
	//自キャラの更新
	player_->Update(railCamera_->GetViewProjection());

	//更新コマンド
	UpdateEnemyPopCommands();
	//敵キャラの更新
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->SetGameScene(this);
		enemy->Update();
	}

	//敵弾の更新
	EnemyBulletUpdate();
	//天球の更新
	skydome_->Update();

	//デバックカメラの更新
	debugCamera_->Update();

	debugText_->SetPos(10, 30);
	debugText_->Printf("%d", enemys_.size());

	debugText_->SetPos(10, 50);
	debugText_->Printf("%d", waitTimer);

#ifdef DEBUG
	if (input_->TriggerKey(DIK_TAB)) {
		if (isDebugCameraActive == false) {
			isDebugCameraActive = true;
		} else {
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
	} else {
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
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		enemy->Draw(railCamera_->GetViewProjection());
	}

	//敵の弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(railCamera_->GetViewProjection());
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
	player_->DrawUI();

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
	float eRadius;
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		eRadius = enemy->GetRadius();
	}

	//自機弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets_ = player_->GetBullets();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets_ = GetBullets();

#pragma region 自キャラと敵の弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();
	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		posB = bullet->GetWorldPosition();
		float ebRadius = bullet->GetRadius();
		//座標Aと座標Bの距離を求める
		//弾と弾の交差判定
		if (
		  ((posB.x - posA.x) * (posB.x - posA.x)) + ((posB.y - posA.y) * (posB.y - posA.y)) +
		    ((posB.z - posA.z) * (posB.z - posA.z)) <=
		  ((pRadius + ebRadius) * (pRadius + ebRadius))) {
			//コールバックを呼び出すS
			player_->OnCollision();
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 敵キャラと自機の弾の当たり判定
	//敵キャラの座標
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		posA = enemy->GetWorldPosition();
		//敵キャラと自弾全ての当たり判定
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
			posB = bullet->GetWorldPosition();
			float pbRadius = bullet->GetRadius();
			//座標Aと座標Bの距離を求める
			//球と球
			if (
			  ((posB.x - posA.x) * (posB.x - posA.x)) + ((posB.y - posA.y) * (posB.y - posA.y)) +
			    ((posB.z - posA.z) * (posB.z - posA.z)) <=
			  ((eRadius + pbRadius) * (eRadius + pbRadius))) {
				//コールバックを呼び出す
				enemy->OnCollision();
				bullet->OnCollision();
			}
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
			  ((posB.x - posA.x) * (posB.x - posA.x)) + ((posB.y - posA.y) * (posB.y - posA.y)) +
			    ((posB.z - posA.z) * (posB.z - posA.z)) <=
			  ((ebRadius + pbRadius) * (ebRadius + pbRadius))) {
				//コールバックを呼び出す
				pbullet->OnCollision();
				ebullet->OnCollision();
			}
		}
	}

#pragma endregion
}
void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet) {
	bullets_.push_back(std::move(enemyBullet));
}
void GameScene::EnemyBulletUpdate() {
	//デスフラグが立った球を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->isDead(); });

	//球の更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

void GameScene::EnemyOcurrence(const Vector3& v) {
	//敵の生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
	//敵の初期化
	newEnemy->Init(model_, enemyHandle_, Vector3(v.x, v.y, v.z));
	newEnemy->SetPlayer(player_);

	//敵の登録
	enemys_.push_back(std::move(newEnemy));
}

void GameScene::LoadEnemyPopData() {
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/Pop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}
void GameScene::UpdateEnemyPopCommands() {
	if (isWait_) {
		waitTimer--;
		if (waitTimer <= 0) {
			//待機完了
			isWait_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行は飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			//敵を発生させる
			EnemyOcurrence(Vector3(x, y, z));
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWait_ = true;
			waitTimer = waitTime;

			//コマンドループを抜ける
			break;
		}
	}
}