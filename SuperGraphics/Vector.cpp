#include "Vector.h"

namespace DiyMath3D
{
	//////////////////////////////////////////////////////////
	//2 vector
	//////////////////////////////////////////////////////////
	CVector2f::CVector2f() 
		:_x(0), _y(0)
	{
	}

	CVector2f::CVector2f(float x, float y) 
		: _x(x), _y(y)
	{
	}
	CVector2f::CVector2f(const CVector2f &vtr)
	{
		*this = vtr;
	}

	CVector2f& CVector2f::operator=(const CVector2f &vtr)
	{
		_x = vtr._x;
		_y = vtr._y;
		return *this;
	}

	bool CVector2f::operator==(const CVector2f &vtr) const
	{
		return (_x == vtr._x &&
			_y == vtr._y);
	}

	bool CVector2f::operator!=(const CVector2f &vtr) const
	{

		return (_x != vtr._x ||
			_y != vtr._y);
	}

	CVector2f& CVector2f::operator+=(const CVector2f &vtr)
	{
		_x += vtr._x;
		_y += vtr._y;
		return *this;
	}

	CVector2f& CVector2f::operator-=(const CVector2f &vtr)
	{
		_x -= vtr._x;
		_y -= vtr._y;
		return *this;
	}

	CVector2f CVector2f::operator+(const CVector2f &vtr) const
	{
		return CVector2f(_x + vtr._x, _y + vtr._y);
	}

	CVector2f CVector2f::operator-(const CVector2f &vtr) const
	{
		return CVector2f(_x - vtr._x, _y - vtr._y);
	}

	CVector2f CVector2f::operator-() const
	{
		return *this * -1;
	}

	CVector2f& CVector2f::operator+=(const float scalar)
	{
		*this += CVector2f(scalar, scalar);
		return *this;
	}

	CVector2f& CVector2f::operator-=(const float scalar)
	{
		*this -= CVector2f(scalar, scalar);
		return *this;
	}

	CVector2f& CVector2f::operator*=(const float scalar)
	{
		_x *= scalar;
		_y *= scalar;
		return *this;
	}

	CVector2f& CVector2f::operator/=(const float scalar)
	{
		//no zero check
		_x /= scalar;
		_y /= scalar;
		return *this;
	}

	CVector2f CVector2f::operator+(const float scalar) const
	{
		return CVector2f(_x + scalar, _y + scalar);
	}

	CVector2f CVector2f::operator-(const float scalar) const
	{
		return CVector2f(_x - scalar, _y - scalar);
	}

	CVector2f CVector2f::operator*(const float scalar) const
	{
		return CVector2f(_x * scalar, _y * scalar);;
	}

	CVector2f CVector2f::operator/(const float scalar) const
	{
		return CVector2f(_x / scalar, _y / scalar);
	}

	//////////////////////////////////////////////////////////
	//3 vector
	//////////////////////////////////////////////////////////
	CVector3f::CVector3f() 
		: _x(0), _y(0), _z(0)
	{
	}

	CVector3f::CVector3f(float x, float y, float z) 
		: _x(x), _y(y), _z(z)
	{
	}

	CVector3f::CVector3f(const CVector2f &vec, float z)
		: _x(vec._x), _y(vec._y), _z(z)
	{
	}

	CVector3f::CVector3f(const CVector3f &vtr)
	{
		*this = vtr;
	}

	CVector3f::CVector3f(const CVector4f &vtr)
	{
		_x = vtr._x;
		_y = vtr._y;
		_z = vtr._z;
	}

	CVector3f& CVector3f::operator=(const CVector3f &vtr)
	{
		_x = vtr._x;
		_y = vtr._y;
		_z = vtr._z;
		return *this;
	}

	CVector3f& CVector3f::operator=(const CVector4f &vtr)
	{
		_x = vtr._x;
		_y = vtr._y;
		_z = vtr._z;
		return *this;
	}

	bool CVector3f::operator==(const CVector3f &vtr) const
	{
		return (_x == vtr._x &&
			_y == vtr._y &&
			_z == vtr._z);
	}

	bool CVector3f::operator!=(const CVector3f &vtr) const
	{

		return (_x != vtr._x ||
			_y != vtr._y ||
			_z != vtr._z);
	}

	CVector3f& CVector3f::operator+=(const CVector3f &vtr)
	{
		_x += vtr._x;
		_y += vtr._y;
		_z += vtr._z;
		return *this;
	}

	CVector3f& CVector3f::operator-=(const CVector3f &vtr)
	{
		_x -= vtr._x;
		_y -= vtr._y;
		_z -= vtr._z;
		return *this;
	}


	CVector3f CVector3f::operator-() const
	{
		return *this * -1;
	}

	CVector3f CVector3f::operator+(const CVector3f &vtr) const
	{
		return CVector3f(_x + vtr._x, _y + vtr._y, _z + vtr._z);
	}

	CVector3f CVector3f::operator-(const CVector3f &vtr) const
	{
		return CVector3f(_x - vtr._x, _y - vtr._y, _z - vtr._z);
	}

