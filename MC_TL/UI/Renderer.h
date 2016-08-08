/*
 * Renderer.h
 *
 *  Created on: Aug 7, 2016
 *      Author: mdoherty
 */

// The renderer class is a collection of openGL drawing functions.
// These functions never set the display window.
// Display window selection should be done before calling Renderer functions.

#ifndef UI_RENDERER_H_
#define UI_RENDERER_H_
#include <vector>
using namespace std;
#include "../Color.h"
#include "../Point.h"
#include "../Vector.h"

class Renderer {
public:
	static void setProjection(int x, int y, int w, int h);

	static void drawMarker();
	static void displayLines();
	static void renderWireFrame(float borderwidth, float r, float g, float b, float a);
	static void displayFaces(int animation_face_code);
	static void displayNormal(Point p1,Point p2, Point p3,
					   Vector& n1,Vector& n2,Vector& n3,
					   float osx,float osy,float osz, int& mode);

	static void drawTriangle(vector<Point>& p, vector<Vector>& n, Color& c);
	static void drawLine(Point& p1, Point& p2, Color& c);
	static void drawCube(float dx, float dy, float dz, float osx, float osy, float osz);
	static void renderBitmapString(float x, float y, float z, void *font, char *string);
	static void renderString(float x, float y, char* string);

private:
	// This is a static class. No instances can be created.
	Renderer();
};

#endif /* UI_RENDERER_H_ */
