#include "PlayerBullet.h"
//初期化処理
void PlayerBullet::Init(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタ
	assert(model);

	//
	model_ = model;
	//テクスチャハンドルの読み込み
	textureHandle_ = TextureManager::Load("bullet.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//引数で受けとった初期座標をセット
	worldTransform_.translation_ = {position.x, position.y, position.z};
	/*affinMat.translate = affin::generateTransMat(worldTransform_);*/

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

}
//更新処理
void PlayerBullet::Update() 
{

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	//座標を移動させる
	worldTransform_.translation_ += velocity_;
	affinMat.translate = affin::generateTransMat(worldTransform_);
	affin::setTransformationWolrdMat(affinMat, worldTransform_);
	//
	worldTransform_.TransferMatrix();
	//時間経過で消滅
	if (--deathTimer_ <= 0) 
	{
		isDead_ = true;
	}
}
//描画処理
void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}