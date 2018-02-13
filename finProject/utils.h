#pragma once

#include<math.h>
#include<iostream>

using namespace std;
const float eps = 1e-6;



class Vec3f {
private:
	bool isZero(float x1) {
		if (abs(x1) < eps) {
			return true;
		}
		else {
			return false;
		}
	}
public:
	float x, y, z;
	Vec3f(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	Vec3f(const Vec3f& _vec3f) {
		x = _vec3f.x;
		y = _vec3f.y;
		z = _vec3f.z;
	}

	Vec3f() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	
	// vec += vec vec + vec 
	inline Vec3f& operator += (const Vec3f &cur) {
		x += cur.x;
		y += cur.y;
		z += cur.z;
		return *this;
	}
	friend inline Vec3f operator + (const Vec3f &cur1, const Vec3f &cur2) {
		return Vec3f(cur1.x + cur2.x, cur1.y + cur2.y, cur1.z + cur2.z);
	 }
	// vec -= vec vec - vec 
	inline Vec3f& operator -= (const Vec3f &cur) {
		x -= cur.x;
		y -= cur.y;
		z -= cur.z;
		return *this;
	}
	friend inline Vec3f operator - (const Vec3f &cur1, const Vec3f &cur2) {
		return Vec3f(cur1.x - cur2.x, cur1.y - cur2.y, cur1.z - cur2.z);
	}
	// vec *= vec vec * vec 
	inline Vec3f& operator *= (const Vec3f &cur) {
		x *= cur.x;
		y *= cur.y;
		z *= cur.z;
		return *this;
	}
	friend inline Vec3f operator * (const Vec3f &cur1, const Vec3f &cur2) {
		return Vec3f(cur1.x * cur2.x, cur1.y * cur2.y, cur1.z * cur2.z);
	}
	// vec /= vec vec / vec 
	inline Vec3f& operator /= (const Vec3f &cur) {
		x /= cur.x;
		y /= cur.y;
		z /= cur.z;
		return *this;
	}
	friend inline Vec3f operator / (const Vec3f &cur1, const Vec3f &cur2) {
		return Vec3f(cur1.x / cur2.x, cur1.y / cur2.y, cur1.z / cur2.z);
	}
	// = == !=
	inline Vec3f& operator = (const Vec3f &cur) {
		x = cur.x;
		y = cur.y;
		z = cur.z;
		return *this;
	}
	inline bool operator == (const Vec3f &cur) {
		return (isZero(x - cur.x) && isZero(y - cur.y) && isZero(z - cur.z));
	}
	inline bool operator != (const Vec3f &cur) {
		return (!isZero(x - cur.x) || !isZero(y - cur.y) || !isZero(z - cur.z));
	}

	inline float length() {
		return sqrt(x*x + y*y + z*z);
	}
	inline Vec3f normalize() {
		return Vec3f(x / length(), y / length(), z / length());
	}
	void printVect() {
		cout << "x:" << x << ",y:" << y << ",z:" << z << endl;
	}
	~Vec3f(){}
};


inline float dotMul(const Vec3f &cur1, const Vec3f &cur2) {
	Vec3f tem = cur1*cur2;
	return (tem.x + tem.y + tem.z);
}
inline Vec3f crossMul(const Vec3f &cur1, const Vec3f &cur2) {
	return Vec3f(cur1.y * cur2.z - cur1.z * cur2.y,
		cur1.z * cur2.x - cur1.x * cur2.z,
		cur1.x * cur2.y - cur1.y * cur2.x);
}

