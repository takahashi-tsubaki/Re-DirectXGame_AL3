#include "Player.h"
Player::Player() {}
void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	
	worldTransform_.Initialize();
	
	worldTransform_.rotation_ = {};

	worldTransform_.translation_ = {};

	//
}


void Player::Update() {

	//デスフラグが立った球を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) 
	{ 
			return bullet->isDead();
	});

	Vector3 move = {0, 0, 0};

	const float kCharaSpeed = 0.2f;
	
	Matrix4 matTrans;

	worldTransform_.matWorld_ = {1, 0, 0, 0,  // x
	                             0, 1, 0, 0,  // y
	                             0, 0, 1, 0,  // z
	                             0, 0, 0, 1}; // Tx,Ty,Tz
#pragma region 移動処理
	//移動処理
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharaSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		move.x += kCharaSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += kCharaSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		move.y -= kCharaSpeed;
	
	} 

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	matTrans = affin::generateTransMat(worldTransform_);
#pragma endregion

#pragma region 回転処理
	Vector3 Rotation = {0, 0, 0};
	const float kCharaRotY = ConvertToRadians(0.5f);

	Matrix4 matRotY;

	//回転処理
	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y += kCharaRotY;
	}
	if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y -= kCharaRotY;
	}
	//(-)2πを超えたら回転角を0に戻す処理
	if (worldTransform_.rotation_.y >= (PI * 2) || worldTransform_.rotation_.y <= -(PI * 2)) {
		worldTransform_.rotation_.y = 0;
	}
	

	matRotY = affin::generateRotateYMat(worldTransform_);
#pragma endregion

	worldTransform_.matWorld_.operator*=(matRotY);

	worldTransform_.matWorld_.operator*=(matTrans);
	
	const float kMoveLimitX = 36.0f;
	const float kMoveLimitY = 19.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//関数の合成
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();

	//攻撃関数
	Attack();

	//球の更新
	for (std::unique_ptr<PlayerBullet>&bullet : bullets_) 
	{
		bullet->Update();
	}
	

	//デバックテキスト
	debugText_->SetPos(20, 100);
	debugText_->Printf(
	  "translation : (%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

	debugText_->SetPos(20, 120);
	debugText_->Printf(
	  "rotation : (%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	  worldTransform_.rotation_.z);
	debugText_->SetPos(20,80);
	debugText_->Printf("dalay : %f",dalayTimer );

}
//描画処理
void Player::Draw(ViewProjection viewProjection) 
{
	//プレイヤーの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//球の描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) 
	{
		bullet->Draw(viewProjection);
	}

}

float Player::ConvertToRadians(float fDegrees) noexcept { return fDegrees * (PI / 180.0f); }
float Player::ConvertToDegrees(float fRadians) noexcept { return fRadians * (180.0f / PI); }

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE))
	{

		dalayTimer-=0.1f;

		//球の速度
		const float kBulletSpeed = 0.5f;

		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = bVelocity(velocity, worldTransform_);

		//クールタイムが０になったとき
		if (dalayTimer <= 0) 
		{
			//球の生成
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//球の初期化
			newBullet->Init(model_, worldTransform_.translation_, velocity);

			//球の登録
			bullets_.push_back(std::move(newBullet));

			dalayTimer = 5.0f;
		}
		
	}
}

Vector3 Player::bVelocity(Vector3& velocity, WorldTransform& worldTransform) {
	
	Vector3 result = {0,0,0};

	//内積
	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
	           velocity.y * worldTransform.matWorld_.m[1][2] +
	           velocity.z * worldTransform.matWorld_.m[2][2];

	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
	           velocity.y * worldTransform.matWorld_.m[1][0] +
	           velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
	           velocity.y * worldTransform.matWorld_.m[1][1] +
	           velocity.z * worldTransform.matWorld_.m[2][1];

	return result;
}

Vector3 Player::GetWorldPosition()
{ 
	//ワールド座標を入れるための変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	//戻り値
	return worldPos;
}