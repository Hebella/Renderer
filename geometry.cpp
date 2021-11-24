#include "geometry.h"
#include <cassert>

using namespace std;

Matrix::Matrix(int r, int c): _rows(r), _cols(c)
{
	_m = vector<vector<float>>(r, vector<float>(c, 0.0f));
}

inline int Matrix::nrows()
{
	return _rows;
}

inline int Matrix::nlines()
{
	return _cols;
}

Matrix Matrix::identity(int dims)
{
	Matrix m(dims, dims);
	for (int i = 0; i < dims; ++i)
		m[i][i] = 1.0f;
	return m;
}

vector<float>& Matrix::operator[](int i)
{
	assert(i >= 0 && i < _rows);
	return _m[i];
}

Matrix Matrix::operator*(const Matrix& a)
{
	assert(_cols == a._rows);
	Matrix res(_rows, a._cols);
	for (int i = 0; i < _rows; ++i)
	{
		for (int j = 0; j < a._cols; ++j)
		{
			res[i][j] = 0.0f;
			for (int k = 0; k < _cols; ++k)
			{
				res[i][j] += _m[i][k] * a._m[k][j];
			}
		}
	}
	return res;
}

Matrix Matrix::transpose()
{
	Matrix res(_cols, _rows);
	for (int i = 0; i < _rows; ++i)
	{
		for (int j = 0; j < _cols; ++j)
			res[j][i] = _m[i][j];
	}
	return res;
}

Matrix Matrix::inverse()
{
	Matrix result(_rows, _cols * 2);
	for (int i = 0; i < _rows; ++i)
	{
		for (int j = 0; j < _cols; ++j)
		{
			result[i][j] = _m[i][j];
		}
	}
	for (int i = 0; i < _cols; ++i)
		result[i][i + _cols] = 1;

	for (int i = 0; i < _rows - 1; ++i)
	{
		for (int j = result._cols - 1; j >= 0; --j)
			result[i][j] /= result[i][i];
		for (int k = i + 1; k < _rows; ++k)
		{
			float coeff = result[k][i]; // 每个元素都减去coeff*第i行同一列的元素
			for (int j = 0; j < result._cols; ++j)
				result[k][j] -= result[i][j] * coeff;
		}
	}

	for (int j = result._cols - 1; j >= _cols - 1; --j)
		result[_rows - 1][j] /= result[_rows - 1][_rows - 1];
	for (int i = _rows - 1; i > 0; --i)
	{
		for (int k = i - 1; k >= 0; --k)
		{
			float coeff = result[k][i]; // 每个元素都减去coeff*第i行同一列的元素
			for (int j = 0; j < result._cols; ++j)
				result[k][j] -= result[i][j] * coeff;
		}
	}

	Matrix res(_rows, _cols);
	for (int i = 0; i < _rows; ++i)
	{
		for (int j = 0; j < _cols; ++j)
			res[i][j] = result[i][j + _cols];
	}
	return res;
}





