//#ifndef _MATH3D_H_
//#define _MATH3D_H_
#pragma once

#include "Matrix.h"
#include <cmath>

namespace DiyMath3D
{
	const float PI = 3.141592;

	inline float Rad(float anger){ return PI*anger / 180.f; }
	////////////////////////////////////////////////////////
	//caculate length of vector
	////////////////////////////////////////////////////////
	float Length(const CVector2f& vec);

	float Length(const CVector3f& vec);
	
	float Length(const CVector4f& vec);
	 
	//normalize a vector
	CVector3f Normalize(const CVector3f &vtr);

	//vector * vector
	float operator*(const CVector3f &left, const CVector3f &right);

	//vector ¡Á vector
	CVector3f CrossProduct(const CVector3f &left, const CVector3f &right);

	//////////////////////////////////////////////////////////
	//matrix * matrix
	//////////////////////////////////////////////////////////
	CMatrix33f operator*(const CMatrix33f& left, const CMatrix33f& right);
	CMatrix44f operator*(const CMatrix44f& left, const CMatrix44f& right);

	////////////////////////////////////////////////////////
	//vector * matrix
	////////////////////////////////////////////////////////
	CVector3f operator*(const CVector3f &coords, const CMatrix33f& right);
	CVector4f operator*(const CVector4f &coords, const CMatrix44f& right);


	////////////////////////////////////////////////////////
	//base transforms
	////////////////////////////////////////////////////////
	CMatrix44f Translate(const CMatrix44f& mat, const CVector3f& trans);

	CMatrix44f Rotate(const CMatrix44f& mat, const CVector3f &axis, float theta);

	CMatrix44f Scale(const CMatrix44f& mat, const CVector3f &scale);

	
	//view transform function
	CMatrix44f LookAt(const CVector3f& pos, const CVector3f& lookat, const CVector3f& up = CVector3f(0, 1, 0));

	//perspective transform function
	CMatrix44f Perspective(float fov, float zn, float zf, float aspect = 1.0f);

	//perspective devide function
	CVector4f PerspectiveDevide(const CVector4f& vec);

	//viewport transform function
	CMatrix44f ViewportTransform(int xmin, int ymin, int w, int h);

	//colordot
	CVector3f ColorDot(const CVector3f& color1, const CVector3f& color2);
}

//#endif //_MATH3D_H_