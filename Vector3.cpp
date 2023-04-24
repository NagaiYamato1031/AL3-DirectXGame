#include "Vector3.h"

//#include <Novice.h>
#include <math.h>


// 描画
//void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
//	Novice::ScreenPrintf(x, y, "%0.2f", vector.x);
//	Novice::ScreenPrintf(x + kColumnWidth, y, "%0.2f", vector.y);
//	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%0.2f", vector.z);
//	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
//}

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {
	return Vector3(scalar * v.x, scalar * v.y, scalar * v.z);
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// 長さ
float Length(const Vector3& v) {
	return sqrtf(Dot(v, v));
}

// 正規化
Vector3 Normalize(const Vector3& v) {
	float length = Length(v);
	Vector3 temp = v;
	if (length != 0) {
		temp.x /= length;
		temp.y /= length;
		temp.z /= length;
	}
	return temp;
}

