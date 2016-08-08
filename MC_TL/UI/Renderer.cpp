/*
 * Renderer.cpp
 *
 *  Created on: Aug 7, 2016
 *      Author: mdoherty
 */

#include "UICommon.h"
#include <GL/glut.h>
#include "../mc-io/mc1_io.h"
#include "Renderer.h"
#include "Materials.h"
#include "../cube.h"
#include "../global.h"

Renderer::Renderer() {
}


void Renderer::setProjection(int x, int y, int w, int h)
{
	glutPositionWindow(x, y);
	glutReshapeWindow(w, h);
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set the viewport to be the entire window
    glViewport(x, y, w, h);
	// Set the perspective projection matrix
	if (h == 0) h = 1; // Prevent a divide by zero
	gluPerspective(45,float(w)/h,0.1,100);
	// step back just a bit
	glTranslatef(0., 0., -4.);
	// return to model view coordinate system
	glMatrixMode(GL_MODELVIEW);
}


void Renderer::drawTriangle(vector<Point>& p, vector<Vector>& n, Color& c) {
	float osx = HEADER.offsetX, osy = HEADER.offsetY, osz = HEADER.offsetZ;
	glColor4f(c.red(), c.green(), c.blue(), 1.0);
	glNormal3f(n[0].x(), n[0].y(), n[0].z());
	glVertex3f(p[0].x()+osx, p[0].y()+osy, p[0].z()+osz);
	glNormal3f(n[1].x(), n[1].y(), n[1].z());
	glVertex3f(p[1].x()+osx, p[1].y()+osy, p[1].z()+osz);
	glNormal3f(n[2].x(), n[2].y(), n[2].z());
	glVertex3f(p[2].x()+osx, p[2].y()+osy, p[2].z()+osz);
}

void Renderer::drawLine(Point& p1, Point& p2, Color& c) {
	float osx = HEADER.offsetX, osy = HEADER.offsetY, osz = HEADER.offsetZ;
	glColor4f(c.red(), c.green(), c.blue(), 1.0);
	glVertex3f(p1.x()+osx, p1.y()+osy, p1.z()+osz);
	glVertex3f(p2.x()+osx, p2.y()+osy, p2.z()+osz);
}

