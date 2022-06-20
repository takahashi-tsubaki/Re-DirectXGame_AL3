#include "Player.h"
Player::Player() {}
void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//インスタンスの生成
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// x,y,z軸周りの回転角を設定
	worldTransform_.rotation_ = {};
	// X,Y,Z軸周りの平行移動を設定
	worldTransform_.translation_ = {};

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

	//移動制限
	const float kMoveLimitX = 36.0f;
	const float kMoveLimitY = 19.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//行列の更新
	worldTransform_.TransferMatrix();

	//攻撃関数
	Attack();

	//球の更新
	if (bullet_) {
		bullet_->Update();
	}

	debugText_->SetPos(20, 100);
	debugText_->Printf(
	  "translation : (%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
	debugText_->SetPos(20, 120);
	debugText_->Printf(
	  "rotation : (%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	  worldTransform_.rotation_.z);
}
void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}

float Player::ConvertToRadians(float fDegrees) noexcept { return fDegrees * (PI / 180.0f); }
float Player::ConvertToDegrees(float fRadians) noexcept { return fRadians * (180.0f / PI); }

void Player::Attack() {
	if (input_->PushKey(DIK_SPACE)) {
		//球を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Init(model_, worldTransform_.translation_);

		//球を登録する
		bullet_ = newBullet;
	}
}