#include "skydome.h"
void skydome::Init(Model* model) {
	assert(model);

	model_ = model;

	worldTransform_.Initialize();

}

void skydome::Update()
{
	
}

void skydome::Draw(ViewProjection viewProjection_) 
{ 
	model_->Draw(worldTransform_,viewProjection_);
}