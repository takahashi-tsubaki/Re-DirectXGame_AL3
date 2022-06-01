#include "Player.h"
void Player::Initialize(Model*model,uint32_t textureHandle) 
{ 
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update() 
{ 
	Vector3 move = {0, 0, 0};

	const float kCharaSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) 
	{
		move = {-kCharaSpeed, 0, 0};
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move = {kCharaSpeed, 0, 0};
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	
	//s—ñ‚ÌXV
	worldTransform_.TransferMatrix();

	debugText_->SetPos(20,100);
	debugText_->Printf(
	  "translation : (%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

}

void Player::Draw(ViewProjection viewProjection_) 
{ 
	model_->Draw(worldTransform_,viewProjection_,textureHandle_); 
}