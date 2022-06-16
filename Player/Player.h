#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include "affin/affin.h"
class Player {
  public:
	Player();
	//
	void Initialize(Model* model, uint32_t textureHandle);
	//
	void Update();
	//
	void Draw(ViewProjection viewProjection);

	void Translate();

	void matRotY();

	void setWorldMat();

	float ConvertToRadians(float fDegrees) noexcept ; 
	float ConvertToDegrees(float fRadians) noexcept ; 

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

	const float PI = 3.141592654f;

	Matrix4 mat;
	affin::AffinMat affinMat;
};