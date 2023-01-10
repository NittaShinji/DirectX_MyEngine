#include"MathUtility.h"
#include <cassert>
#include <cmath>

namespace MathUtility
{

	//Vector3 VectorSubtract(Vector3 eyePos, Vector3 target)
	//{
	//	Matrix4 result = 
	//	{
	//		 eyePos.vector4_f32[0] - V2.vector4_f32[0],
	//			V1.vector4_f32[1] - V2.vector4_f32[1],
	//			V1.vector4_f32[2] - V2.vector4_f32[2],
	//			V1.vector4_f32[3] - V2.vector4_f32[3]
	//	}
	//}

	//Matrix4 MatrixLooktoLH(Vector3 eyePos, Vector3 eyeDirection, Vector3 up)
	//{
	//
	//}
	//
	//Matrix4 TestMatrix4LookAtLH(const Vector3& eyePos, const Vector3& target, const Vector3& up)
	//{
	//	Vector3 eyeDirection;
	//	VectorSubtract(eyePos, target);
	//	return MatrixLooktoLH(eyePos, eyeDirection, up);
	//}

	//ビュー行列の作成
	Matrix4 Matrix4LookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up)
	{
		assert(eye.equal(target));
		assert(up.equal(Vector3{ 0,0,0 }));

		// 視線方向
		Vector3 eyeDirection = target - eye;

		// 各軸ベクトルを算出
		Vector3 R2 = eyeDirection;
		R2.normalize();

		Vector3 R0 = up.cross(R2);
		R0.normalize();

		Vector3 R1 = R2.cross(R0);
		
		// 平行移動成分を算出
		Vector3 NegEyePosition = -eye;

		float D0 = R0.dot(NegEyePosition);
		float D1 = R1.dot(NegEyePosition);
		float D2 = R2.dot(NegEyePosition);

		Matrix4 m{ R0.x, R1.x, R2.x, 0.0f, R0.y, R1.y, R2.y, 0.0f,
				  R0.z, R1.z, R2.z, 0.0f, D0,   D1,   D2,   1.0f };

		return m;
	}

	//透視投影行列の作成
	Matrix4 Matrix4Perspective(float fovAngleY, float aspectRatio, float nearZ, float farZ) {
		assert(nearZ > 0.0f && farZ > 0.f);
		assert(fabsf(fovAngleY) > 0.00001f * 2.0f);
		assert(fabsf(aspectRatio) > 0.00001f);
		assert(fabsf(farZ - nearZ) > 0.00001f);

		float sinFov = std::sin(0.5f * fovAngleY);
		float cosFov = std::cos(0.5f * fovAngleY);

		float height = cosFov / sinFov;
		float width = height / aspectRatio;
		float fRange = farZ / (farZ - nearZ);

		Matrix4 m{ width, 0.0f,   0.0f, 0.0f, 0.0f, height, 0.0f, 0.0f, 0.0f,
				  0.0f,  fRange, 1.0f, 0.0f, 0.0f, -fRange * nearZ, 0.0f };

		return m;
	}

	//度数法変換(ラジアン→度数法)
	float ToRadian(float angle)
	{
		return angle / 180;
	}
}