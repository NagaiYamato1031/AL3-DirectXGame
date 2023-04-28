#pragma once

/// <summary>
/// 4次元ベクトル
/// </summary>
struct Vector4 final {
	float x;
	float y;
	float z;
	float w;

	inline Vector4 operator+=(const Vector4& vector) {
		this->x += vector.x;
		this->y += vector.y;
		this->z += vector.z;
		this->w += vector.w;
		return *this;
	}

	inline Vector4 operator-=(const Vector4& vector) {
		this->x -= vector.x;
		this->y -= vector.y;
		this->z -= vector.z;
		this->w -= vector.w;
		return *this;
	}

	inline Vector4 operator+(const Vector4& vector) const {
		Vector4 temp = *this;
		temp += vector;
		return temp;
	}

	inline Vector4 operator-(const Vector4& vector) const {
		Vector4 temp = *this;
		temp -= vector;
		return temp;
	}

	inline Vector4 operator*=(float scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		this->w *= scalar;
		return *this;
	}

	inline Vector4 operator*(float scalar) const {
		Vector4 temp = *this;
		temp *= scalar;
		return temp;
	}

	inline Vector4& operator=(const Vector4& vector) {
		this->x = vector.x;
		this->y = vector.y;
		this->z = vector.z;
		this->w = vector.w;
		return *this;
	}
};