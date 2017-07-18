#ifndef VECTOR3_CPP
#define VECTOR3_CPP
#include "../include/Vector3.h"
#include <cmath>
#include "..\include\Vector3.h"

namespace Johnny
{
	template<class T>
	Vector3<T>::Vector3(const T& x, const T& y, const T& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	template<class T>
	Vector3<T>::Vector3(const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	template<class T>
	T Vector3<T>::length(bool squared) const
	{
		if (squared)
			return x*x + y*y + z*z;
		else
			return sqrt(x*x + y*y + z*z);
	}
	template<class T>
	Vector3<T>& Vector3<T>::normalise()
	{
		T Length = length();

		x /= Length;
		y /= Length;
		z /= Length;

		return *this;
	}
	template<class T>
	Vector3<T>& Vector3<T>::add(const Vector3<T>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}
	template<class T>
	Vector3<T>& Vector3<T>::subtract(const Vector3<T>& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}
	template<class T>
	Vector3<T>& Vector3<T>::multiply(const Vector3<T>& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;

		return *this;
	}
	template<class T>
	Vector3<T>& Vector3<T>::multiply(const T& s)
	{
		x *= s;
		y *= s;
		z *= s;

		return *this;
	}
	template<class T>
	Vector3<T>& Vector3<T>::divide(const Vector3<T>& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;

		return *this;
	}
	template<class T>
	Vector3<T>& Vector3<T>::divide(const T& s)
	{
		x /= s;
		y /= s;
		z /= s;

		return *this;
	}
	template<class T>
	Vector3<T> Vector3<T>::cross(const Vector3<T>& v)
	{
		Vector3<T> v1(*this);

		v1.x = v1.y * v.z - v1.z * v.y;
		v1.y = v1.z * v.x - v1.x * v.z;
		v1.z = v1.x * v.y - v1.y * v.x;

		return v1;
	}
	template<class T>
	T Vector3<T>::distance(const Vector3<T>& v, bool squared) const
	{
		Vector3<T> v1(v);

		v1.subtract(*this);

		return v1.length(squared);
	}
	template<class T>
	Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& v2)
	{
		return add(v2);
	}
	template<class T>
	Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& v2)
	{
		return subtract(v2);
	}
	template<class T>
	Vector3<T>& Vector3<T>::operator*=(const Vector3<T>& v2)
	{
		return multiply(v2);
	}
	template<class T>
	Vector3<T>& Vector3<T>::operator/=(const Vector3<T>& v2)
	{
		return divide(v2);
	}
	template<class T>
	const Vector3<T> operator+(const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return Vector3<T>(v1).add(v2);
	}
	template<class T>
	const Vector3<T> operator-(const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return Vector3<T>(v1).subtract(v2);
	}
	template<class T>
	const Vector3<T> operator*(const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return Vector3<T>(v1).multiply(v2);
	}
	template<class T>
	const Vector3<T> operator/(const Vector3<T>& v1, const Vector3<T>& v2)
	{
		return Vector3<T>(v1).divide(v2);
	}
	template<class T>
	std::ostream& operator<<(std::ostream& os, const Vector3<T>& v)
	{
		os << "(" << v.x << ";" << v.y << ";" << v.z << ")";
		return os;
	}
}

#endif
