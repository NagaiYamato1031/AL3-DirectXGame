#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	inline Vector3 operator+=(const Vector3& vector) {
		this->x += vector.x;
		this->y += vector.y;
		this->z += vector.z;
		return *this;
	}

	inline Vector3 operator-=(const Vector3& vector) {
		this->x -= vector.x;
		this->y -= vector.y;
		this->z -= vector.z;
		return *this;
	}

	inline Vector3 operator+(const Vector3& vector) const {
		Vector3 temp = *this;
		temp += vector;
		return temp;
	}

	inline Vector3 operator-(const Vector3& vector) const {
		Vector3 temp = *this;
		temp -= vector;
		return temp;
	}

	inline Vector3 operator*=(float scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		return *this;
	}

	inline Vector3 operator*(float scalar) const {
		Vector3 temp = *this;
		temp *= scalar;
		return temp;
	}

	inline Vector3& operator=(const Vector3& vector) {
		this->x = vector.x;
		this->y = vector.y;
		this->z = vector.z;
		return *this;
	}
};