#pragma once
#pragma once
#include "Matrix4.h"
#include "WorldTransform.h"
#include "math.h"

namespace affin {
	
  void matScale(Matrix4 mat,WorldTransform &worldTransform);
  void matRot(Matrix4 mat, WorldTransform& worldTransform,int axis);
  void matTrans(Matrix4 mat, WorldTransform& worldTransform);

  Matrix4 GenerateScaleMat(Matrix4 mat,WorldTransform& worldTransform);
  Matrix4 GenerateRotateXMat(Matrix4 mat, WorldTransform& worldTransform);
  Matrix4 GenerateRotateYMat(Matrix4 mat, WorldTransform& worldTransform);
  Matrix4 GenerateRotateZMat(Matrix4 mat, WorldTransform& worldTransform);
  Matrix4 GenerateTranslateMat(Matrix4 mat, WorldTransform& worldTransform);
 
 
  float PI = 3.141592f;
};