// draw the movable marker cube
void Renderer::drawMarker()
{
	Color c1(0,1,0,1), c2(1,0,1,1);
	if (ui_common.blackBG) c2 = Color(1,1,0,1);
	int i = ui_common.marker_x;
	int j = ui_common.marker_y;
	int k = ui_common.marker_z;
	int d = 3;

	int x1 = i, x2 = i+1, y1 = j, y2 = j+1, z1 = k, z2 = k+1;
	Point p0(float(x1)*HEADER.dX,float(y1)*HEADER.dY,float(z1)*HEADER.dZ);
	Point p1(float(x2)*HEADER.dX,float(y1)*HEADER.dY,float(z1)*HEADER.dZ);
	Point p2(float(x1)*HEADER.dX,float(y2)*HEADER.dY,float(z1)*HEADER.dZ);
	Point p3(float(x2)*HEADER.dX,float(y2)*HEADER.dY,float(z1)*HEADER.dZ);
	Point p4(float(x1)*HEADER.dX,float(y1)*HEADER.dY,float(z2)*HEADER.dZ);
	Point p5(float(x2)*HEADER.dX,float(y1)*HEADER.dY,float(z2)*HEADER.dZ);
	Point p6(float(x1)*HEADER.dX,float(y2)*HEADER.dY,float(z2)*HEADER.dZ);
	Point p7(float(x2)*HEADER.dX,float(y2)*HEADER.dY,float(z2)*HEADER.dZ);

	x1 = i-d, x2 = i+d+1, y1 = j-d, y2 = j+d+1, z1 = k-d, z2 = k+d+1;
	Point q0(float(x1)*HEADER.dX,float(y1)*HEADER.dY,float(z1)*HEADER.dZ);
	Point q1(float(x2)*HEADER.dX,float(y1)*HEADER.dY,float(z1)*HEADER.dZ);
	Point q2(float(x1)*HEADER.dX,float(y2)*HEADER.dY,float(z1)*HEADER.dZ);
	Point q3(float(x2)*HEADER.dX,float(y2)*HEADER.dY,float(z1)*HEADER.dZ);
	Point q4(float(x1)*HEADER.dX,float(y1)*HEADER.dY,float(z2)*HEADER.dZ);
	Point q5(float(x2)*HEADER.dX,float(y1)*HEADER.dY,float(z2)*HEADER.dZ);
	Point q6(float(x1)*HEADER.dX,float(y2)*HEADER.dY,float(z2)*HEADER.dZ);
	Point q7(float(x2)*HEADER.dX,float(y2)*HEADER.dY,float(z2)*HEADER.dZ);

	Vector nfront( 0, 0,-1);
	Vector nback( 0, 0, 1);
	Vector nbottom( 0,-1, 0);
	Vector ntop( 0, 1, 0);
	Vector nleft(-1, 0, 0);
	Vector nright( 1, 0, 0);

	vector<Point> points;
	points.resize(3);
	vector<Vector> normals;
	normals.resize(3);

	glBegin(GL_TRIANGLES);
	// front
	normals[0] = normals[1] = normals[2] = nfront;
	points[0] = p2;	points[1] = p3;	points[2] = p0;
	drawTriangle(points, normals, c1);
	points[0] = p1;	points[1] = p0;	points[2] = p3;
	drawTriangle(points, normals, c1);
	// back
	normals[0] = normals[1] = normals[2] = nback;
	points[0] = p4;	points[1] = p5;	points[2] = p6;
	drawTriangle(points, normals, c1);
	points[0] = p7;	points[1] = p6;	points[2] = p5;
	drawTriangle(points, normals, c1);
	// top
	normals[0] = normals[1] = normals[2] = ntop;
	points[0] = p6;	points[1] = p7;	points[2] = p2;
	drawTriangle(points, normals, c1);
	points[0] = p3;	points[1] = p2;	points[2] = p7;
	drawTriangle(points, normals, c1);
	// bottom
	normals[0] = normals[1] = normals[2] = nbottom;
	points[0] = p0;	points[1] = p1;	points[2] = p4;
	drawTriangle(points, normals, c1);
	points[0] = p5;	points[1] = p4;	points[2] = p1;
	drawTriangle(points, normals, c1);
	// left
	normals[0] = normals[1] = normals[2] = nleft;
	points[0] = p6;	points[1] = p2;	points[2] = p4;
	drawTriangle(points, normals, c1);
	points[0] = p0;	points[1] = p4;	points[2] = p2;
	drawTriangle(points, normals, c1);
	// right
	normals[0] = normals[1] = normals[2] = nright;
	points[0] = p3;	points[1] = p7;	points[2] = p1;
	drawTriangle(points, normals, c1);
	points[0] = p5;	points[1] = p1;	points[2] = p7;
	drawTriangle(points, normals, c1);
	glEnd();
	glBegin(GL_LINES);
	drawLine(q0,q1,c2);
	drawLine(q1,q3,c2);
	drawLine(q3,q2,c2);
	drawLine(q2,q0,c2);
	drawLine(q6,q7,c2);
	drawLine(q7,q5,c2);
	drawLine(q5,q4,c2);
	drawLine(q4,q6,c2);
	drawLine(q2,q6,c2);
	drawLine(q3,q7,c2);
	drawLine(q0,q4,c2);
	drawLine(q1,q5,c2);
	glEnd();
}

void Renderer::displayLines() {

	float offset_Lx = -0.55, offset_Ly = -0.55, offset_Lz = 0.55;
	glBegin(GL_LINES);
		glColor3f(1., 0., 0.);
		glVertex3f(0.+offset_Lx,0.+offset_Ly,0.+offset_Lz);				//x-axis
		glVertex3f(0.3+offset_Lx,0.+offset_Ly,0.+offset_Lz);

		glColor3f(0., 1., 0.);
		glVertex3f(0.+offset_Lx,0.+offset_Ly,0.+offset_Lz);				//y-axis
		glVertex3f(0.+offset_Lx,0.3+offset_Ly,0.+offset_Lz);

		glColor3f(0., 0., 1.);
		glVertex3f(0.+offset_Lx,0.+offset_Ly,0.+offset_Lz);				//z-axis
		glVertex3f(0.+offset_Lx,0.+offset_Ly,-0.3+offset_Lz);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(1,0,0); glVertex3f(.5, 0.0, 0.0);  glVertex3f(1.0, 0.0, 0.0);
		glColor3f(0,1,0); glVertex3f(0.0, 0.5, 0.0); glVertex3f(0.0, 1.0, 0.0);
		glColor3f(0,0,1); glVertex3f(0.0, 0.0, 0.5); glVertex3f(0.0, 0.0, 1.0);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(1,0,0); glVertex3f(-0.5, 0.0, 0.0); glVertex3f(-1.0, 0.0, 0.0);
		glColor3f(0,1,0); glVertex3f(0.0, -0.5, 0.0); glVertex3f(0.0, -1.0, 0.0);
		glColor3f(0,0,1); glVertex3f(0.0, 0.0, -0.5); glVertex3f(0.0, 0.0, -1.0);
	glEnd();
}

