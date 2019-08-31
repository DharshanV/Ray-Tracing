#ifndef VECTOR3F_H
#define VECTOR3F_H
#include <iostream>
#include <cmath>
#include <assert.h>
using namespace std;
class Vector3f {
public:
	Vector3f() : x(0), y(0), z(0) { }
	Vector3f(float x, float y, float z) : x(x), y(y), z(z) { }
	Vector3f(float value) : x(value), y(value), z(value) { }
	Vector3f(const Vector3f& v) : x(v.x), y(v.y), z(v.z) { }
	void print(ostream& out) const { out << "[" << x << "," << y << "," << z<<"]"; }
	virtual ~Vector3f() { }
public:
	inline float normalize() {
		float L = this->length();
		*this /= L;
		return L;
	}

	inline Vector3f getNormalized() {
		Vector3f temp(*this);
		temp /= temp.length();
		return temp;
	}

	inline float length() {
		return this->sqrt(sqr(x) + sqr(y) + sqr(z));
	}

	inline float dot(const Vector3f& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	inline Vector3f cross(const Vector3f& v) const {
		Vector3f temp;
		temp.x = y * v.z - (v.y* z);
		temp.y = -(x*v.z - (v.x * z));
		temp.z = x * v.y - (v.x*y);
		return temp;
	}

public:
	inline Vector3f& operator = (const Vector3f& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	inline Vector3f& operator += (const Vector3f& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline Vector3f& operator -= (const Vector3f& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline Vector3f& operator /= (float value) {
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}

	inline Vector3f& operator *= (float value) {
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	inline float operator [] (int index) {
		if (index == 0) return x;
		if (index == 1) return y;
		if (index == 2) return z;
		assert(!true);
	}

	inline Vector3f operator - () {
		Vector3f temp(*this);
		temp *= -1;
		return temp;
	}
private:
	inline float sqr(float value) {
		return value * value;
	}

	inline float sqrt(float value) {
		return sqrtf(value);
	}

private:
	float x, y, z;
};

inline Vector3f operator + (const Vector3f& a, const Vector3f& b) {
	Vector3f temp(a);
	temp += b;
	return temp;
}

inline Vector3f operator - (const Vector3f& a, const Vector3f& b) {
	Vector3f temp(a);
	temp -= b;
	return temp;
}

inline Vector3f operator * (const Vector3f& a, float value) {
	Vector3f temp(a);
	temp *= value;
	return temp;
}

inline Vector3f operator / (const Vector3f& a, float value) {
	Vector3f temp(a);
	temp /= value;
	return temp;
}

ostream& operator << (ostream& out, const Vector3f& a) {
	a.print(out);
	return out;
}

#endif // !VECTOR3F_H