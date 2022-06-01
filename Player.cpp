#include "Player.h"
Player::Player() 
{

}
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

	Matrix4 matTrans = MathUtility::Matrix4Identity();
	
	worldTransform_.matWorld_ = {1, 0, 0, 0,  // x
	                             0, 1, 0, 0,  // y
	                             0, 0, 1, 0,  // z
	                             0, 0, 0, 1}; // Tx,Ty,Tz

	if (input_->PushKey(DIK_LEFT)) 
	{
		move = {-kCharaSpeed, 0, 0};
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move = {kCharaSpeed, 0, 0};
	} 
	else if (input_->PushKey(DIK_UP)) {
		move = {0, kCharaSpeed, 0};
	} 
	else if (input_->PushKey(DIK_DOWN)) {
		move = {0, -kCharaSpeed, 0};
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;

	
	worldTransform_.matWorld_.operator*=(matTrans);
	
	//s—ñ‚ÌXV
	worldTransform_.TransferMatrix();

	debugText_->SetPos(20,100);
	debugText_->Printf(
	  "translation : (%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);

}

//float Player::Translate() 
//{ 
//	Matrix4 matTrans = MathUtility::Matrix4Identity();
//	
//	matTrans = {1, 0, 0, 0, //x
//				0, 1, 0, 0, //y
//				0, 0, 1, 0, //z
//				0.5f, 0.5f, 0.5f, 1};//Tx,Ty,Tz
//
//	worldTransform_.matWorld_ = {1,    0,    0,    0,  // x
//	                             0,    1,    0,    0,  // y
//	                             0,    0,    1,    0,  // z
//	                             0,    0,    0,    1}; // Tx,Ty,Tz
//	worldTransform_.matWorld_.operator*=(matTrans);
//
//}

void Player::Draw(ViewProjection viewProjection) 
{ 
	model_->Draw(worldTransform_,viewProjection,textureHandle_); 
}