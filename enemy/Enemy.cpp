#include "enemy.h"
void Enemy::Init(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	//引数で受けとった初期座標をセット
	worldTransform_.translation_ = {15,0,50};
	
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

	switch (phase_) {
	case Phase::Approach://接近フェーズ
	default:
		//移動
		Approach();
		break;

	//case Phase::Leave://離脱フェーズ
	//	Leave();
	//	break;
	}

	//行列の計算
	affinMat.translate = affin::generateTransMat(worldTransform_);
	//行列の合成
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();

	//球の発射する関数
	Shot();

	//球の更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//デバックテキスト
	debugText_->SetPos(20, 140);
	debugText_->Printf(
	  "enemyTranslation : (%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

void Enemy::Draw(ViewProjection viewProjection) 
{ 
	model_->Draw(worldTransform_,viewProjection,textureHandle_); 
	//球の描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Approach()
{
	worldTransform_.translation_ += approach_; 
	//既定の位置に到達したら初期位置に戻す
	if (worldTransform_.translation_.z <= -40.0f)
	{
		worldTransform_.translation_.z = 50.0f;
	}
	////既定の位置に到達したら離脱
	//if (worldTransform_.translation_.z <= 0.0f)
	//{
	//	phase_ = Phase::Leave;
	//}
}

void Enemy::Leave()
{ 
	worldTransform_.translation_ += leave_; 
}

void Enemy::Shot()
{
	dalayTimer -= 0.1f;

	//球の速度
	const float kBulletSpeed = 0.5f;

	Vector3 velocity(0, 0, kBulletSpeed);

	//クールタイムが０になったとき
	if (dalayTimer <= 0) {
		//球の生成
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		//球の初期化
		newBullet->Init(model_, worldTransform_.translation_, velocity);

		//球の登録
		bullets_.push_back(std::move(newBullet));

		dalayTimer = 20.0f;
	}
}