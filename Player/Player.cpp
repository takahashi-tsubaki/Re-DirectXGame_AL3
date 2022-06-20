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

	Vector3 move = {0, 0, 0};

	const float kCharaSpeed = 0.2f;

	Matrix4 matTrans;

	worldTransform_.matWorld_ = {1, 0, 0, 0,  // x
	                             0, 1, 0, 0,  // y
	                             0, 0, 1, 0,  // z
	                             0, 0, 0, 1}; // Tx,Ty,Tz

	if (input_->PushKey(DIK_LEFT)) {

		move.x -= kCharaSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharaSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharaSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharaSpeed;
	
	} 

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	matTrans = affin::generateTransMat(worldTransform_);

	Vector3 Rotation = {0, 0, 0};
	const float kCharaRotY = ConvertToRadians(1.0f);

	Matrix4 matRotY;

	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y += kCharaRotY;
	}
	if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y -= kCharaRotY;
	}

	matRotY = affin::generateRotateYMat(worldTransform_);

	worldTransform_.matWorld_.operator*=(matTrans);

	worldTransform_.matWorld_.operator*=(matRotY);

	
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
		//球の生成
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

		//球の初期化
		newBullet->Init(model_, worldTransform_.translation_);

		//球の登録
		bullets_.push_back(std::move(newBullet));
	}
}