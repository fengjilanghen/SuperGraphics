#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <cmath>

namespace DiyMath3D
{
	//2 vector
	class CVector2f;
	//3 vector
	class CVector3f;
	//4 vector
	class CVector4f;
}

//////////////////////////////////////////////////////////
//2 vector
//////////////////////////////////////////////////////////
class DiyMath3D::CVector2f
{
public:
	CVector2f();
	CVector2f(float x, float y);
	CVector2f(const CVector2f &vtr);

	//~CVector3(){}

	CVector2f& operator=(const CVector2f &vtr);

	bool operator==(const CVector2f &vtr) const;

	bool operator!=(const CVector2f &vtr) const;

	CVector2f& operator+=(const CVector2f &vtr);

	CVector2f& operator-=(const CVector2f &vtr);

	CVector2f operator+(const CVector2f &vtr) const;

	CVector2f operator-(const CVector2f &vtr) const;

	CVector2f operator-() const;

	CVector2f& operator+=(const float scalar);

	CVector2f& operator-=(const float scalar);

	CVector2f& operator*=(const float scalar);

	CVector2f& operator/=(const float scalar);

	CVector2f operator+(const float scalar) const;

	CVector2f operator-(const float scalar) const;

	CVector2f operator*(const float scalar) const;

	CVector2f operator/(const float scalar) const;

public:
	float _x, _y;
};

//////////////////////////////////////////////////////////
//3 vector
//////////////////////////////////////////////////////////
class DiyMath3D::CVector3f
{
public:
	CVector3f();
	explicit CVector3f(const CVector2f &vec, float z);
	CVector3f(float x, float y, float z);
	CVector3f(const CVector3f &vtr);
	CVector3f(const CVector4f &vtr);

	//~CVector3(){}

	CVector3f& operator=(const CVector3f &vtr);
	CVector3f& operator=(const CVector4f &vtr);

	bool operator==(const CVector3f &vtr) const;

	bool operator!=(const CVector3f &vtr) const;

	CVector3f& operator+=(const CVector3f &vtr);

	CVector3f& operator-=(const CVector3f &vtr);

	CVector3f operator-() const;

	CVector3f operator+(const CVector3f &vtr) const;

	CVector3f operator-(const CVector3f &vtr) const;

	CVector3f& operator+=(const float scalar);

	CVector3f& operator-=(const float scalar);

	CVector3f& operator*=(const float scalar);

	CVector3f& operator/=(const float scalar);

	CVector3f operator+(const float scalar) const;

	CVector3f operator-(const float scalar) const;

	CVector3f operator*(const float scalar) const;

	CVector3f operator/(const float scalar) const;

public:
	float _x, _y, _z;
};
//////////////////////////////////////////////////////////
//4 vector
//////////////////////////////////////////////////////////
class DiyMath3D::CVector4f
{
public:
	CVector4f();
	explicit CVector4f(const CVector2f &vec, float z, float w);
	explicit CVector4f(const CVector3f &vec, float w);
	CVector4f(float x, float y, float z, float w);
	CVector4f(const CVector4f &vtr);

	//~CVector3(){}

	CVector4f& operator=(const CVector4f &vtr);

	bool operator==(const CVector4f &vtr) const;

	bool operator!=(const CVector4f &vtr) const;

	CVector4f& operator+=(const CVector4f &vtr);

	CVector4f& operator-=(const CVector4f &vtr);

	CVector4f operator+(const CVector4f &vtr) const;

	CVector4f operator-(const CVector4f &vtr) const;

	CVector4f operator-() const;

	CVector4f& operator+=(const float scalar);

	CVector4f& operator-=(const float scalar);

	CVector4f& operator*=(const float scalar);

	CVector4f& operator/=(const float scalar);

	CVector4f operator+(const float scalar) const;

	CVector4f operator-(const float scalar) const;

	CVector4f operator*(const float scalar) const;

	CVector4f operator/(const float scalar) const;

public:
	float _x, _y, _z, _w;
};

#endif //_VECTOR_H_