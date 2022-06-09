#include "affin.h"

void affin::matScale(Matrix4 mat, WorldTransform& worldTransform)
{
	

	// X,Y,Z方向のスケーリングを設定
	mat = {
	  worldTransform.scale_.x, 0.0f, 0.0f,0.0f,
	  0.0f,worldTransform.scale_.y,0.0f,0.0f,
	  0.0f, 0.0f, worldTransform.scale_.z,0.0f,
	  0.0f, 0.0f, 0.0f, 1.0f,
	};
}
Matrix4 affin::GenerateScaleMat(Matrix4 mat,WorldTransform& worldTransform) {
	//単位行列の代入
	worldTransform.matWorld_ = {
	  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	worldTransform.matWorld_ *= mat;
}
void affin::matRot(Matrix4 mat, WorldTransform& worldTransform, int axis) 
{ 
	switch (axis) 
	{ 
		case 1:
		{
			mat = {
			  1.0f, 0.0f,         0.0f,        0.0f,
			  0.0f, cos(PI / 4),  sin(PI / 4), 0.0f,
			  0.0f, -sin(PI / 4), cos(PI / 4), 0.0f,
			  0.0f, 0.0f,         0.0f,        1.0f,
			};
		    break;
		}
		case 2: 
		{
		    mat = {
		      cos(PI / 4), 0.0f, -sin(PI / 4), 0.0f, 
			  0.0f, 1.0f,  0.0,  0.0f,
		      sin(PI / 4), 0.0f, cos(PI / 4),  0.0f,
			  0.0f, 0.0f,  0.0f, 1.0f,
		    };
		    break;
		}
		case 4: 
		{
		    mat = {
		      cos(PI / 4),  sin(PI / 4), 0.0f, 0.0f,
			  -sin(PI / 4), cos(PI / 4), 0.0f, 0.0f,
		      0.0f,         0.0f,        1.0f, 0.0f,
			  0.0f,         0.0f,        0.0f, 1.0f,
		    };
		    break;
		}
	}
}
Matrix4 affin::GenerateRotateXMat(Matrix4 mat, WorldTransform& worldTransform) {

}
Matrix4 affin::GenerateRotateYMat(Matrix4 mat, WorldTransform& worldTransform) {

}
Matrix4 affin::GenerateRotateZMat(Matrix4 mat, WorldTransform& worldTransform) {

}