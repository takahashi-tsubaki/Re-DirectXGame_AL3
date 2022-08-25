#include "Player.h"
using namespace MathUtility;
Player::Player() {}
void Player::Initialize(Model * model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	winApp_ = WinApp::GetInstance();
	
	worldTransform_.Initialize();
	
	worldTransform_.rotation_ = {};

	worldTransform_.translation_ = Vector3{0,0,30};

	//3Dレティクルのワールドトランスフォームの初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャを取得
	uint32_t textureReticle = TextureManager::Load("reticle.png");

	//スプライト生成
	sprite2DReticle_.reset(Sprite::Create(
	  textureReticle, Vector2(640,320), RED,
	  Vector2(0.5f, 0.5f)));

}


void Player::Update(const ViewProjection& viewProjection) {

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

	//行列の合成
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	////親子のワールド座標の設定

	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	//行列の転送
	worldTransform_.TransferMatrix();

	//攻撃関数
	Attack();

	//球の更新
	for (std::unique_ptr<PlayerBullet>&bullet : bullets_) 
	{
		bullet->Update();
	}
	
	

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	
	//自機から3Dレティクルの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	//自機から3Dレティクルへのオフセット
	Vector3 offSet = {0, 0, 1.0f};
	//自機のワールド座標の回転を反映
	offSet = affin::matVector(offSet,worldTransform_.matWorld_);
	//ベクトルの長さを整える
	offSet = affin::Vector3Normalize(offSet)*kDistancePlayerTo3DReticle;
	//3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ =
	  affin::addVector3(affin::GetWorldPosition(worldTransform_.matWorld_) ,offSet );
	////行列の更新と転送
	//affin::generateTransMat(worldTransform3DReticle_);
	AffinTrans::affin(worldTransform3DReticle_);
	affin::setTransformationWolrdMat(affinMat, worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();
	
	// 3Dレティクルのワールド行列から,ワールド座標を取得
	Vector3 positionReticle = affin::GetWorldPosition(worldTransform3DReticle_.matWorld_);
	//ビューポート行列
	Matrix4 matViewport = affin::setViewportMat(worldTransform3DReticle_, winApp_,Vector3(0,0,0));
	
	//ビュー行列とプロジェクション行列,ビューポート行列を合成する
	Matrix4 matViewprojectionViewport =
	  viewProjection.matView * viewProjection.matProjection * matViewport;
	//ワールド→スクリーン座標変換
	positionReticle = affin::division(positionReticle, matViewprojectionViewport);
	//座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
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

	//デバックテキスト
	debugText_->SetPos(20, 140);
	debugText_->Printf(
	  "translation : (%f,%f,%f)", worldTransform3DReticle_.translation_.x,
	  worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	//デバックテキスト
	debugText_->SetPos(20, 260);
	debugText_->Printf(
	  "position : (%f,%f,%f)", positionReticle.x, positionReticle.y, positionReticle.z);

}
//描画処理
void Player::Draw(ViewProjection viewProjection) 
{

	//プレイヤーの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//3Dレティクルの描画
	model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);

	//球の描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) 
	{
		bullet->Draw(viewProjection);
	}

}
void Player::DrawUI()
{ 
	sprite2DReticle_->Draw(); 
}
float Player::ConvertToRadians(float fDegrees) noexcept { return fDegrees * (PI / 180.0f); }
float Player::ConvertToDegrees(float fRadians) noexcept { return fRadians * (180.0f / PI); }

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE))
	{

		dalayTimer-=0.1f;

		//自キャラの座標をコピー
		Vector3 position = affin::GetWorldPosition(worldTransform_.matWorld_);

		//球の速度
		const float kBulletSpeed = 0.5f;

		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = bVelocity(velocity, worldTransform_);

		//自機から標準オブジェクトへのベクトル
		velocity = affin::GetWorldPosition(worldTransform3DReticle_.matWorld_) -
		           affin::GetWorldPosition(worldTransform_.matWorld_);

		velocity = affin::Vector3Normalize(velocity) * kBulletSpeed;

		//クールタイムが０になったとき
		if (dalayTimer <= 0) 
		{
			//球の生成
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//球の初期化
			newBullet->Init(model_, position, velocity);

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
void Player::OnCollision() 
{

}

float Player::GetRadius() { return radius; }

void Player::SetParent(WorldTransform* worldTransform) 
{ 
	worldTransform_.parent_ = worldTransform;
}
