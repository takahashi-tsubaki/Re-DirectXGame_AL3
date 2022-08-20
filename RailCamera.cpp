#include "RailCamera.h"
using namespace MathUtility;
void RailCamera::Init(const Vector3& position, const Vector3& rotation) { 

	input_ = Input::GetInstance();
	//シングルインスタンスを取得する
	debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 300.0f;
	viewProjection_.Initialize();
	
}

void RailCamera::Update() 
{ 
	Vector3 move = {0, 0, 0};

	const float kCharaSpeed = 0.2f;

	Matrix4 matTrans;

	worldTransform_.matWorld_ = {1, 0, 0, 0,  // x
	                             0, 1, 0, 0,  // y
	                             0, 0, 1, 0,  // z
	                             0, 0, 0, 1}; // Tx,Ty,Tz
#pragma region 移動処理
	if (input_->PushKey(DIK_UP)) {
		move.z+= kCharaSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.z -= kCharaSpeed;
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	matTrans = affin::generateTransMat(worldTransform_);
#pragma endregion

#pragma region 回転処理
	Vector3 Rotation = {0, 0, 0};
	const float kCharaRotX = ConvertToRadians(0.5f);

	Matrix4 matRotX;

	//回転処理
	if (input_->PushKey(DIK_O)) {
		worldTransform_.rotation_.x += kCharaRotX;
	}
	if (input_->PushKey(DIK_P)) {
		worldTransform_.rotation_.x -= kCharaRotX;
	}
	//(-)2πを超えたら回転角を0に戻す処理
	if (worldTransform_.rotation_.x >= (PI * 2) || worldTransform_.rotation_.x <= -(PI * 2)) {
		worldTransform_.rotation_.x = 0;
	}

	matRotX = affin::generateRotateXMat(worldTransform_);
#pragma endregion

	worldTransform_.matWorld_.operator*=(matRotX);

	worldTransform_.matWorld_.operator*=(matTrans);

	//行列の転送
	worldTransform_.TransferMatrix();

	//
	viewProjection_.eye = worldTransform_.translation_;
	//前方ベクトル
	Vector3 forward(0,0,1);
	//レールカメラの回転を反映
	forward = affin::matVector(forward,worldTransform_.matWorld_);
	//視点から前方に適当に進んだ位置が注視点
	viewProjection_.target = viewProjection_.eye + forward;

	//上方ベクトル
	Vector3 up(0, 1, 0);
	//
	viewProjection_.up = affin::matVector(up, worldTransform_.matWorld_);

	//ビュープロジェクションの更新と行列バッファへの転送
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	//eyeの表示
	debugText_->SetPos(20, 160);
	debugText_->Printf(
	  "rail : (%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y,
	  viewProjection_.eye.z);

}
float RailCamera::ConvertToRadians(float fDegrees) noexcept { return fDegrees * (PI / 180.0f); }
float RailCamera::ConvertToDegrees(float fRadians) noexcept { return fRadians * (180.0f / PI); }

WorldTransform* RailCamera::GetWorldPosition() {
	//ワールド座標を入れるための変数
	WorldTransform worldPos;

	//戻り値
	return &worldPos;
}