#pragma once
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
	static const int kRowHeight = 20;
	static const int kColumnWidth = 68;
};

void Matrix4x4ScreenPrintf(int x, int y, const Matrix4x4& matrix);

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Inverse(const Matrix4x4& m1);
Matrix4x4 Transpose(const Matrix4x4& m1);

inline Matrix4x4 MakeIdentity4x4() {
	return Matrix4x4{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
}