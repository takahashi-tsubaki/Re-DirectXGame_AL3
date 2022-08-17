#include "affin.h"

void affin::setScaleMat(Matrix4& mat, WorldTransform& worldTransform) {
	//�X�P�[�����O�{�����s��ɐݒ肷��
	mat.m[0][0] = worldTransform.scale_.x;
	mat.m[1][1] = worldTransform.scale_.y;
	mat.m[2][2] = worldTransform.scale_.z;
}

Matrix4 affin::generateScaleMat(WorldTransform& worldTransform) {
	//�X�P�[�����O�s���錾
	Matrix4 matScale = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	                    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	//�X�P�[�����O�{�����s��ɐݒ肷��
	matScale.m[0][0] = worldTransform.scale_.x;
	matScale.m[1][1] = worldTransform.scale_.y;
	matScale.m[2][2] = worldTransform.scale_.z;

	return matScale;
}

void affin::setRotateMat(AffinMat& mat, WorldTransform& worldTransform) {
	mat.rotateX.m[1][1] = cos(worldTransform.rotation_.x);
	mat.rotateX.m[1][2] = sin(worldTransform.rotation_.x);
	mat.rotateX.m[2][1] = -sin(worldTransform.rotation_.x);
	mat.rotateX.m[2][2] = cos(worldTransform.rotation_.x);

	mat.rotateY.m[0][0] = cos(worldTransform.rotation_.y);
	mat.rotateY.m[0][2] = -sin(worldTransform.rotation_.y);
	mat.rotateY.m[2][0] = sin(worldTransform.rotation_.y);
	mat.rotateY.m[2][2] = cos(worldTransform.rotation_.y);

	mat.rotateZ.m[0][0] = cos(worldTransform.rotation_.z);
	mat.rotateZ.m[0][1] = sin(worldTransform.rotation_.z);
	mat.rotateZ.m[1][0] = -sin(worldTransform.rotation_.z);
	mat.rotateZ.m[1][1] = cos(worldTransform.rotation_.z);
}

//
// void affin::setRotateMat(Matrix4& mat, WorldTransform& worldTransform, int axis)
//{
//	switch (axis)
//	{
//		case 1:
//			mat.m[1][1] = cos(worldTransform.rotation_.x);
//			mat.m[1][2] = sin(worldTransform.rotation_.x);
//			mat.m[2][1] = -sin(worldTransform.rotation_.x);
//			mat.m[2][2] = cos(worldTransform.rotation_.x);
//
//			break;
//		case 2:
//			mat.m[0][0] = cos(worldTransform.rotation_.y);
//			mat.m[0][2] = -sin(worldTransform.rotation_.y);
//			mat.m[2][0] = sin(worldTransform.rotation_.y);
//			mat.m[2][2] = cos(worldTransform.rotation_.y);
//
//			break;
//		case 3:
//			mat.m[0][0] = cos(worldTransform.rotation_.z);
//			mat.m[0][1] = sin(worldTransform.rotation_.z);
//			mat.m[1][0] = -sin(worldTransform.rotation_.z);
//			mat.m[1][1] = cos(worldTransform.rotation_.z);
//
//			break;
//	}
//}

Matrix4 affin::generateRotateXMat(WorldTransform& worldTransform) {
	// X����]�s���錾
	Matrix4 matRotX = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	                   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	matRotX.m[1][1] = cos(worldTransform.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform.rotation_.x);

	return matRotX;
}

Matrix4 affin::generateRotateYMat(WorldTransform& worldTransform) {
	// Y����]�s���錾
	Matrix4 matRotY = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	                   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	matRotY.m[0][0] = cos(worldTransform.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
	matRotY.m[2][0] = sin(worldTransform.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform.rotation_.y);

	return matRotY;
}

Matrix4 affin::generateRotateZMat(WorldTransform& worldTransform) {
	// Z����]�s���錾
	Matrix4 matRotZ = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	                   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform.rotation_.z);

	return matRotZ;
}

void affin::setTranslateMat(Matrix4& mat, WorldTransform& worldTransform) {
	mat.m[3][0] = worldTransform.translation_.x;
	mat.m[3][1] = worldTransform.translation_.y;
	mat.m[3][2] = worldTransform.translation_.z;
}

Matrix4 affin::generateTransMat(WorldTransform& worldTransform) {
	//���s�ړ��s���錾
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matTrans.m[3][0] = worldTransform.translation_.x;
	matTrans.m[3][1] = worldTransform.translation_.y;
	matTrans.m[3][2] = worldTransform.translation_.z;

	return matTrans;
}

void affin::setTransformationWolrdMat(AffinMat& affinMat, WorldTransform& worldTransform) {
	worldTransform.matWorld_ *= affinMat.scale;
	worldTransform.matWorld_ *= affinMat.rotateX;
	worldTransform.matWorld_ *= affinMat.rotateY;
	worldTransform.matWorld_ *= affinMat.rotateZ;
	worldTransform.matWorld_ *= affinMat.translate;
}
