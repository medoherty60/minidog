#ifndef IMPLICITSURFACE_H_
#define IMPLICITSURFACE_H_

#include <vector>
using namespace std;
#include "Vector.h"
#include "AffineTransform3D.h"

//===============================================

class ImplicitSurface {
public:

	ImplicitSurface(float tx=0.0f, float ty=0.0f, float tz=0.0f,
			float rx=0.0f, float ry=0.0f, float rz=0.0f,
			float sx=1.0f, float sy=1.0f, float sz=1.0f)
	{
		Vector t(tx,ty,tz);
		Vector r(rx,ry,rz);
		Vector s(sx,sy,sz);
		buildTransformation(t,r,s);
	}

	ImplicitSurface(Vector& t, Vector& r, Vector& s)
	{
		buildTransformation(t,r,s);
	}

	// solidObject() should return 50 if (x,y,z) is inside the surface
	// and -100 if it is outside.
	virtual short solidObject(short x, short y, short z) = 0;

	// distanceField() should return the smallest distance from (x,y,z) to
	// the surface. Distances inside the surface should be negative and
	// distances outside the surface should be positive.
	virtual short distanceField(short x, short y, short z) = 0;

	// directedDistanceField() should return a vector from (x,y,z) to
	// the nearest point on the surface.
	virtual void directedDistanceField(short x, short y, short z,
									   float& dist_x, float& dist_y, float& dist_z) = 0;

protected:
	AffineTransform3D transform;

	Vector applyTransform(Vector& v1) {
		Vector v2 = transform.transformPoint(v1);
		return v2;
	}

private:
	void buildTransformation(Vector& t, Vector& r, Vector& s)
	{
		// This could be much more efficient, since the inverse transforms
		// could be computed more directly.
		AffineTransform3D tt = AffineTransform3D::translationTransform(t);
		AffineTransform3D tr = AffineTransform3D::rotationTransformRPY(r);
		AffineTransform3D ts = AffineTransform3D::scalingTransform(s);
		AffineTransform3D itt = tt.inverse();
		AffineTransform3D itr = tr.inverse();
		AffineTransform3D its = ts.inverse();
		transform = (its*itr)*itt;
	}
};

//===============================================

class Sphere : public ImplicitSurface {
public:

	Sphere(float tx=0.0f, float ty=0.0f, float tz=0.0f,
			float rx=0.0f, float ry=0.0f, float rz=0.0f,
			float sx=1.0f, float sy=1.0f, float sz=1.0f) :
	ImplicitSurface(tx,ty,tz, rx,ry,rz, sx,sy,sz), radius(1.0f)
	{ radius_sq = radius*radius; }

	Sphere(Vector& t, Vector& r, Vector& s) :
	ImplicitSurface(t,r,s), radius(1.0f)
	{ radius_sq = radius*radius; }

	virtual short solidObject(short x, short y, short z) {
		Vector v_in = Vector(x,y,z);
		Vector v = applyTransform(v_in);
		float i = v.x();
		float j = v.y();
		float k = v.z();

		float d_ijk = 50.0f;
		//compute the magnitude from the origin to the point ijk
		if (sqrt(i*i+j*j+k*k) > radius) d_ijk = -100.0f;		//Air
		return short(d_ijk);
	}

	virtual short distanceField(short x, short y, short z) {
		Vector v_in = Vector(x,y,z);
		Vector v = applyTransform(v_in);
		float i = v.x();
		float j = v.y();
		float k = v.z();

		//each point is assigned with a scalar distance d_ijk from surface
		float d_ijk = sqrt(i*i+j*j+k*k) - radius;
		return short(d_ijk);
	}

	virtual void directedDistanceField(short x, short y, short z,
									   float& dist_x, float& dist_y, float& dist_z) {
		Vector v_in = Vector(x,y,z);
		Vector v = applyTransform(v_in);
		float i = v.x();
		float j = v.y();
		float k = v.z();

		//each point is assigned with three direct distances from surface
		dist_x = i - radius;
		dist_y = j - radius;
		dist_z = k - radius;
	}

	// Not tested.
	short f_sphere2(long x, long y, long z) {
		Vector v_in = Vector(x,y,z);
		Vector v = applyTransform(v_in);
		float i = v.x();
		float j = v.y();
		float k = v.z();

	    float d_ijk = radius_sq - i*i - j*j - k*k;
		if (d_ijk < 0.) d_ijk = 0;
		if (d_ijk > 50.) d_ijk = 50.;

	    d_ijk = d_ijk*i;
		return short(d_ijk);
	}

	// Not tested.
	short f_sphere3(long x, long y, long z)
	{
		Vector v_in = Vector(x,y,z);
		Vector v = applyTransform(v_in);
		float i = v.x();
		float j = v.y();
		float k = v.z();

		float mag_sq = i*i+j*j+k*k;
		float d_ijk = 0.0f;					 //Air

		if (mag_sq < radius_sq) {		 //inside the sphere
			d_ijk = radius_sq - mag_sq;  //boundary of the sphere
			if(d_ijk >50) d_ijk = 50.0f;	 //interior of sphere
		}

		d_ijk = d_ijk*i;
		return short(d_ijk);
	}

private:
	float radius;
	float radius_sq;
};

