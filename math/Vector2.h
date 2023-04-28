#pragma once

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;

	inline Vector2 operator+=(const Vector2& vector) {
		this->x += vector.x;
		this->y += vector.y;
		return *this;
	}

	inline Vector2 operator-=(const Vector2& vector) {
		this->x -= vector.x;
		this->y -= vector.y;
		return *this;
	}

	inline Vector2 operator+(const Vector2& vector) const {
		Vector2 temp = *this;
		temp += vector;
		return temp;
	}

	inline Vector2 operator-(const Vector2& vector) const {
		Vector2 temp = *this;
		temp -= vector;
		return temp;
	}

	inline Vector2 operator*=(float scalar) {
		this->x *= scalar;
		this->y *= scalar;
		return *this;
	}

	inline Vector2 operator*(float scalar) const {
		Vector2 temp = *this;
		temp *= scalar;
		return temp;
	}

	inline Vector2& operator=(const Vector2& vector) {
		this->x = vector.x;
		this->y = vector.y;
		return *this;
	}
};
