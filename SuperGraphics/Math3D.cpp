#include "Math3D.h"

namespace DiyMath3D
{

	////////////////////////////////////////////////////////
	//caculate length of vector
	////////////////////////////////////////////////////////
	float Length(const CVector2f& vec)
	{
		return sqrt(vec._x*vec._x + vec._y*vec._y);
	}

	float Length(const CVector3f& vec)
	{
		return sqrt(vec._x*vec._x + vec._y*vec._y + vec._z*vec._z);
	}

	float Length(const CVector4f& vec)
	{
		return sqrt(vec._x*vec._x + vec._y*vec._y + vec._z*vec._z);
	}

	//normalize a vector
	CVector3f Normalize(const CVector3f &vtr)
	{
		float length = Length(vtr);
		return CVector3f(vtr._x / length, vtr._y / length, vtr._z / length);
	}

	//vector * vector
	float operator*(const CVector3f &left, const CVector3f &right)
	{
		return left._x*right._x + left._y*right._y + left._z*right._z;
	}

	//vector ¡Á vector
	CVector3f CrossProduct(const CVector3f &left, const CVector3f &right)
	{
		float x = left._y*right._z - left._z*right._y;
		float y = left._z*right._x - left._x*right._z;
		float z = left._x*right._y - left._y*right._x;
		return CVector3f(x, y, z);
	}

	//////////////////////////////////////////////////////////
	//matrix * matrix
	//////////////////////////////////////////////////////////
	CMatrix33f operator*(const CMatrix33f& left, const CMatrix33f& right)
	{
		CMatrix33f tmp;
		tmp(0, 0) = left(0, 0) * right(0, 0) + left(0, 1)*right(1, 0) + left(0, 2)*right(2, 0);
		tmp(0, 1) = left(0, 0) * right(0, 1) + left(0, 1)*right(1, 1) + left(0, 2)*right(2, 1);
		tmp(0, 2) = left(0, 0) * right(0, 2) + left(0, 1)*right(1, 2) + left(0, 2)*right(2, 2);

		tmp(1, 0) = left(1, 0) * right(0, 0) + left(1, 1)*right(1, 0) + left(1, 2)*right(2, 0);
		tmp(1, 1) = left(1, 0) * right(0, 1) + left(1, 1)*right(1, 1) + left(1, 2)*right(2, 1);
		tmp(1, 2) = left(1, 0) * right(0, 2) + left(1, 1)*right(1, 2) + left(1, 2)*right(2, 2);

		tmp(2, 0) = left(2, 0) * right(0, 0) + left(2, 1)*right(1, 0) + left(2, 2)*right(2, 0);
		tmp(2, 1) = left(2, 0) * right(0, 1) + left(2, 1)*right(1, 1) + left(2, 2)*right(2, 1);
		tmp(2, 2) = left(2, 0) * right(0, 2) + left(2, 1)*right(1, 2) + left(2, 2)*right(2, 2);

		tmp(3, 0) = left(3, 0) * right(0, 0) + left(3, 1)*right(1, 0) + left(3, 2)*right(2, 0);
		tmp(3, 1) = left(3, 0) * right(0, 1) + left(3, 1)*right(1, 1) + left(3, 2)*right(2, 1);
		tmp(3, 2) = left(3, 0) * right(0, 2) + left(3, 1)*right(1, 2) + left(3, 2)*right(2, 2);
		return tmp;
	}