	CVector3f& CVector3f::operator+=(const float scalar)
	{
		*this += CVector3f(scalar, scalar, scalar);
		return *this;
	}

	CVector3f& CVector3f::operator-=(const float scalar)
	{
		*this -= CVector3f(scalar, scalar, scalar);
		return *this;
	}

	CVector3f& CVector3f::operator*=(const float scalar)
	{
		_x *= scalar;
		_y *= scalar;
		_z *= scalar;
		return *this;
	}

	CVector3f& CVector3f::operator/=(const float scalar)
	{
		//no zero check
		_x /= scalar;
		_y /= scalar;
		_z /= scalar;
		return *this;
	}

	CVector3f CVector3f::operator+(const float scalar) const
	{

		return CVector3f(_x + scalar, _y + scalar, _z + scalar);
	}

	CVector3f CVector3f::operator-(const float scalar) const
	{
		return CVector3f(_x - scalar, _y - scalar, _z - scalar);
	}

	CVector3f CVector3f::operator*(const float scalar) const
	{
		return CVector3f(_x * scalar, _y * scalar, _z * scalar);
	}

	CVector3f CVector3f::operator/(const float scalar) const
	{
		return CVector3f(_x / scalar, _y / scalar, _z / scalar);
	}


	//////////////////////////////////////////////////////////
	//4 vector
	//////////////////////////////////////////////////////////

	CVector4f::CVector4f() 
		:_x(0), _y(0), _z(0), _w(0)
	{
	}

	CVector4f::CVector4f(float x, float y, float z, float w) 
		: _x(x), _y(y), _z(z), _w(w)
	{

	}

	CVector4f::CVector4f(const CVector2f &vec, float z, float w)
		: _x(vec._x), _y(vec._y), _z(z), _w(w)
	{

	}

	CVector4f::CVector4f(const CVector3f &vec, float w)
		: _x(vec._x), _y(vec._y), _z(vec._z), _w(w)
	{

	}

	CVector4f::CVector4f(const CVector4f &vtr)
	{
		*this = vtr;
	}

	CVector4f& CVector4f::operator=(const CVector4f &vtr)
	{
		_x = vtr._x;
		_y = vtr._y;
		_z = vtr._z;
		_w = vtr._w;
		return *this;
	}

	bool CVector4f::operator==(const CVector4f &vtr) const
	{
		return (_x == vtr._x &&
			_y == vtr._y &&
			_z == vtr._z &&
			_w == vtr._w);
	}

	bool CVector4f::operator!=(const CVector4f &vtr) const
	{
		return (_x != vtr._x ||
			_y != vtr._y ||
			_z != vtr._z ||
			_w != vtr._w);
	}

	CVector4f& CVector4f::operator+=(const CVector4f &vtr)
	{
		_x += vtr._x;
		_y += vtr._y;
		_z += vtr._z;
		_w += vtr._w;
		return *this;
	}

	CVector4f& CVector4f::operator-=(const CVector4f &vtr)
	{
		_x -= vtr._x;
		_y -= vtr._y;
		_z -= vtr._z;
		_w -= vtr._w;
		return *this;
	}

	CVector4f CVector4f::operator+(const CVector4f &vtr) const
	{
		return CVector4f(_x + vtr._x, _y + vtr._y, _z + vtr._z, _w + vtr._w);
	}

	CVector4f CVector4f::operator-(const CVector4f &vtr) const
	{
		return CVector4f(_x - vtr._x, _y - vtr._y, _z - vtr._z, _w - vtr._w);
	}

	CVector4f CVector4f::operator-() const
	{
		return *this * -1;
	}

	CVector4f& CVector4f::operator+=(const float scalar)
	{
		*this += CVector4f(scalar, scalar, scalar, scalar);
		return *this;
	}

	CVector4f& CVector4f::operator-=(const float scalar)
	{
		*this -= CVector4f(scalar, scalar, scalar, scalar);
		return *this;
	}

	CVector4f& CVector4f::operator*=(const float scalar)
	{
		_x *= scalar;
		_y *= scalar;
		_z *= scalar;
		_w *= scalar;
		return *this;
	}

	CVector4f& CVector4f::operator/=(const float scalar)
	{
		//no zero check
		_x /= scalar;
		_y /= scalar;
		_z /= scalar;
		_w /= scalar;
		return *this;
	}

	CVector4f CVector4f::operator+(const float scalar) const
	{
		return CVector4f(_x + scalar, _y + scalar, _z + scalar, _w + scalar);
	}

	CVector4f CVector4f::operator-(const float scalar) const
	{
		return CVector4f(_x - scalar, _y - scalar, _z - scalar, _w - scalar);
	}

	CVector4f CVector4f::operator*(const float scalar) const
	{
		return CVector4f(_x * scalar, _y * scalar, _z * scalar, _w * scalar);
	}

	CVector4f CVector4f::operator/(const float scalar) const
	{
		return CVector4f(_x / scalar, _y / scalar, _z / scalar, _w / scalar);
	}
}