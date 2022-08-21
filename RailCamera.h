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
	/// ������
	/// </summary>
  void Init(const Vector3& position, const Vector3& rotation);


	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	float ConvertToRadians(float fDegrees) noexcept;
	float ConvertToDegrees(float fRadians) noexcept;

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//���[���h���W�̎擾
	WorldTransform* GetWorldPosition();

private:
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
	//
	Input* input_ = nullptr;
	//
	DebugText* debugText_ = nullptr;

	const float PI = 3.141592654f;
};