void Renderer::renderWireFrame(float borderwidth, float r, float g, float b, float a){
	glColor4f(r, g, b, a);
	glutWireCube(borderwidth);
}

void Renderer::renderBitmapString(float x, float y, float z, void *font, char *string)
{
  char *c;
  glColor3f(0.0, 0.0, 0.);
  glRasterPos3f(x,y,z);
  for (c=string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void Renderer::renderString(float x, float y, char* string)
{
	glPushMatrix();		//save orientation of coordinate systems
	glLoadIdentity();
	renderBitmapString(x ,y, 0, GLUT_BITMAP_HELVETICA_18, string);
	glPopMatrix();		//restore orientation of coordinate systems
}


void Renderer::displayFaces(int animation_face_code){

	float osx = HEADER.offsetX, osy = HEADER.offsetY, osz = HEADER.offsetZ;
	//float dx = HEADER.cf_deltaX, dy = HEADER.cf_deltaY, dz = HEADER.cf_deltaZ;
	Cube* curr_cube;

	int drawBadNormalCounter=0;
	for (int j=0; j<global_facesVector.size(); j++) {
	//for (int j=0; j<100; j++) {
		Face f = global_facesVector[j];
		if (ui_common.enable_animation && (f.getAnimationCode() > animation_face_code))
			continue;
		Point p1 = f.point1(), p2 = f.point2(), p3 = f.point3();
		Vector n1 = f.normal1(), n2=f.normal2(), n3 = f.normal3();
		Color color = f.getColor();

		int isBadNormal=0;
		if (f.isDrawFace()) {
			isBadNormal=1;
			drawBadNormalCounter++;
			cout<<"isDrawFace="<<f.isDrawFace()<<endl;
				//	glColor4f(0.9,0.9,0.,1.);
				//}else{
				//glColor4f(1.,0.,0.,1.);
		}
			//curr_cube = f.getCube();
			//if(find_j && curr_cube->ci == 3998885)
			//{ cout<<"cube_id="<<curr_cube->ci<<", j="<<j<<endl; find_j=0; }

		if(isBadNormal && ui_common.displayIndex && drawBadNormalCounter<=ui_common.displayAmt){
			curr_cube = f.getCube();
			cout<<"---"<<endl;
			cout<<"j="<<j<<", drawBadNormalCounter="<<drawBadNormalCounter<<endl;
			cout<<"Number face in a cube = "<<f.getNumFaces()<<", faceID="<<f.getFaceID();
			cout<<", cornerFlag="<<curr_cube->cornerFlag<<endl;
			cout<<"ActiveCorners="<<curr_cube->activeCorners<<", corner Positions=[";
			for(int i=0; i<curr_cube->cornerPos.size(); i++){
				cout<<curr_cube->cornerPos[i];
				if(i!=curr_cube->cornerPos.size()-1) cout<<",";
			}
			cout<<"]"<<endl;
			cout<<"Corner0 position=["<<curr_cube->pt.x()<<","<<curr_cube->pt.y()<<","<<curr_cube->pt.z()<<"]"<<endl;
			cout<<"point=["<<p1.x()+osx<<","<<p1.y()+osy<<","<<p1.z()+osz<<"]"<<endl;
			cout<<"point=["<<p2.x()+osx<<","<<p2.y()+osy<<","<<p2.z()+osz<<"]"<<endl;
			cout<<"point=["<<p3.x()+osx<<","<<p3.y()+osy<<","<<p3.z()+osz<<"]"<<endl;
			if (drawBadNormalCounter==ui_common.displayAmt) ui_common.displayIndex=0;
		}
		int target_j = 161827;
		int mode = 0;    //0=anti-clockwise; 1=clockwise; 3=translate the face to (0,0,0)
		if((ui_common.drawOneFace && (j==target_j)) || ((!ui_common.drawOneFace)&&(mode==0))){
			Point translation, p1_prime, p2_prime, p3_prime;
			if(ui_common.isDrawFaceNormal){
				glDisable(GL_LIGHTING);
				if(mode==3){ //translate a face to (0,0,0)
					translation = p1;
					p1_prime = Point(p1-translation);
					p2_prime = Point(p2-translation);
					p3_prime = Point(p3-translation);
					p1_prime = p1_prime*50;
					p2_prime = p2_prime*50;
					p3_prime = p3_prime*50;
					displayNormal(p1_prime,p2_prime,p3_prime,n1,n2,n3,osx,osy,osz,mode);

				}else if(mode==0 && j==target_j)
					displayNormal(p1,p2,p3,n1,n2,n3,osx,osy,osz,mode);
				glEnable(GL_LIGHTING);
			}
			setMaterial_using_glColor();
			glBegin(GL_TRIANGLES);
			switch (mode) {
				case 0:
						//glColor4f(0.5,0.0,0.0,1.);
						//	glColor4f(0.0,0.5,0.,1.);
					glColor4f(color.red(), color.green(), color.blue(), 1.0);
					glNormal3f(n1.x(), n1.y(), n1.z());
					glVertex3f(p1.x()+osx, p1.y()+osy, p1.z()+osz);

					glNormal3f(n2.x(), n2.y(), n2.z());
					glVertex3f(p2.x()+osx, p2.y()+osy, p2.z()+osz);

					glNormal3f(n3.x(), n3.y(), n3.z());
					glVertex3f(p3.x()+osx, p3.y()+osy, p3.z()+osz);

					break;
				case 1:
					glVertex3f(p3.x()+osx, p3.y()+osy, p3.z()+osz);
					glNormal3f(n3.x(), n3.y(), n3.z());
					glVertex3f(p2.x()+osx, p2.y()+osy, p2.z()+osz);
					glNormal3f(n2.x(), n2.y(), n2.z());
					glVertex3f(p1.x()+osx, p1.y()+osy, p1.z()+osz);
					glNormal3f(n1.x(), n1.y(), n1.z());
					break;
				case 3: //centering the face
						//glColor4f(1., 0, 1., 0.);
						glNormal3f(n1.x(), n1.y(), n1.z());
						glVertex3f(p1_prime.x(), p1_prime.y(), p1_prime.z());
						glNormal3f(n2.x(), n2.y(), n2.z());
						glVertex3f(p2_prime.x(), p2_prime.y(), p2_prime.z());
						glNormal3f(n3.x(), n3.y(), n3.z());
						glVertex3f(p3_prime.x(), p3_prime.y(), p3_prime.z());
						break;
				default:
					break;
			}
			glEnd();

				//glDisable(GL_LIGHTING);
				//displayNormal(p1,p2,p3,n1,n2,n3,osx,osy,osz,mode);
				//glEnable(GL_LIGHTING);

			}

		// outfile<<" rendering vertex1: ["<<p1.x()+offsetx<<","<<p1.y()+offsety<<","<<p1.z()+offsetz<<"]"<<endl;
		// outfile<<" rendering vertex2: ["<<p2.x()+offsetx<<","<<p2.y()+offsety<<","<<p2.z()+offsetz<<"]"<<endl;
		// outfile<<" rendering vertex3: ["<<p3.x()+offsetx<<","<<p3.y()+offsety<<","<<p3.z()+offsetz<<"]"<<endl;
		// outfile<<" ------------------ "<<endl;
	}//end for
	 // drawCube(dx,dy,dz, osx, osy, osz);
}


void Renderer::displayNormal(Point p1,Point p2, Point p3,
				   Vector& n1,Vector& n2,Vector& n3,
				   float osx,float osy,float osz, int& mode) {

		//if(global_index){
	float s=0.05;

	Point dn1 = p1+(n1*s);
	Point dn2 = p2+(n2*s);
	Point dn3 = p3+(n3*s);

	if(mode==3) { osx =0., osy=0.; osz=0; }

	glBegin(GL_LINES);
		glColor3f(1., 0., 0.);
		glVertex3f(p1.x()+osx, p1.y()+osy, p1.z()+osz);				//x-axis
		glVertex3f(dn1.x()+osx,dn1.y()+osy, dn1.z()+osz);

		glColor3f(0., 1., 0.);
		glVertex3f(p2.x()+osx, p2.y()+osy, p2.z()+osz);				//y-axis
		glVertex3f(dn2.x()+osx, dn2.y()+osy, dn2.z()+osz);

		glColor3f(0., 0., 1.);
		glVertex3f(p3.x()+osx, p3.y()+osy, p3.z()+osz);				//z-axis
		glVertex3f(dn3.x()+osx,dn3.y()+osy,dn3.z()+osz);
	glEnd();

	if(ui_common.outputOnce){
		cout<<"Display Normal: s="<<s<<endl;
		cout<<"ofsetX="<<osx<<","<<"offsetY="<<osy<<","<<"offsetZ="<<osz<<endl;
		cout<<"Point 1 (start point) =["<<p1.x()+osx<<", "<<p1.y()+osy<<", "<<p1.z()+osz<<"]"<<endl;
		cout<<"Point 1 (end point) =["<<(dn1.x()+osx)<<", "<<(dn1.y()+osy)<<", "<<(dn1.z()+osz)<<"]"<<endl;
		cout<<"Point 2 (start point) =["<<p2.x()+osx<<", "<<p1.y()+osy<<", "<<p1.z()+osz<<"]"<<endl;
		cout<<"Point 2 (end point) =["<<(dn2.x()+osx)<<", "<<(dn1.y()+osy)<<", "<<(dn1.z()+osz)<<"]"<<endl;
		cout<<"Point 3 (start point) =["<<p3.x()+osx<<", "<<p1.y()+osy<<", "<<p1.z()+osz<<"]"<<endl;
		cout<<"Point 3 (end point) =["<<(dn3.x()+osx)<<", "<<(dn1.y()+osy)<<", "<<(dn1.z()+osz)<<"]"<<endl;
		ui_common.outputOnce = false;
	}

}

void Renderer::drawCube(float dx, float dy, float dz, float osx, float osy, float osz) {

		//cout<<"[osx,osy,osz]=["<<osx<<","<<osy<<","<<osz<<"]"<<endl;
	osx = -0.5;
	osy = -0.5;
	osz = -0.5;

	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads

		// Top face (y = 1.0f)
		// Define vertices in counter-clockwise (CCW) order with normal pointing out
		//	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f( 0.0f*dx+osx, 1.0f*dy+osy, 0.0f*dz+osz);
	glVertex3f( 0.0f*dx+osx, 1.0f*dy+osy, -1.0f*dz+osz);
	glVertex3f( 1.0f*dx+osx, 1.0f*dy+osy, -1.0f*dz+osz);
	glVertex3f( 1.0f*dx+osx, 1.0f*dy+osy,  0.0f*dz+osz);

		// Bottom face (y = 0.0f)
		//	glColor3f(1.0f, 0.5f, 0.0f);     // Orange
	glVertex3f( 0.0f*dx+osx, 0.0f*dy+osy,  0.0f*dz+osz);
	glVertex3f( 0.0f*dx+osx, 0.0f*dy+osy, -1.0f*dz+osz);
	glVertex3f( 1.0f*dx+osx, 0.0f*dy+osy, -1.0f*dz+osz);
	glVertex3f( 1.0f*dx+osx, 0.0f*dy+osy,  0.0f*dz+osz);

		// Front face  (z = 0.0f)
		//	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f( 0.0f*dx+osx,  0.0f*dy+osy, 0.0f*dz+osz);
	glVertex3f( 1.0f*dx+osx,  0.0f*dy+osy, 0.0f*dz+osz);
	glVertex3f( 1.0f*dx+osx,  1.0f*dy+osy, 0.0f*dz+osz);
	glVertex3f( 0.0f*dx+osx,  1.0f*dy+osy, 0.0f*dz+osz);

		// Back face (z = -1.0f)
		//glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
	glVertex3f( 0.0f*dx+osx,  0.0f*dy+osy, -1.0f*dz+osz);
	glVertex3f( 1.0f*dx+osx,  0.0f*dy+osy, -1.0f*dz+osz);
	glVertex3f( 1.0f*dx+osx,  1.0f*dy+osy, -1.0f*dz+osz);
	glVertex3f( 0.0f*dx+osx,  1.0f*dy+osy, -1.0f*dz+osz);

		// Left face (x = 0.0f)
		//glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f( 0.0f*dx+osx,  0.0f*dy+osy,  0.0f*dz+osz);
	glVertex3f( 0.0f*dx+osx,  0.0f*dy+osy, -1.0f*dz+osz);
	glVertex3f( 0.0f*dx+osx,  1.0f*dy+osy, -1.0f*dz+osz);
	glVertex3f( 0.0f*dx+osx,  1.0f*dy+osy,  0.0f*dz+osz);

		// Right face (x = 1.0f)
		//glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
	glVertex3f(1.0f*dx+osx,  0.0f*dy+osy,  0.0f*dz+osz);
	glVertex3f(1.0f*dx+osx,  0.0f*dy+osy, -1.0f*dz+osz);
	glVertex3f(1.0f*dx+osx,  1.0f*dy+osy, -1.0f*dz+osz);
	glVertex3f(1.0f*dx+osx,  1.0f*dy+osy,  0.0f*dz+osz);
	glEnd();  // End of drawing color-cube
}

