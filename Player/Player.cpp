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
#pragma region ˆÚ“®ˆ—
	//ˆÚ“®ˆ—
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

#pragma region ‰ñ“]ˆ—
	Vector3 Rotation = {0, 0, 0};
	const float kCharaRotY = ConvertToRadians(1.0f);

	Matrix4 matRotY;

	//‰ñ“]ˆ—
	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y += kCharaRotY;
	}
	if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y -= kCharaRotY;
	}
	//(-)2ƒÎ‚ğ’´‚¦‚½‚ç‰ñ“]Šp‚ğ0‚É–ß‚·ˆ—
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

	//ŠÖ”‚Ì‡¬
	/*affin::setTransformationWolrdMat(affinMat, worldTransform_);*/

	//s—ñ‚Ì“]‘—
	worldTransform_.TransferMatrix();

	//UŒ‚ŠÖ”
	Attack();

	//‹…‚ÌXV
	for (std::unique_ptr<PlayerBullet>&bullet : bullets_) 
	{
		bullet->Update();
	}


	//ƒfƒoƒbƒNƒeƒLƒXƒg
	debugText_->SetPos(20, 100);
	debugText_->Printf(
	  "translation : (%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

	debugText_->SetPos(20, 120);
	debugText_->Printf(
	  "rotation : (%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
	  worldTransform_.rotation_.z);
}
//•`‰æˆ—
void Player::Draw(ViewProjection viewProjection) 
{
	//ƒvƒŒƒCƒ„[‚Ì•`‰æ
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//‹…‚Ì•`‰æ
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
		//‹…‚Ì¶¬
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

		//‹…‚Ì‰Šú‰»
		newBullet->Init(model_, worldTransform_.translation_);

		//‹…‚Ì“o˜^
		bullets_.push_back(std::move(newBullet));
	}
}