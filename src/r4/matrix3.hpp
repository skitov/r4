#pragma once

#include <algorithm>
#include <iostream>

#include <utki/debug.hpp>

#include "vector3.hpp"

namespace r4{

template <class T> class vector2;
template <class T> class vector3;

/**
 * @brief 4x4 matrix template class.
 * Note, that this matrix class stores elements in memory column by column.
 * This is the same way as OpenGL matrices are stored in memory.
 * This means easy use of this class with OpenGL.
 */
template <typename T> class matrix3{
public:
	/**
	 * @brief 0th column of the matrix.
	 */
	vector3<T> c0;

	/**
	 * @brief 1st column of the matrix.
	 */
	vector3<T> c1;

	/**
	 * @brief 2nd column of the matrix.
     */
	vector3<T> c2;

	/**
	 * @brief Default constructor.
	 * NOTE: it does not initialize the matrix with any values.
	 * Matrix elements are undefined after the matrix is created with this constructor.
	 */
	matrix3()noexcept{}

	/**
	 * @brief Construct initialized matrix.
	 * Creates a matrix and initializes its columns by the given values.
     * @param column0 - 0th column of the matrix.
	 * @param column1 - 1st column of the matrix.
	 * @param column2 - 2nd column of the matrix.
     */
	matrix3(
			const vector4<T>& column0,
			const vector4<T>& column1,
			const vector4<T>& column2
		)noexcept :
			c0(column0),
			c1(column1),
			c2(column2)
	{}

	template <class TT> matrix3(const matrix3<TT>& m) :
			c0(m.c0),
			c1(m.c1),
			c2(m.c2)
	{}

	matrix3(const matrix3&) = default;
	matrix3& operator=(const matrix3&) = default;

	/**
	 * @brief returns reference to specified column.
	 * Returns reference to the matrix column indicated by the argument.
	 * @param col - column number, must be from 0 to 2.
	 * @return reference to the matrix column indicated by the argument.
	 */
	vector3<T>& operator[](unsigned col)noexcept{
		ASSERT(col < 3)
		return (&this->c0)[col];
	}

	/**
	 * @brief returns reference to specified column.
	 * Constant variant of operator[].
	 * @param col - column number, must be from 0 to 2.
	 * @return reference to the matrix column indicated by the argument.
	 */
	const vector3<T>& operator[](unsigned col)const noexcept{
		ASSERT(col < 3)
		return (&this->c0)[col];
	}

	/**
	 * @brief Transform vector by matrix.
	 * Multiply vector V by this matrix M from the right (M * V).
	 * i.e. transform vector with this transformation matrix.
	 * @param vec - vector to transform.
     * @return Transformed vector.
     */
	vector2<T> operator*(const vector2<T>& vec)const noexcept;

	/**
	 * @brief Transform vector by matrix.
	 * Multiply vector V by this matrix M from the right (M * V).
	 * i.e. transform vector with this transformation matrix.
	 * @param vec - vector to transform.
     * @return Transformed vector.
     */
	vector3<T> operator*(const vector3<T>& vec)const noexcept;

	/**
	 * @brief Get matrix row.
	 * Constructs a vector4 holding requested row of the matrix.
	 * @param rowNum - row number to get, must be from 0 to 3.
     * @return vector4 representing the row of this matrix.
     */
	vector3<T> row(unsigned rowNum)const noexcept{
		ASSERT(rowNum < 3)
		return vector3<T>(this->c0[rowNum], this->c1[rowNum], this->c2[rowNum]);
	}

	/**
	 * @brief Multiply by matrix from the right.
	 * Calculate result of this matrix M multiplied by another matrix K from the right (M * K).
	 * @param matr - matrix to multiply by (matrix K).
     * @return New matrix as a result of matrices product.
     */
	matrix3 operator*(const matrix3& matr)const noexcept{
		return matrix3(
				vector3<T>(this->row(0) * matr[0], this->row(1) * matr[0], this->row(2) * matr[0]),
				vector3<T>(this->row(0) * matr[1], this->row(1) * matr[1], this->row(2) * matr[1]),
				vector3<T>(this->row(0) * matr[2], this->row(1) * matr[2], this->row(2) * matr[2])
			);
	}

	/**
	 * @brief Transpose matrix.
	 */
	matrix3& transpose()noexcept{
		std::swap(this->c0[1], this->c1[0]);
		std::swap(this->c0[2], this->c2[0]);

		std::swap(this->c1[2], this->c2[1]);
		return (*this);
	}

	/**
	 * @brief Multiply by matrix from the right.
	 * Multiply this matrix M by another matrix K from the right (M  = M * K).
     * @return reference to this matrix object.
     */
	matrix3& operator*=(const matrix3& matr)noexcept{
		return this->operator=(this->operator*(matr));
	}

	/**
	 * @brief Multiply by matrix from the right.
	 * Multiply this matrix M by another matrix K from the right (M  = M * K).
	 * This is the same as operator*=().
	 * @param matr - matrix to multiply by.
	 * @return reference to this matrix object.
	 */
	matrix3& right_mul(const matrix3& matr)noexcept{
		return this->operator*=(matr);
	}

	/**
	 * @brief Multiply by matrix from the left.
	 * Multiply this matrix M by another matrix K from the left (M  = K * M).
	 * @param matr - matrix to multiply by.
	 * @return reference to this matrix object.
	 */
	matrix3& left_mul(const matrix3& matr)noexcept{
		return this->operator=(matr.operator*(*this));
	}

	/**
	 * @brief Initialize this matrix with identity matrix.
	 */
	matrix3& identity()noexcept{
		this->c0 = vector3<T>(1, 0, 0);
		this->c1 = vector3<T>(0, 1, 0);
		this->c2 = vector3<T>(0, 0, 1);
		return (*this);
	}

	/**
	 * @brief Multiply current matrix by scale matrix.
	 * Multiplies this matrix M by scale matrix S from the right (M = M * S).
	 * @param x - scaling factor in x direction.
	 * @param y - scaling factor in y direction.
	 * @return reference to this matrix instance.
	 */
	matrix3& scale(T x, T y)noexcept{
		//update 0th column
		this->c0 *= x;

		//update 1st column
		this->c1 *= y;

		//NOTE: 2nd column remains unchanged
		return (*this);
	}

	/**
	 * @brief Multiply current matrix by scale matrix.
	 * Multiplies this matrix M by scale matrix S from the right (M = M * S).
	 * @param s - scaling factor to be applied in all 3 directions (x, y and z).
	 * @return reference to this matrix instance.
	 */
	matrix3& scale(T s)noexcept{
		return this->scale(s, s, s);
	}

	/**
	 * @brief Multiply current matrix by scale matrix.
	 * Multiplies this matrix M by scale matrix S from the right (M = M * S).
	 * @param s - vector of scaling factors in x and y directions, scaling factor in z direction is 1.
	 * @return reference to this matrix instance.
	 */
	matrix3& scale(const vector2<T>& s)noexcept;

	/**
	 * @brief Multiply this matrix by translation matrix.
	 * Multiplies this matrix M by translation matrix T from the right (M = M * T).
	 * @param x - x component of translation vector.
	 * @param y - y component of translation vector.
	 * @return reference to this matrix object.
	 */
	matrix3& translate(T x, T y)noexcept{
		//NOTE: 0th and 1st columns remain unchanged

		//calculate 2nd column
		this->c2 = this->c0 * x + this->c1 * y + this->c2;

		return (*this);
	}

	/**
	 * @brief Multiply this matrix by translation matrix.
	 * Multiplies this matrix M by translation matrix T from the right (M = M * T).
	 * @param t - translation vector.
	 * @return reference to this matrix object.
	 */
	matrix3& translate(const vector2<T>& t)noexcept;

	/**
	 * @brief Multiply this matrix by rotation matrix.
	 * Multiplies this matrix M by rotation matrix R from the right (M = M * R).
	 * Positive direction of rotation is counter-clockwise.
	 * @param rot - the angle of rotation in radians.
	 * @return reference to this matrix object.
	 */
	matrix3& rotate(T rot)noexcept;

	friend std::ostream& operator<<(std::ostream& s, const matrix4<T>& mat){
		s << "\n";
		s << "\t/" << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << " " << "\\" << std::endl;
		s << "\t|" << mat[0][1] << " " << mat[1][1] << " " << mat[2][1] << " " << "|" << std::endl;
		s << "\t\\" << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << " " << "/";
		return s;
	};
};

}

#include "vector2.hpp"
#include "vector3.hpp"

namespace r4{

template <class T> vector2<T> matrix3<T>::operator*(const vector2<T>& vec)const noexcept{
	return vector2<T>(
			this->row(0) * vec,
			this->row(1) * vec
		);
}

template <class T> vector3<T> matrix3<T>::operator*(const vector3<T>& vec)const noexcept{
	return vector3<T>(
			this->row(0) * vec,
			this->row(1) * vec,
			this->row(2) * vec
		);
}

template <class T> matrix3<T>& matrix3<T>::scale(const vector2<T>& s)noexcept{
	return this->scale(s.x, s.y);
}

template <class T> matrix3<T>& matrix3<T>::translate(const vector2<T>& t)noexcept{
	return this->translate(t.x, t.y);
}

typedef matrix3<float> mat3f;
static_assert(sizeof(mat3f) == sizeof(float) * 3 * 3, "size mismatch");

typedef matrix3<double> mat3d;
static_assert(sizeof(mat3d) == sizeof(double) * 3 * 3, "size mismatch");

}