	CMatrix44f operator*(const CMatrix44f& left, const CMatrix44f& right)
	{
		CMatrix44f tmp;
		tmp(0, 0) = left(0, 0) * right(0, 0) + left(0, 1)*right(1, 0) + left(0, 2)*right(2, 0) + left(0, 3)*right(3, 0);
		tmp(0, 1) = left(0, 0) * right(0, 1) + left(0, 1)*right(1, 1) + left(0, 2)*right(2, 1) + left(0, 3)*right(3, 1);
		tmp(0, 2) = left(0, 0) * right(0, 2) + left(0, 1)*right(1, 2) + left(0, 2)*right(2, 2) + left(0, 3)*right(3, 2);
		tmp(0, 3) = left(0, 0) * right(0, 3) + left(0, 1)*right(1, 3) + left(0, 2)*right(2, 3) + left(0, 3)*right(3, 3);

		tmp(1, 0) = left(1, 0) * right(0, 0) + left(1, 1)*right(1, 0) + left(1, 2)*right(2, 0) + left(1, 3)*right(3, 0);
		tmp(1, 1) = left(1, 0) * right(0, 1) + left(1, 1)*right(1, 1) + left(1, 2)*right(2, 1) + left(1, 3)*right(3, 1);
		tmp(1, 2) = left(1, 0) * right(0, 2) + left(1, 1)*right(1, 2) + left(1, 2)*right(2, 2) + left(1, 3)*right(3, 2);
		tmp(1, 3) = left(1, 0) * right(0, 3) + left(1, 1)*right(1, 3) + left(1, 2)*right(2, 3) + left(1, 3)*right(3, 3);

		tmp(2, 0) = left(2, 0) * right(0, 0) + left(2, 1)*right(1, 0) + left(2, 2)*right(2, 0) + left(2, 3)*right(3, 0);
		tmp(2, 1) = left(2, 0) * right(0, 1) + left(2, 1)*right(1, 1) + left(2, 2)*right(2, 1) + left(2, 3)*right(3, 1);
		tmp(2, 2) = left(2, 0) * right(0, 2) + left(2, 1)*right(1, 2) + left(2, 2)*right(2, 2) + left(2, 3)*right(3, 2);
		tmp(2, 3) = left(2, 0) * right(0, 3) + left(2, 1)*right(1, 3) + left(2, 2)*right(2, 3) + left(2, 3)*right(3, 3);

		tmp(3, 0) = left(3, 0) * right(0, 0) + left(3, 1)*right(1, 0) + left(3, 2)*right(2, 0) + left(3, 3)*right(3, 0);
		tmp(3, 1) = left(3, 0) * right(0, 1) + left(3, 1)*right(1, 1) + left(3, 2)*right(2, 1) + left(3, 3)*right(3, 1);
		tmp(3, 2) = left(3, 0) * right(0, 2) + left(3, 1)*right(1, 2) + left(3, 2)*right(2, 2) + left(3, 3)*right(3, 2);
		tmp(3, 3) = left(3, 0) * right(0, 3) + left(3, 1)*right(1, 3) + left(3, 2)*right(2, 3) + left(3, 3)*right(3, 3);
		return tmp;
	}

	////////////////////////////////////////////////////////
	//vector * matrix
	////////////////////////////////////////////////////////
	CVector3f operator*(const CVector3f &coords, const CMatrix33f& right)
	{
		CVector3f tmp;
		tmp._x = coords._x * right(0, 0) + coords._y*right(1, 0) + coords._z*right(2, 0);
		tmp._y = coords._x * right(0, 1) + coords._y*right(1, 1) + coords._z*right(2, 1);
		tmp._z = coords._x * right(0, 2) + coords._y*right(1, 2) + coords._z*right(2, 2);
		return tmp;
	}

	CVector4f operator*(const CVector4f &coords, const CMatrix44f& right)
	{
		CVector4f tmp;
		tmp._x = coords._x * right(0, 0) + coords._y*right(1, 0) + coords._z*right(2, 0) + coords._w*right(3, 0);
		tmp._y = coords._x * right(0, 1) + coords._y*right(1, 1) + coords._z*right(2, 1) + coords._w*right(3, 1);
		tmp._z = coords._x * right(0, 2) + coords._y*right(1, 2) + coords._z*right(2, 2) + coords._w*right(3, 2);
		tmp._w = coords._x * right(0, 3) + coords._y*right(1, 3) + coords._z*right(2, 3) + coords._w*right(3, 3);
		return tmp;
	}


	////////////////////////////////////////////////////////
	//base transforms
	////////////////////////////////////////////////////////
	CMatrix44f Translate(const CMatrix44f& mat, const CVector3f& trans)
	{
		CMatrix44f tmp;
		tmp(3, 0) = trans._x;
		tmp(3, 1) = trans._y;
		tmp(3, 2) = trans._z;

		return mat*tmp;
	}

