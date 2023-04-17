#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {	
	float x;
	float y;
	float z;
};

// 描画
static const int kColumnWidth = 68;
static const int kRowHeight = 20;
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

    // 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);

// 内積
float Dot(const Vector3& v1, const Vector3& v2);

// 長さ
float Length(const Vector3& v);

// 正規化
Vector3 Normalize(const Vector3& v);
