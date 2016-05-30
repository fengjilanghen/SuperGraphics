#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Vector.h"
#include <cassert>
#include <iostream>

namespace DiyMath3D
{
	//3*3 matrix
	class CMatrix33f;
	//4*4 matrix
	class CMatrix44f;
}

class DiyMath3D::CMatrix33f
{
public:
	CMatrix33f();
	CMatrix33f(const CMatrix33f& mat);
	explicit CMatrix33f(const CMatrix44f& mat);

	CMatrix33f& loadIdentity();

	CMatrix33f& operator=(const CMatrix33f& mat33);

	float& operator()(int row, int col);

	float operator()(int row, int col) const;

private:
	float _matrix[3][3];
};

class DiyMath3D::CMatrix44f
{
public:
	CMatrix44f();
	CMatrix44f(const CMatrix44f& mat);

	CMatrix44f& loadIdentity();

	CMatrix44f& operator=(const CMatrix44f& mat);

	float& operator()(int row, int col);

	float operator()(int row, int col) const;

private:
	float _matrix[4][4];
};

#endif //_MATRIX_H_