#include "affin.h"

void affin::SetMatScale(AffinMat& mat, WorldTransform& worldTransform) {
	
	// X,Y,Z方向のスケーリングを設定
	mat.matScale = {
	  worldTransform.scale_.x, 0.0f, 0.0f,0.0f,
	  0.0f,worldTransform.scale_.y,0.0f,0.0f,
	  0.0f, 0.0f, worldTransform.scale_.z,0.0f,
	  0.0f, 0.0f, 0.0f, 1.0f,
	};
}


void affin::SetMatTrans(AffinMat& mat, WorldTransform& worldTransform) {
	//移動量を行列に設定
	mat.matTrans = {
	  1.0f, 0.0f, 0.0f, 0.0f, 
	  0.0f, 1.0f, 0.0f, 0.0f,
	  0.0f, 0.0f, 1.0f, 0.0f,
	  worldTransform.translation_.x,worldTransform.translation_.y, worldTransform.translation_.z,1.0f,
	};
}

void affin::SetMatRot(AffinMat& mat, WorldTransform& worldTransform) {
	//X軸の回転
	mat.matRotX = {
	  1.0f, 0.0f, 0.0f, 0.0f,         
	  0.0f, cos(worldTransform.rotation_.x),sin(worldTransform.rotation_.x), 0.0f,
	  0.0f, -sin(worldTransform.rotation_.x), cos(worldTransform.rotation_.x), 0.0f,
	  0.0f, 0.0f, 0.0f, 1.0f,
	};

	// Y軸の回転
	mat.matRotY = {
	  cos(worldTransform.rotation_.y), 0.0f, -sin(worldTransform.rotation_.y), 0.0f,
	  0.0f, 1.0f, 0.0, 0.0f,
	  sin(worldTransform.rotation_.y), 0.0f, cos(worldTransform.rotation_.y), 0.0f,
	  0.0f, 0.0f, 0.0f,1.0f,
	};

	// Z軸の回転
	mat.matRotZ = {
	  cos(worldTransform.rotation_.z),sin(worldTransform.rotation_.z), 0.0f,0.0f,
	  -sin(worldTransform.rotation_.z), cos(worldTransform.rotation_.z), 0.0f, 0.0f,
	  0.0f,0.0f,1.0f, 0.0f, 
	  0.0f,0.0f,0.0f, 1.0f,
	};
}
Matrix4 affin::GenerateScaleMat(Matrix4& mat, WorldTransform& worldTransform) {
	//単位行列の代入
	Matrix4 matScale = {
	  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
	};
	matScale.m[0][0] = worldTransform.scale_.x;
	matScale.m[1][1] = worldTransform.scale_.y;
	matScale.m[2][2] = worldTransform.scale_.z;

	return matScale;
}
Matrix4 affin::GenerateRotateXMat(Matrix4& mat, WorldTransform& worldTransform) {
	Matrix4 matRotX = {
	  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	matRotX.m[1][1] = cos(worldTransform.scale_.x);
	matRotX.m[1][2] = sin(worldTransform.scale_.x);
	matRotX.m[2][2] = -sin(worldTransform.scale_.x);
	matRotX.m[2][2] = cos(worldTransform.scale_.x);

	return matRotX;
}
Matrix4 affin::GenerateRotateYMat(Matrix4& mat, WorldTransform& worldTransform) {
	Matrix4 matRotY = {
	  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	matRotY.m[0][0] = cos(worldTransform.scale_.y);
	matRotY.m[0][2] = -sin(worldTransform.scale_.y);
	matRotY.m[2][0] = sin(worldTransform.scale_.y);
	matRotY.m[2][2] = cos(worldTransform.scale_.y);

	return matRotY;
}
Matrix4 affin::GenerateRotateZMat(Matrix4& mat, WorldTransform& worldTransform) {
	
	Matrix4 matRotZ = {
	  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	matRotZ.m[0][0] = cos(worldTransform.scale_.z);
	matRotZ.m[0][1] = sin(worldTransform.scale_.z);
	matRotZ.m[1][0] = -sin(worldTransform.scale_.z);
	matRotZ.m[1][1] = cos(worldTransform.scale_.z);

	return matRotZ;
}

Matrix4 affin::GenerateTranslateMat(Matrix4& mat, WorldTransform& worldTransform) {
	 //移動量を行列に設定
	 Matrix4 matTrans = {
	    1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
	  };
	 matTrans.m[3][0] = worldTransform.translation_.x;
	 matTrans.m[3][1] = worldTransform.translation_.y;
	 matTrans.m[3][2] = worldTransform.translation_.z;
	 return matTrans;
}
void affin::SetWorldMat(AffinMat& mat, WorldTransform& worldTransform) 
{
	worldTransform.matWorld_ *= mat.matScale;
	worldTransform.matWorld_ *= mat.matRotX;
	worldTransform.matWorld_ *= mat.matRotY;
	worldTransform.matWorld_ *= mat.matRotZ;
	worldTransform.matWorld_ *= mat.matScale;
}