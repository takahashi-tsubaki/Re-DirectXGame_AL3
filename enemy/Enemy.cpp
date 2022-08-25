#include "enemy.h"
#include "GameScene.h"
#include "MathUtility.h"
#include "player/Player.h"
using namespace MathUtility;

void Enemy::Init(Model* model, uint32_t textureHandle, const Vector3& v) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	// input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	//引数で受けとった初期座標をセット
	worldTransform_.translation_ = {v.x, v.y, v.z};

	modelBullet_ = Model::CreateFromOBJ("bullet", true);
}

void Enemy::Update() {

	//単位行列を設定
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	////座標を移動させる
	// worldTransform_.translation_ -= velocity_;

	////敵が画面外に行った際に座標を初期値へ戻す処理
	// if (worldTransform_.translation_.z <= -50)
	//{
	//	worldTransform_.translation_.z = 50;
	// }

	switch (phase_) {
	case Phase::Approach: //接近フェーズ
	default:
		//移動
		Approach();
		break;

		// case Phase::Leave://離脱フェーズ
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

	////デバックテキスト
	// debugText_->SetPos(20, 140);
	// debugText_->Printf(
	//	"enemyTranslation : (%f,%f,%f)", worldTransform_.translation_.x,
	//	worldTransform_.translation_.y, worldTransform_.translation_.z);

	/*debugText_->SetPos(20, 200);
	debugText_->Printf(
	    "isDead : (%s)", isDead_);*/
}

void Enemy::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Approach() {
	worldTransform_.translation_ += approach_;
	//既定の位置に到達したら初期位置に戻す
	if (worldTransform_.translation_.z <= -40.0f) {
		worldTransform_.translation_.z = 50.0f;
	}

	////既定の位置に到達したら離脱
	// if (worldTransform_.translation_.z <= 0.0f)
	//{
	//	phase_ = Phase::Leave;
	// }
}

void Enemy::Leave() { worldTransform_.translation_ += leave_; }

void Enemy::Shot() {
	dalayTimer -= 0.1f;

	////球の速度
	const float kBulletSpeed = 0.1f;

	//-------自機狙い弾-------//

	assert(player_);

	//プレイヤーのワールド座標の取得
	Vector3 playerPosition;
	playerPosition = player_->GetWorldPosition();
	//敵のワールド座標の取得
	Vector3 enemyPosition;
	enemyPosition = affin::GetWorldPosition(worldTransform_.matWorld_);

	Vector3 distance(0, 0, 0);

	//差分ベクトルを求める
	distance = playerPosition - enemyPosition;

	//長さを求める
	Vector3Length(distance);
	//正規化
	Vector3Normalize(distance);
	//ベクトルの長さを,速さに合わせる
	distance *= kBulletSpeed; //これが速度になる

	//クールタイムが０になったとき
	if (dalayTimer <= 0) {
		//球の生成
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		//球の初期化
		newBullet->Init(modelBullet_, worldTransform_.translation_, distance);
		//球の登録
		/*bullets_.push_back(std::move(newBullet));*/
		gameScene_->AddEnemyBullet(newBullet);
		dalayTimer = 60.0f;
	}
}
Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れるための変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	//戻り値
	return worldPos;
}
void Enemy::OnCollision() { isDead_ = true; }
float Enemy::GetRadius() { return radius; }