	CMatrix44f Rotate(const CMatrix44f& mat, const CVector3f &axis, float theta)
	{
		CVector3f naxis = Normalize(axis);
		float a = naxis._x;
		float b = naxis._y;
		float c = naxis._z;
		float costheta = cos(Rad(theta));
		float sintheta = sin(Rad(theta));
		float onecostheta = (1 - costheta);
		float act = a*onecostheta;
		float bct = b*onecostheta;
		float ast = a*sintheta;
		float bst = b*sintheta;
		float cst = c*sintheta;

		CMatrix44f tmp;
		tmp(0, 0) = a*act + costheta;
		tmp(0, 1) = a*bct + cst;
		tmp(0, 2) = c*act - bst;
		tmp(1, 0) = a*bct - cst;
		tmp(1, 1) = b*bct + costheta;
		tmp(1, 2) = c*bct + ast;
		tmp(2, 0) = c*act + bst;
		tmp(2, 1) = c*bct - ast;
		tmp(2, 2) = c*c*onecostheta + costheta;

		return mat*tmp;
	}

	CMatrix44f Scale(const CMatrix44f& mat, const CVector3f &scale)
	{
		CMatrix44f tmp;
		tmp(0, 0) = scale._x;
		tmp(1, 1) = scale._y;
		tmp(2, 2) = scale._z;

		return mat*tmp;
	}

	//////////////////////////////////////////////////////////
	//view transform function
	//////////////////////////////////////////////////////////
	CMatrix44f LookAt(const CVector3f& pos, const CVector3f& lookat, const CVector3f& up /*= CVector3f(0, 1, 0)*/)
	{
		CVector3f n = Normalize(lookat - pos);
		CVector3f u = Normalize(CrossProduct(up, n));
		CVector3f v = Normalize(CrossProduct(n, u));

		CMatrix44f tmp;
		tmp(0, 0) = u._x;
		tmp(1, 0) = u._y;
		tmp(2, 0) = u._z;
		tmp(3, 0) = -pos*u;
		tmp(0, 1) = v._x;
		tmp(1, 1) = v._y;
		tmp(2, 1) = v._z;
		tmp(3, 1) = -pos*v;
		tmp(0, 2) = n._x;
		tmp(1, 2) = n._y;
		tmp(2, 2) = n._z;
		tmp(3, 2) = -pos*n;
		return tmp;
	}

	//////////////////////////////////////////////////////////
	//perspective transform function
	//////////////////////////////////////////////////////////
	CMatrix44f Perspective(float fov, float zn, float zf, float aspect /*= 1.0f*/)
	{
		CMatrix44f mat;
		float cothalffov = 1.f / tan(Rad(fov / 2));
		mat(0, 0) = cothalffov / aspect;
		mat(1, 1) = cothalffov;
		mat(2, 2) = zf / (zf - zn);
		mat(2, 3) = 1;
		mat(3, 2) = zf*zn / (zn - zf);
		mat(3, 3) = 0.0f;
		return mat;
	}

	CVector4f PerspectiveDevide(const CVector4f& vec)
	{
		float w = (vec._w == 0.0f ? 1.0f : vec._w);
		return CVector4f(vec._x / w, vec._y / w, vec._z / w, vec._w / w);
	}

	//////////////////////////////////////////////////////////
	//viewport transform function
	//////////////////////////////////////////////////////////
	CMatrix44f ViewportTransform(int x, int y, int w, int h)
	{
		CMatrix44f mat;
		//mat(0, 0) = 0.5f * w;
		//mat(0, 2) = xmin + 0.5f * w;
		//mat(1, 1) = 0.5f * h;
		//mat(1, 2) = ymin + 0.5f * h;
		//mat(2, 2) = 1.0f;
		//mat(2, 3) = 1.0f;
		//mat(3, 0) = w/2;
		//mat(3, 1) = -h/2;
		float halfw = 0.5f*w;
		float halfh = 0.5f*h;
		mat(0, 0) = halfw;
		mat(1, 1) = -halfh;
		mat(3, 0) = halfw + x;
		mat(3, 1) = halfh + y;
		return mat;
	}

	//colordot
	CVector3f ColorDot(const CVector3f& color1, const CVector3f& color2)
	{
		return CVector3f(color1._x*color2._x, color1._y*color2._y, color1._z*color2._z);
	}
}