//===============================================

class Cube : public ImplicitSurface {
public:

	Cube(float tx=0.0f, float ty=0.0f, float tz=0.0f,
			float rx=0.0f, float ry=0.0f, float rz=0.0f,
			float sx=1.0f, float sy=1.0f, float sz=1.0f) :
	ImplicitSurface(tx,ty,tz, rx,ry,rz, sx,sy,sz), half_width(1.0f)
	{  }

	Cube(Vector& t, Vector& r, Vector& s) :
	ImplicitSurface(t,r,s), half_width(1.0f)
	{  }

	virtual short solidObject(short x, short y, short z) {
		Vector v_in = Vector(x,y,z);
		Vector v = applyTransform(v_in);
		float i = fabs(v.x());
		float j = fabs(v.y());
		float k = fabs(v.z());

		float d = 50.0f;
		//If the projection of a vector from the center to a point onto
		//one of the coordinate axes is greater than half width, then
		//it is outside the cube.
		if ((i>half_width) || (j>half_width) || (k>half_width)) d = -100.0f;	//Air
		return short(d);
	}

	virtual short distanceField(short x, short y, short z) {
		Vector v_in = Vector(x,y,z);
		Vector v = applyTransform(v_in);
		float i = fabs(v.x());
		float j = fabs(v.y());
		float k = fabs(v.z());

		float di = i-half_width;
		float dj = j-half_width;
		float dk = k-half_width;

		// This logic is incorrect!
		float d_ijk = dk;
		if ((di<dj) && (di<dk)) d_ijk = di;
		else if ((dj<di) && (dj<dk)) d_ijk = dj;

		return short(d_ijk);
	}

	// This is probably not correct.
	virtual void directedDistanceField(short x, short y, short z,
									   float& dist_x, float& dist_y, float& dist_z) {
		Vector v_in = Vector(x,y,z);
		Vector v = applyTransform(v_in);
		float i = fabs(v.x());
		float j = fabs(v.y());
		float k = fabs(v.z());

		dist_x = i-half_width;
		dist_y = j-half_width;
		dist_z = k-half_width;
	}

private:
	float half_width;
};

//===============================================

/* cone
 * implicit equation:
 * (x^2+y^2)/c^2 = (z-z0)^2,
 *     where c = r/h = opening angle (radius/height)
 *     z0 = height of apex
 */
class Cone : public ImplicitSurface {
public:
	Cone(float tx=0.0f, float ty=0.0f, float tz=0.0f,
			float rx=0.0f, float ry=0.0f, float rz=0.0f,
			float sx=1.0f, float sy=1.0f, float sz=1.0f) :
	ImplicitSurface(tx,ty,tz, rx,ry,rz, sx,sy,sz), radius(1.0f), height(1.0f), z0(0.0f)
	{ c = radius/height; }

	Cone(Vector& t, Vector& r, Vector& s) :
	ImplicitSurface(t,r,s), radius(1.0f), height(1.0f), z0(0.0f)
	{ c = radius/height; }

	// Needs fine tuning.
	virtual short solidObject(short x, short y, short z) {
		Vector v_in = Vector(x,y,z);
		Vector v = applyTransform(v_in);
		float i = v.x();
		float j = v.y();
		float k = v.z();

		float density = 50.0f;
		if ((k<z0) || (k>height)) {
			density = -100.0f;
		} else {
				float value = (i*i+j*j)/(c*c) - (k-z0)*(k-z0);
				if (value > 0.0) density = -100.0;
		}
		return density;
	}

	// Not yet implemented.
	virtual short distanceField(short x, short y, short z) {
		return 0;
	}

	// Not yet implemented.
	virtual void directedDistanceField(short x, short y, short z,
									   float& dist_x, float& dist_y, float& dist_z) {
		dist_x = dist_y = dist_z = 0.0f;
	}

private:
	float radius;
	float height;
	float c; // opening angle = radius/height
	float z0; // height of apex
};

//===============================================

class CompoundObject : public ImplicitSurface {
public:
	CompoundObject() : ImplicitSurface() { }

	void addObject(ImplicitSurface* _obj) {
		subobjects.push_back(_obj);
	}

	virtual short solidObject(short x, short y, short z) {
		// union, for the moment
		float density = -100.0f;
		for (unsigned short i=0; i<subobjects.size(); i++) {
			if (subobjects[i]->solidObject(x,y,z) == 50.0f)
			{
				density = 50.0f;
				break;
			}
		}
		return density;
	}

	// Not yet implemented.
	virtual short distanceField(short x, short y, short z) {
		return 0;
	}

	// Not yet implemented.
	virtual void directedDistanceField(short x, short y, short z,
			float& dist_x, float& dist_y, float& dist_z) {
		dist_x = dist_y = dist_z = 0.0f;
	}

private:
	vector<ImplicitSurface*> subobjects;
};

#endif /* IMPLICITSURFACE_H_ */
