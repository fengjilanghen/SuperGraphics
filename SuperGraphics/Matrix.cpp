#include "Matrix.h"

namespace DiyMath3D
{

	//////////////////////////////////////////////////////////
	//3*3 matrix
	//////////////////////////////////////////////////////////
	CMatrix33f::CMatrix33f()
	{
		loadIdentity();
	}
	CMatrix33f::CMatrix33f(const CMatrix33f& mat)
	{
		memcpy(_matrix, mat._matrix, sizeof(_matrix));
	}

	CMatrix33f::CMatrix33f(const CMatrix44f& mat)
	{
		_matrix[0][0] = mat(0, 0);
		_matrix[0][1] = mat(0, 1);
		_matrix[0][2] = mat(0, 2);
		_matrix[1][0] = mat(1, 0);
		_matrix[1][1] = mat(1, 1);
		_matrix[1][2] = mat(1, 2);
		_matrix[2][0] = mat(2, 0);
		_matrix[2][1] = mat(2, 1);
		_matrix[2][2] = mat(2, 2);
	}
	CMatrix33f& CMatrix33f::loadIdentity()
	{
		float identity[3][3] = { 1.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 1.0f };
		memcpy(_matrix, identity, sizeof(_matrix));
		return *this;
	}

	CMatrix33f& CMatrix33f::operator=(const CMatrix33f& mat33)
	{
		memcpy(_matrix, mat33._matrix, sizeof(_matrix));
		return *this;
	}

	float& CMatrix33f::operator()(int row, int col)
	{
		assert(row >= 0 && row < 3);
		assert(col >= 0 && col < 3);
		return _matrix[row][col];
	}

	float CMatrix33f::operator()(int row, int col) const
	{
		assert(row >= 0 && row < 3);
		assert(col >= 0 && col < 3);
		return _matrix[row][col];
	}

	//////////////////////////////////////////////////////////
	//4*4 matrix
	//////////////////////////////////////////////////////////
	CMatrix44f::CMatrix44f()
	{
		loadIdentity();
	}

	CMatrix44f::CMatrix44f(const CMatrix44f& mat33)
	{
		memcpy(_matrix, mat33._matrix, sizeof(_matrix));
	}

	CMatrix44f& CMatrix44f::loadIdentity()
	{
		float identity[4][4] = { 1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f };
		memcpy(_matrix, identity, sizeof(_matrix));
		return *this;
	}

	CMatrix44f& CMatrix44f::operator=(const CMatrix44f& mat)
	{
		memcpy(_matrix, mat._matrix, sizeof(_matrix));
		return *this;
	}

	float& CMatrix44f::operator()(int row, int col)
	{
		assert(row >= 0 && row < 4);
		assert(col >= 0 && col < 4);
		return _matrix[row][col];
	}

	float CMatrix44f::operator()(int row, int col) const
	{
		assert(row >= 0 && row < 4);
		assert(col >= 0 && col < 4);
		return _matrix[row][col];
	}
}