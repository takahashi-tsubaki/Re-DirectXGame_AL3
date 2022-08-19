#include "skydome.h"
void skydome::Init(Model* model) {
	assert(model);

	model_ = model;

	worldTransform_.Initialize();

	//�V���̊g��
	worldTransform_.scale_ = {2, 2, 2};
	Matrix4 matScale;

	matScale = affin::generateScaleMat(worldTransform_);

	worldTransform_.matWorld_.operator*=(matScale);
	//�s��̓]��
	worldTransform_.TransferMatrix();
}

void skydome::Update()
{ 

	
}

void skydome::Draw(ViewProjection viewProjection_) 
{ 
	model_->Draw(worldTransform_,viewProjection_);
}