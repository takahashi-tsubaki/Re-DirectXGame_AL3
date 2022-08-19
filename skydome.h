#pragma once
#include <cassert>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"
#include "affin/affin.h"
class skydome 
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
  void Init(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//
	ViewProjection viewProjection_;
	//モデル
	Model* model_ = nullptr;
	// Affin関数の構造体
	affin::AffinMat affinMat;
};