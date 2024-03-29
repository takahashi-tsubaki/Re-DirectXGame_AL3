#include "EnemyBullet.h"
void EnemyBullet::Init(Model* model, const Vector3& position, const Vector3& velocity) 
{
	// NULLポインタ
	assert(model);

	//
	model_ = model;
	//テクスチャハンドルの読み込み
	textureHandle_ = TextureManager::Load("EnemyBullet.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//引数で受けとった初期座標をセット
	worldTransform_.translation_ = {position.x, position.y, position.z};
	/*affinMat.translate = affin::generateTransMat(worldTransform_);*/

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void EnemyBullet::Update() 
{
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	//座標を移動させる
	worldTransform_.translation_ += velocity_;
	affinMat.translate = affin::generateTransMat(worldTransform_);
	affin::setTransformationWolrdMat(affinMat, worldTransform_);
	//行列の合成
	worldTransform_.TransferMatrix();
	
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition() 
{
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	//戻り値
	return worldPos;
}

void EnemyBullet::OnCollision() 
{ 
	isDead_ = true;
}
float EnemyBullet::GetRadius() 
{ 
	return radius;
}