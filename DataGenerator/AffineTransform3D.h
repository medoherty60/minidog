/*
 * AffineTransform3D.h
 *      A AffineTransform3D object holds a 3D affine transformation
 *      as a 4x4 matrix.
 *
 *      The matrix is stored in a 1D array in column-major order.
 *       0  4  8 12    [0,0] [0,1] [0,2] [0,3]
 *       1  5  9 13    [1,0] [1,1] [1,2] [1,3]
 *       2  6 10 14    [2,0] [2,1] [2,2] [2,3]
 *       3  7 11 15    [3,0] [3,1] [3,2] [3,3]
 */

#ifndef AFFINETRANSFORM3D_DOT_H
#define AFFINETRANSFORM3D_DOT_H
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include "Vector.h"

static const float IDENTITY_MATRIX[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

class AffineTransform3D {

private:
	float m[16];

public:
	// Default constructor creates an identity transformation.
	AffineTransform3D()	{ setToIdentity(); }

	~AffineTransform3D() { }

	float get(int r, int c) { return m[(4*c)+r]; }
	void set(int r, int c, float val) { m[(4*c)+r]=val; }
	float* getMatrix() { return m; }

	// Set this transformation to an identity transformation.
	void setToIdentity() { memcpy(m,IDENTITY_MATRIX,16*sizeof(float)); }

	// Create an identity transformation.
	static AffineTransform3D identityTransform() {
		AffineTransform3D M;
		return M;
	}

	// Create a scaling transformation.
	static AffineTransform3D scalingTransform(float sx, float sy, float sz) {
		AffineTransform3D M;
		M.m[0] = sx; M.m[5] = sy; M.m[10] = sz;
		return M;
	}
	static AffineTransform3D scalingTransform(Vector& s) {
		return scalingTransform(s.x(), s.y(), s.z());
	}

	// Create a translation transformation.
	static AffineTransform3D translationTransform(float tx, float ty, float tz) {
		AffineTransform3D M;
		M.m[12] = tx; M.m[13] = ty; M.m[14] = tz;
		return M;
	}
	static AffineTransform3D translationTransform(Vector& t) {
		return translationTransform(t.x(), t.y(), t.z());
	}

	//	Create a rotation transformation matrix from Euler angles.
	//  Angles are expressed in radians.
	//	The order of transformations is roll(z), then pitch(x), then yaw(y).
	//	A = pitch(x), B = yaw(y), C = roll(z)
	//
	//		Ry*Rx*Rz =
	//			 cB   0  sB  |   1   0   0  |  cC -sC   0
	//			  0   1   0  |   0  cA -sA  |  sC  cC   0
	//			-sB   0  cB  |   0  sA  cA  |   0   0   1
	//		=
	//			(cBcC + sBsAsC) (-cBsC + sBsAcC) (sBcA)
	//			(cAsC)          (cAcC)           (-sA)
	//			(-sBcC + cBsAsC) (sBsC + cBsAcC) (cBcA)
	//
	//	reference: http://www.songho.ca/opengl/gl_anglestoaxes.html
	//
	static AffineTransform3D rotationTransformRPY(float pitch, float yaw, float roll)
	{
		AffineTransform3D M;
		float sA = sin(pitch);
		float cA = cos(pitch);
		float sB = sin(yaw);
		float cB = cos(yaw);
		float sC = sin(roll);
		float cC = cos(roll);

		M.m[0]  = cB*cC + sB*sA*sC;
		M.m[1]  = cA*sC;
		M.m[2]  = cB*sA*sC - sB*cC;
		M.m[3]  = 0.0f;

		M.m[4]  = sB*sA*cC - cB*sC;
		M.m[5]  = cA*cC;
		M.m[6]  = sB*sC + cB*sA*cC;
		M.m[7] = 0.0f;

		M.m[8]  = sB*cA;
		M.m[9]  = -sA;
		M.m[10] = cB*cA;
		M.m[11] = 0.0f;

		M.m[12] = 0.0f;
		M.m[13] = 0.0f;
		M.m[14] = 0.0f;
		M.m[15] = 1.0f;
		return M;
	}
	static AffineTransform3D rotationTransformRPY(Vector& r) {
		return rotationTransformRPY(r.x(), r.y(), r.z());
	}

	// Transform a point <x,y,z> by applying this transformation to <x,y,z,1>.
	Vector transformPoint(Vector& p1)
	{
		float x = p1.x()*m[0]  + p1.y()*m[4]  + p1.z()*m[8]  + m[12];
		float y = p1.x()*m[1]  + p1.y()*m[5]  + p1.z()*m[9]  + m[13];
		float z = p1.x()*m[2]  + p1.y()*m[6]  + p1.z()*m[10] + m[14];
		float w = p1.x()*m[3]  + p1.y()*m[7]  + p1.z()*m[11] + m[15];
		Vector p2(x/w,y/w,z/w);
		return p2;
	}

	// Transform a vector <x,y,z> by applying this transformation to <x,y,z,0>.
	Vector transformVector(Vector& v1)
	{
		float x = v1.x()*m[0]  + v1.y()*m[4]  + v1.z()*m[8];
		float y = v1.x()*m[1]  + v1.y()*m[5]  + v1.z()*m[9];
		float z = v1.x()*m[2]  + v1.y()*m[6]  + v1.z()*m[10];
		Vector v2(x,y,z);
		return v2;
	}

	// Combine (multiply) this transformation with another transformation (to the right)
	// and return the resulting transformation.
	AffineTransform3D operator*(const AffineTransform3D& rhs)
	{
		AffineTransform3D M;
		M.m[0]  = m[0]*rhs.m[0]  + m[4]*rhs.m[1]  + m[8]*rhs.m[2]   + m[12]*rhs.m[3];
		M.m[1]  = m[1]*rhs.m[0]  + m[5]*rhs.m[1]  + m[9]*rhs.m[2]   + m[13]*rhs.m[3];
		M.m[2]  = m[2]*rhs.m[0]  + m[6]*rhs.m[1]  + m[10]*rhs.m[2]  + m[14]*rhs.m[3];
		M.m[3]  = m[3]*rhs.m[0]  + m[7]*rhs.m[1]  + m[11]*rhs.m[2]  + m[15]*rhs.m[3];

		M.m[4]  = m[0]*rhs.m[4]  + m[4]*rhs.m[5]  + m[8]*rhs.m[6]   + m[12]*rhs.m[7];
		M.m[5]  = m[1]*rhs.m[4]  + m[5]*rhs.m[5]  + m[9]*rhs.m[6]   + m[13]*rhs.m[7];
		M.m[6]  = m[2]*rhs.m[4]  + m[6]*rhs.m[5]  + m[10]*rhs.m[6]  + m[14]*rhs.m[7];
		M.m[7]  = m[3]*rhs.m[4]  + m[7]*rhs.m[5]  + m[11]*rhs.m[6]  + m[15]*rhs.m[7];

		M.m[8]  = m[0]*rhs.m[8]  + m[4]*rhs.m[9]  + m[8]*rhs.m[10]  + m[12]*rhs.m[11];
		M.m[9]  = m[1]*rhs.m[8]  + m[5]*rhs.m[9]  + m[9]*rhs.m[10]  + m[13]*rhs.m[11];
		M.m[10] = m[2]*rhs.m[8]  + m[6]*rhs.m[9]  + m[10]*rhs.m[10] + m[14]*rhs.m[11];
		M.m[11] = m[3]*rhs.m[8]  + m[7]*rhs.m[9]  + m[11]*rhs.m[10] + m[15]*rhs.m[11];

		M.m[12] = m[0]*rhs.m[12] + m[4]*rhs.m[13] + m[8]*rhs.m[14]  + m[12]*rhs.m[15];
		M.m[13] = m[1]*rhs.m[12] + m[5]*rhs.m[13] + m[9]*rhs.m[14]  + m[13]*rhs.m[15];
		M.m[14] = m[2]*rhs.m[12] + m[6]*rhs.m[13] + m[10]*rhs.m[14] + m[14]*rhs.m[15];
		M.m[15] = m[3]*rhs.m[12] + m[7]*rhs.m[13] + m[11]*rhs.m[14] + m[15]*rhs.m[15];
		return M;
	}

	// Create the inverse of this transformation.
	// Adapted from code supplied by Anders Gustafsson at
	// http://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform
	AffineTransform3D inverse()
	{
		AffineTransform3D m_inv;

    	float s0 = m[ 0] * m[ 5] - m[ 4] * m[ 1];
    	float s1 = m[ 0] * m[ 6] - m[ 4] * m[ 2];
    	float s2 = m[ 0] * m[ 7] - m[ 4] * m[ 3];
    	float s3 = m[ 1] * m[ 6] - m[ 5] * m[ 2];
    	float s4 = m[ 1] * m[ 7] - m[ 5] * m[ 3];
    	float s5 = m[ 2] * m[ 7] - m[ 6] * m[ 3];

    	float c5 = m[10] * m[15] - m[14] * m[11];
    	float c4 = m[ 9] * m[15] - m[13] * m[11];
    	float c3 = m[ 9] * m[14] - m[13] * m[10];
    	float c2 = m[ 8] * m[15] - m[12] * m[11];
    	float c1 = m[ 8] * m[14] - m[12] * m[10];
    	float c0 = m[ 8] * m[13] - m[12] * m[ 9];

    	// Should check for 0 determinant
    	float det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
    	if (det == 0.0f) {
    		cerr << "Attempt to divide by a zero determinant in AffineTransform3D::inverse()" << endl;
    		det = 1.0f;
    	}
    	float invdet = 1.0 / det;

    	m_inv.m[ 0] = ( m[ 5] * c5 - m[ 6] * c4 + m[ 7] * c3) * invdet;
    	m_inv.m[ 1] = (-m[ 1] * c5 + m[ 2] * c4 - m[ 3] * c3) * invdet;
    	m_inv.m[ 2] = ( m[13] * s5 - m[14] * s4 + m[15] * s3) * invdet;
    	m_inv.m[ 3] = (-m[ 9] * s5 + m[10] * s4 - m[11] * s3) * invdet;

    	m_inv.m[ 4] = (-m[ 4] * c5 + m[ 6] * c2 - m[ 7] * c1) * invdet;
    	m_inv.m[ 5] = ( m[ 0] * c5 - m[ 2] * c2 + m[ 3] * c1) * invdet;
    	m_inv.m[ 6] = (-m[12] * s5 + m[14] * s2 - m[15] * s1) * invdet;
    	m_inv.m[ 7] = ( m[ 8] * s5 - m[10] * s2 + m[11] * s1) * invdet;

    	m_inv.m[ 8] = ( m[ 4] * c4 - m[ 5] * c2 + m[ 7] * c0) * invdet;
    	m_inv.m[ 9] = (-m[ 0] * c4 + m[ 1] * c2 - m[ 3] * c0) * invdet;
    	m_inv.m[10] = ( m[12] * s4 - m[13] * s2 + m[15] * s0) * invdet;
    	m_inv.m[11] = (-m[ 8] * s4 + m[ 9] * s2 - m[11] * s0) * invdet;

    	m_inv.m[12] = (-m[ 4] * c3 + m[ 5] * c1 - m[ 6] * c0) * invdet;
    	m_inv.m[13] = ( m[ 0] * c3 - m[ 1] * c1 + m[ 2] * c0) * invdet;
    	m_inv.m[14] = (-m[12] * s3 + m[13] * s1 - m[14] * s0) * invdet;
    	m_inv.m[15] = ( m[ 8] * s3 - m[ 9] * s1 + m[10] * s0) * invdet;

    	return m_inv;
	}

	/*
	// Code supplied by Anders Gustafsson at
	// http://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform
	// Gustafsson's indexes were [col,row]
	public static double[,] GetInverse(double[,] a)
	{
	    var s0 = a[0, 0] * a[1, 1] - a[1, 0] * a[0, 1];
	    var s1 = a[0, 0] * a[1, 2] - a[1, 0] * a[0, 2];
	    var s2 = a[0, 0] * a[1, 3] - a[1, 0] * a[0, 3];
	    var s3 = a[0, 1] * a[1, 2] - a[1, 1] * a[0, 2];
	    var s4 = a[0, 1] * a[1, 3] - a[1, 1] * a[0, 3];
	    var s5 = a[0, 2] * a[1, 3] - a[1, 2] * a[0, 3];

	    var c5 = a[2, 2] * a[3, 3] - a[3, 2] * a[2, 3];
	    var c4 = a[2, 1] * a[3, 3] - a[3, 1] * a[2, 3];
	    var c3 = a[2, 1] * a[3, 2] - a[3, 1] * a[2, 2];
	    var c2 = a[2, 0] * a[3, 3] - a[3, 0] * a[2, 3];
	    var c1 = a[2, 0] * a[3, 2] - a[3, 0] * a[2, 2];
	    var c0 = a[2, 0] * a[3, 1] - a[3, 0] * a[2, 1];

	    // Should check for 0 determinant
	    var invdet = 1.0 / (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);

	    var b = new double[4, 4];

	    b[0, 0] = ( a[1, 1] * c5 - a[1, 2] * c4 + a[1, 3] * c3) * invdet;
	    b[0, 1] = (-a[0, 1] * c5 + a[0, 2] * c4 - a[0, 3] * c3) * invdet;
	    b[0, 2] = ( a[3, 1] * s5 - a[3, 2] * s4 + a[3, 3] * s3) * invdet;
	    b[0, 3] = (-a[2, 1] * s5 + a[2, 2] * s4 - a[2, 3] * s3) * invdet;

	    b[1, 0] = (-a[1, 0] * c5 + a[1, 2] * c2 - a[1, 3] * c1) * invdet;
	    b[1, 1] = ( a[0, 0] * c5 - a[0, 2] * c2 + a[0, 3] * c1) * invdet;
	    b[1, 2] = (-a[3, 0] * s5 + a[3, 2] * s2 - a[3, 3] * s1) * invdet;
	    b[1, 3] = ( a[2, 0] * s5 - a[2, 2] * s2 + a[2, 3] * s1) * invdet;

	    b[2, 0] = ( a[1, 0] * c4 - a[1, 1] * c2 + a[1, 3] * c0) * invdet;
	    b[2, 1] = (-a[0, 0] * c4 + a[0, 1] * c2 - a[0, 3] * c0) * invdet;
	    b[2, 2] = ( a[3, 0] * s4 - a[3, 1] * s2 + a[3, 3] * s0) * invdet;
	    b[2, 3] = (-a[2, 0] * s4 + a[2, 1] * s2 - a[2, 3] * s0) * invdet;

	    b[3, 0] = (-a[1, 0] * c3 + a[1, 1] * c1 - a[1, 2] * c0) * invdet;
	    b[3, 1] = ( a[0, 0] * c3 - a[0, 1] * c1 + a[0, 2] * c0) * invdet;
	    b[3, 2] = (-a[3, 0] * s3 + a[3, 1] * s1 - a[3, 2] * s0) * invdet;
	    b[3, 3] = ( a[2, 0] * s3 - a[2, 1] * s1 + a[2, 2] * s0) * invdet;

	    return b;
	}
	*/

	// print this transformation
	void print(){
		cout<<"Affine Transformation:"<<endl;
		for (int row=0; row<4; row++) {
			for (int col=0; col<4; col++)
				cout << setw(10) << get(row,col);
			cout << endl;
		}
	}
};

#endif /* AFFINETRANSFORM3D_DOT_H */
