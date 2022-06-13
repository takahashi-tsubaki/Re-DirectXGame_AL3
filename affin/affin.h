#pragma once
#pragma once
#include "MathUtility.h"
#include "Matrix4.h"
#include "WorldTransform.h"
#include "Math.h"

namespace affin {

enum Axis {
	Axis_NONE,//0
	Axis_X,//1
	Axis_Y,//2
	Axis_XY,//3
	Axis_Z,//4
	Axis_XZ,//5
	Axis_YZ,//6
	Axis_XYZ//7
};

typedef struct AffinMat {
	Matrix4 matScale = MathUtility::Matrix4Identity();
	Matrix4 matRotX = MathUtility::Matrix4Identity();
	Matrix4 matRotY = MathUtility::Matrix4Identity();
	Matrix4 matRotZ = MathUtility::Matrix4Identity();
	Matrix4 matTrans = MathUtility::Matrix4Identity();
};

void SetMatScale(AffinMat& mat, WorldTransform& worldTransform);
void SetMatTrans(AffinMat& mat, WorldTransform& worldTransform);
void SetMatRot(AffinMat& mat, WorldTransform& worldTransform);

Matrix4 GenerateScaleMat(Matrix4& mat, WorldTransform& worldTransform);
Matrix4 GenerateRotateXMat(Matrix4& mat, WorldTransform& worldTransform);
Matrix4 GenerateRotateYMat(Matrix4& mat, WorldTransform& worldTransform);
Matrix4 GenerateRotateZMat(Matrix4& mat, WorldTransform& worldTransform);
Matrix4 GenerateTranslateMat(Matrix4& mat, WorldTransform& worldTransform);

void SetWorldMat(AffinMat& mat, WorldTransform& worldTransform);
}; // namespace affin