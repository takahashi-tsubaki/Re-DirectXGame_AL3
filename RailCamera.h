#pragma once
#include <cassert>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affin/affin.h"
#include "Input.h"
#include "DebugText.h"
class RailCamera
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
  void Init(const Vector3& position, const Vector3& rotation);


	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	float ConvertToRadians(float fDegrees) noexcept;
	float ConvertToDegrees(float fRadians) noexcept;

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//ワールド座標の取得
	WorldTransform* GetWorldPosition();

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//
	Input* input_ = nullptr;
	//
	DebugText* debugText_ = nullptr;

	const float PI = 3.141592654f;
};