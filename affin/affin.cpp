#include "affin.h"
using namespace MathUtility;
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

Matrix4 affin::setViewportMat(WorldTransform& worldTransform, WinApp* window, const Vector3& v)
{
	//�P�ʍs��̐ݒ�
	Matrix4 matViewport = MathUtility::Matrix4Identity();
	matViewport.m[0][0] = window->GetInstance()->kWindowWidth / 2;
	matViewport.m[1][1] = -window->GetInstance()->kWindowHeight / 2;
	matViewport.m[3][0] = (window->GetInstance()->kWindowWidth / 2) + v.x;
	matViewport.m[3][1] = (window->GetInstance()->kWindowHeight / 2) + v.y;
	return matViewport;
}

void affin::setTransformationWolrdMat(AffinMat& affinMat, WorldTransform& worldTransform) {
	worldTransform.matWorld_ *= affinMat.scale;
	worldTransform.matWorld_ *= affinMat.rotateX;
	worldTransform.matWorld_ *= affinMat.rotateY;
	worldTransform.matWorld_ *= affinMat.rotateZ;
	worldTransform.matWorld_ *= affinMat.translate;
}
Vector3 affin::matVector(Vector3 v, Matrix4 mat) 
{
	Vector3 pos;
	pos.x = mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z + mat.m[0][3] * 1;
	pos.y = mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z + mat.m[1][3] * 1;
	pos.z = mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z + mat.m[2][3] * 1;

	return pos;
}
Vector3 affin::Vector3Normalize(Vector3& v)
{
	float x, y, z, w;

	//����
	w = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	//���ꂼ��̐����𐳋K�������x�N�g��
	x = v.x / w;
	y = v.y / w;
	z = v.z / w;

	return Vector3(x, y, z);

}
Vector3 affin::GetWorldPosition(Matrix4 mat) 
{
	//���[���h���W�����邽�߂̕ϐ�
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = mat.m[3][0];
	worldPos.y = mat.m[3][1];
	worldPos.z = mat.m[3][2];

	//�߂�l
	return worldPos;
}

const Vector3 affin::addVector3(const Vector3& v1, const Vector3& v2) 
{ 
	Vector3 addVec;
	addVec.x = v1.x + v2.x;
	addVec.y = v1.y + v2.y;
	addVec.z = v1.z + v2.z;

	return addVec;
}
const Vector3 affin::division(const Vector3& v, Matrix4 mat)
{
	Vector4 devision;
	devision.x = mat.m[0][0] * v.x + mat.m[1][0] * v.y + mat.m[2][0] * v.z + mat.m[3][0] * 1;
	devision.y = mat.m[0][1] * v.x + mat.m[1][1] * v.y + mat.m[2][1] * v.z + mat.m[3][1] * 1;
	devision.z = mat.m[0][2] * v.x + mat.m[1][2] * v.y + mat.m[2][2] * v.z + mat.m[3][2] * 1;
	devision.w = mat.m[0][3] * v.x + mat.m[1][3] * v.y + mat.m[2][3] * v.z + mat.m[3][3] * 1;
																				   
	devision.x = devision.x / devision.w; 
	devision.y = devision.y / devision.w; 
	devision.z = devision.z / devision.w; 

	return {devision.x, devision.y, devision.z};
}
void affin::HangView(Matrix4 mat1, Matrix4 mat2, Matrix4 mat3)
{ 
	 mat1 * mat2 * mat3;
}