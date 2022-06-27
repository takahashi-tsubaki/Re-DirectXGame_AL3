#include "enemy.h"
void Enemy::Init(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	//引数で受けとった初期座標をセット
	worldTransform_.translation_ = {0,3,50};
	
	//メンバ関数ポインタに関数のアドレスを代入する
	void (Enemy::*Enemy::spFuncTable[])() = {
	  &Enemy::Approach, //要素番号0
	  &Enemy::Leave,    //要素番号1
	};
	
}

void Enemy::Update() 
{
	//単位行列を設定
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	////座標を移動させる
	//worldTransform_.translation_ -= velocity_;
	
	////敵が画面外に行った際に座標を初期値へ戻す処理
	//if (worldTransform_.translation_.z <= -50)
	//{
	//	worldTransform_.translation_.z = 50;
	//}

	//switch (phase_) {
	//case Phase::Approach://接近フェーズ
	//default:
	//	//移動
	//	Approach();
	//	break;

	//case Phase::Leave://離脱フェーズ
	//	Leave();
	//	break;
	//}
	
	//メンバ関数ポインタに入っている関数を呼び出す
	(this->*spFuncTable[0])();

	//行列の計算
	affinMat.translate = affin::generateTransMat(worldTransform_);
	//行列の合成
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
	//デバックテキスト
	debugText_->SetPos(20, 140);
	debugText_->Printf(
	  "enemyTranslation : (%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

void Enemy::Draw(ViewProjection viewProjection) 
{ 
	model_->Draw(worldTransform_,viewProjection,textureHandle_); 
}

void Enemy::Approach()
{
	worldTransform_.translation_ += approach_; 
	//既定の位置に到達したら離脱
	if (worldTransform_.translation_.z <= 0.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave()
{ 
	worldTransform_.translation_ += leave_; 
}