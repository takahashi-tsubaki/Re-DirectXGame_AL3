#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
class Player {
  public:
	Player();
	//
	void Initialize(Model* model, uint32_t textureHandle);
	//
	void Update();
	//
	void Draw(ViewProjection viewProjection);

	float Translate();

  private:
	//
	WorldTransform worldTransform_;
	//
	Model* model_ = nullptr;
	//
	uint32_t textureHandle_ = 0u;
	//
	Input* input_ = nullptr;
	//
	DebugText* debugText_ = nullptr;
};