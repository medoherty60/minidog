#include <cstdio>
#include <cstring>
using namespace std;
#include <GL/glut.h>

#include "RenderManager.h"
#include "Camera.h"
#include "InputProcessor.h"
#include "LightManager.h"
#include "../global.h"

int RenderManager::main_window=0, RenderManager::object_window=0;
int RenderManager::controlPanel_window=0;
Camera* RenderManager::camera = NULL;
InputProcessor* RenderManager::input_processor = NULL;

#ifdef USE_GLUI
static int mouse_x_display = 0;
static int mouse_y_display = 0;
static GLUI_EditText* mouse_x_text = NULL;
static GLUI_EditText* mouse_y_text = NULL;
static GLUI_EditText* marker_x_text = NULL;
static GLUI_EditText* marker_y_text = NULL;
static GLUI_EditText* marker_z_text = NULL;
static GLUI_EditText* marker_incr_text = NULL;
static GLUI_Button* camera_restore_buttons[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
GLUI *RenderManager::gluiCPW = NULL;
#endif

bool RenderManager::bothFaces = true;
bool RenderManager::drawOneFace = false;
bool RenderManager::outputOnce = false;
bool RenderManager::isDrawFaceNormal = false;
int RenderManager::displayAmt = 5;
int RenderManager::displayIndex = 0;
int RenderManager::blackBG = 0;
int RenderManager::showCube = 1;
int RenderManager::showAxes = 1;

// animation control
clock_t RenderManager::last_frame_time = 0;
float RenderManager::elapsed_frame_time = 0.0;
bool RenderManager::first_animation_frame = true;
bool RenderManager::enable_animation = true;
int RenderManager::animation_face_code = 0;

// marker object
int RenderManager::marker_x = 0;
int RenderManager::marker_y = 0;
int RenderManager::marker_z = 0;
bool RenderManager::marker_enabled = true;
int RenderManager::marker_incr = 10;

static void copyColor(float c1[4], float c2[4]){
	memcpy(c1, c2, 4*sizeof(float));
}

static void setMaterial_Cube(){
	
	GLfloat mat_specular[4],  mat_diffuse[4];
	GLfloat mat_diffuse_bk[4], mat_ambient_bk[4], mat_specular_bk[4];
	
	GLfloat white[4] = 	{ 1., 1., 1., 1.};
	GLfloat red_dark[4] = { 0.0, 0.0, 0.75, 1.0 };
	GLfloat green_full[4] = { 0, 0., 1, 1.0, };
	GLfloat green_light[4] = { 0, 0.5, 0, 1.0,  };
	GLfloat green_dark[4] = { 0, .75, 0, 1.0, };
	GLfloat gray_dark[4] = {0.75, 0.75, 0.75, 1.0 };
	
	GLfloat mat_shininess[] = {70.0};
	
	copyColor(mat_specular, gray_dark);
	copyColor(mat_diffuse, white);
	copyColor(mat_diffuse_bk, green_full);
	copyColor(mat_ambient_bk, green_light);
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
	glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse_bk);
	glMaterialfv(GL_BACK, GL_AMBIENT, mat_ambient_bk);
	glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);
	
	glColorMaterial (GL_FRONT, GL_AMBIENT);
		//glEnable (GL_COLOR_MATERIAL);
}

static void setMaterial_using_glColor(){
	
	GLfloat mat_specular[4],  mat_diffuse[4];
	GLfloat mat_diffuse_bk[4], mat_ambient_bk[4], mat_specular_bk[4];
	
	GLfloat white[4] = 	{ 1., 1., 1., 1.};
	GLfloat red_dark[4] = { 0.75, 0.75, 0.75, 1.0 };
	GLfloat green_full[4] = { 0, 1., 0, 1.0, };
	GLfloat green_light[4] = { 0, 0.5, 0, 1.0,  };
	GLfloat green_dark[4] = { 0, .75, 0, 1.0, };
	GLfloat gray_dark[4] = {0.75, 0.75, 0.75, 1.0 };
	
	GLfloat mat_shininess[] = {70.0};
	
	copyColor(mat_specular, gray_dark);
	copyColor(mat_diffuse, white);
	copyColor(mat_diffuse_bk, green_full);
	copyColor(mat_ambient_bk, green_light);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse_bk);
	glMaterialfv(GL_BACK, GL_AMBIENT, mat_ambient_bk);
	glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);
	
	glColorMaterial (GL_FRONT, GL_AMBIENT);
	glEnable (GL_COLOR_MATERIAL);
}

static void setMaterial(){
	
	int SpecularOn = 1;
	
	GLfloat mat_specular[4], mat_diffuse[4], mat_ambient[4];
	GLfloat mat_shininess[] = {70.0};
	
	GLfloat green_full[4] = { 0, 1., 0, 1.0, };
	GLfloat green_light[4] = { 0, 0.5, 0, 1.0,  };

	GLfloat black[4] = { 0.,0.,0.,1.0};
	GLfloat gray_dark[4] = { 0.65, 0.65, 0.75, 1.0 };
	GLfloat red_light[4] = { 0.5, 0., 0., 1.};
	GLfloat white[4] = 	{ 1., 1., 1., 1.};
	
	//Set up Surface Material Color
	copyColor(mat_diffuse, white);
    copyColor(mat_ambient, red_light);
		
	if(SpecularOn) copyColor(mat_specular, gray_dark);
	else copyColor(mat_specular, black);
	
	//Back Face Material (default: green)
	GLfloat mat_diffuse_bk[4], mat_ambient_bk[4];
	
	copyColor(mat_diffuse_bk, green_full);
	copyColor(mat_ambient_bk, green_light);
	
		//default: glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE )
		// set Ambient and Diffuse the same color using glColor in rendering routine
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse_bk);
	glMaterialfv(GL_BACK, GL_AMBIENT, mat_ambient_bk);
	glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);
	
	//glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glEnable (GL_COLOR_MATERIAL);
}

static void setMaterialUNUSED(){
	
	int SpecularOn = 1;
	
		//Set up Surface Material Color
	GLfloat mat_specular[4];
	GLfloat mat_shininess[] = {70.0};
	GLfloat mat_diffuse[] = {1.,1.,1.,1.};
	GLfloat mat_ambient[] = {0.5,0,0,1.};
	
	if(SpecularOn){
		mat_specular[0] = 0.75;
		mat_specular[1] = 0.75;
		mat_specular[2] = 0.75;
		mat_specular[3] = 1.0;
	}else {
		mat_specular[0] = 0.0;
		mat_specular[1] = 0.0;
		mat_specular[2] = 0.0;
		mat_specular[3] = 1.0;
	}
	
		//Back Face Material
	GLfloat mat_diffuse_bk[] = { 0, 1., 0, 1.0 };
	GLfloat mat_ambient_bk[] = { 0, 0.5, 0, 1.0 };
	
		//default: glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE )
		// set Ambient and Diffuse the same color using glColor in rendering routine
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
	glMaterialfv(GL_BACK, GL_DIFFUSE, mat_diffuse_bk);
	glMaterialfv(GL_BACK, GL_AMBIENT, mat_ambient_bk);
	glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);
	
		//glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		//glEnable (GL_COLOR_MATERIAL);
}

void displayNormal(Point p1,Point p2, Point p3,
				   Vector& n1,Vector& n2,Vector& n3,
				   float osx,float osy,float osz, int& mode){

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

	if(RenderManager::outputOnce){
		cout<<"Display Normal: s="<<s<<endl;
		cout<<"ofsetX="<<osx<<","<<"offsetY="<<osy<<","<<"offsetZ="<<osz<<endl;
		cout<<"Point 1 (start point) =["<<p1.x()+osx<<", "<<p1.y()+osy<<", "<<p1.z()+osz<<"]"<<endl;
		cout<<"Point 1 (end point) =["<<(dn1.x()+osx)<<", "<<(dn1.y()+osy)<<", "<<(dn1.z()+osz)<<"]"<<endl;
		cout<<"Point 2 (start point) =["<<p2.x()+osx<<", "<<p1.y()+osy<<", "<<p1.z()+osz<<"]"<<endl;
		cout<<"Point 2 (end point) =["<<(dn2.x()+osx)<<", "<<(dn1.y()+osy)<<", "<<(dn1.z()+osz)<<"]"<<endl;
		cout<<"Point 3 (start point) =["<<p3.x()+osx<<", "<<p1.y()+osy<<", "<<p1.z()+osz<<"]"<<endl;
		cout<<"Point 3 (end point) =["<<(dn3.x()+osx)<<", "<<(dn1.y()+osy)<<", "<<(dn1.z()+osz)<<"]"<<endl;
		RenderManager::outputOnce = false;
	}
	
}

static void drawCube(float dx, float dy, float dz, float osx, float osy, float osz){
	
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

static void renderWireFrame(float borderwidth, float r, float g, float b, float a){
	glColor4f(r, g, b, a);
	glutWireCube(borderwidth);
}

static void displayLines(){

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

void RenderManager::displayFaces(){
	
	float osx = HEADER.offsetX, osy = HEADER.offsetY, osz = HEADER.offsetZ;
	//float dx = HEADER.cf_deltaX, dy = HEADER.cf_deltaY, dz = HEADER.cf_deltaZ;
	Cube* curr_cube;
	
	int drawBadNormalCounter=0;
	for (int j=0; j<global_facesVector.size(); j++) {
	//for (int j=0; j<100; j++) {
		Face f = global_facesVector[j];
		if (enable_animation && (f.getAnimationCode() > animation_face_code))
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
			
		if(isBadNormal && displayIndex && drawBadNormalCounter<=displayAmt){ 
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
			if (drawBadNormalCounter==displayAmt) displayIndex=0;
		}
		int target_j = 161827;
		int mode = 0;    //0=anti-clockwise; 1=clockwise; 3=translate the face to (0,0,0)
		if((drawOneFace && (j==target_j)) || ((!drawOneFace)&&(mode==0))){
			Point translation, p1_prime, p2_prime, p3_prime;
			if(isDrawFaceNormal){
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


/* Marina's  version

void RenderManager::displayFaces(){

	float osx = HEADER.offsetX, osy = HEADER.offsetY, osz = HEADER.offsetZ;

	drawBadNormalCounter=0;
	for (unsigned int j=0; j<global_facesVector.size(); j++) {

		Face f = global_facesVector[j];
		Point p1 = f.point1(), p2 = f.point2(), p3 = f.point3();
		Vector n1 = f.normal1(), n2=f.normal2(), n3 = f.normal3();
		Color color = f.getColor();

		isBadNormal=0;
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

		if(isBadNormal && displayIndex && drawBadNormalCounter<=displayAmt){
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
			if (drawBadNormalCounter==displayAmt) displayIndex=0;
		}
		int target_j = 161827;
		int mode = 0;    //0=anti-clockwise; 1=clockwise; 3=translate the face to (0,0,0)
		if((drawOneFace && (j==target_j)) || ((!drawOneFace)&&(mode==0))){

			if(isDrawFaceNormal){
				glDisable(GL_LIGHTING);
				if(mode==3){ //translate a face to (0,0,0)
					translation = p1;
					p1_prime = Point(p1-translation);
					p2_prime = Point(p2-translation);
					p3_prime = Point(p3-translation);
					p1_prime = p1_prime*50;
					p2_prime = p2_prime*50;
					p3_prime = p3_prime*50;
					displayNormal(p1_prime,p2_prime,p3_prime,
								  n1,n2,n3,
								  osx,osy,osz,
								  mode);

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
		 //outfile<<" rendering vertex1: ["<<p1.x()+offsetx<<","<<p1.y()+offsety<<","<<p1.z()+offsetz<<"]"<<endl;
		 //outfile<<" rendering vertex2: ["<<p2.x()+offsetx<<","<<p2.y()+offsety<<","<<p2.z()+offsetz<<"]"<<endl;
		 //outfile<<" rendering vertex3: ["<<p3.x()+offsetx<<","<<p3.y()+offsety<<","<<p3.z()+offsetz<<"]"<<endl;
		 //outfile<<" ------------------ "<<endl;
	}//end for
	 // drawCube(dx,dy,dz, osx, osy, osz);
}
*/

void RenderManager::idle(void){
	glutSetWindow(controlPanel_window);
	glutPostRedisplay();
	if (text_window != NULL) {
		glutSetWindow(text_window->getGlutWindow());
		glutPostRedisplay();
	}
	glutSetWindow(object_window);
	glutPostRedisplay();
}
/*
// auto-rotate animation
void RenderManager::updateAnimation() {
	if (first_animation_frame) {
		elapsed_frame_time = 0.0f;
		last_frame_time = clock();
		first_animation_frame = false;
	}
	else {
		 clock_t time = clock() ;
		 elapsed_frame_time = (double)(time - last_frame_time)/CLOCKS_PER_SEC;
		 last_frame_time = time;
	}
	float angle = elapsed_frame_time;
	camera->doRotationAngleAxis(angle*2.0,0.0,1.0,0.0);
	camera->doRotationAngleAxis(angle,1.0,0.0,0.0);
}
*/
// selective face animation
static int frame_count = 0;

void RenderManager::updateAnimation() {
	if (first_animation_frame) {
		elapsed_frame_time = 0.0f;
		last_frame_time = clock();
		first_animation_frame = false;
		animation_face_code = 0;
	}
	else {
		 clock_t time = clock() ;
		 elapsed_frame_time = (double)(time - last_frame_time)/CLOCKS_PER_SEC;
		 last_frame_time = time;
		 frame_count++;
		 if (frame_count >= 10) {
			 animation_face_code+=1;
			 frame_count = 0;
		 }
	}
}

void drawTriangle(vector<Point>& p, vector<Vector>& n, Color& c) {
	float osx = HEADER.offsetX, osy = HEADER.offsetY, osz = HEADER.offsetZ;
	glColor4f(c.red(), c.green(), c.blue(), 1.0);
	glNormal3f(n[0].x(), n[0].y(), n[0].z());
	glVertex3f(p[0].x()+osx, p[0].y()+osy, p[0].z()+osz);
	glNormal3f(n[1].x(), n[1].y(), n[1].z());
	glVertex3f(p[1].x()+osx, p[1].y()+osy, p[1].z()+osz);
	glNormal3f(n[2].x(), n[2].y(), n[2].z());
	glVertex3f(p[2].x()+osx, p[2].y()+osy, p[2].z()+osz);
}

void drawLine(Point& p1, Point& p2, Color& c) {
	float osx = HEADER.offsetX, osy = HEADER.offsetY, osz = HEADER.offsetZ;
	glColor4f(c.red(), c.green(), c.blue(), 1.0);
	glVertex3f(p1.x()+osx, p1.y()+osy, p1.z()+osz);
	glVertex3f(p2.x()+osx, p2.y()+osy, p2.z()+osz);
}

// draw the movable marker cube
void RenderManager::drawMarker()
{
	Color c1(0,1,0,1), c2(1,0,1,1);
	if (blackBG) c2 = Color(1,1,0,1);
	int i = marker_x;
	int j = marker_y;
	int k = marker_z;
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


//Display func for Object Window
void RenderManager::renderSceneOW() {
	
	glutSetWindow(object_window);
	if (blackBG)
		glClearColor (0.0, 0.0, 0.0, 0.0);
	else
		glClearColor (1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	//glPushMatrix();		//save previous matrix - contains settings for perspective projection
	glLoadIdentity();		//reset matrix  // glTranslatef(0., 0., -4.);
	gluPerspective(40., camera->getWindowWidth()/camera->getWindowHeight(), 0.1, 60.);   //set clipping volume
	glTranslatef(0., 0., -3.);

#ifdef USE_MULTISAMPLING
    glEnable(GL_MULTISAMPLE_ARB);
#else
    glDisable(GL_MULTISAMPLE_ARB);
#endif
	
    if (enable_animation) updateAnimation();
    displayFaces();
	glDisable(GL_LIGHTING);
	//displayNormal();

	if (showCube) {
		// draw a unit wireframe cube around the rendered volume
		if (blackBG)
			renderWireFrame(1.,1.,1.,1.,1.);       //borderwidth,r,g,b,a
		else
			renderWireFrame(1.,0.,0.,0.,1.);
	}

	if (showAxes) {
		// draw the coordinate axes
		displayLines();
	}

	if (marker_enabled) {
		drawMarker();
	}

	// turn on the lights
	glEnable(GL_LIGHTING);
	//renderLightObjects();
	
	//setOrthogrpahicProjection();
	glutSwapBuffers();
	glFlush();
}

//Display func for Control Panel Window
void RenderManager::renderSceneCPW(){

	glutSetWindow(controlPanel_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef USE_MULTISAMPLING
    glEnable(GL_MULTISAMPLE_ARB);
#else
    glDisable(GL_MULTISAMPLE_ARB);
#endif

#ifdef USE_GLUI
	mouse_x_text->set_int_val(input_processor->getMouseX());
	mouse_y_text->set_int_val(input_processor->getMouseY());

	for (short i=0; i<10; i++)
		if (camera->restoreStateAvailable(i)) camera_restore_buttons[i]->enable();
		else camera_restore_buttons[i]->disable();
#else
	//render Control Panel
	char s0[1000], s1[1000];

	sprintf(s0,"isovalue=%0.2f", HEADER.isoVal);
	renderString(-0.5,0.6,s0);

	sprintf(s1,"x,y = %i,%i", input_processor->getMouseX(), input_processor->getMouseY());
	renderString(-0.5,0.5,s1);

    char* s00 = "Keyboard controls (image window)";
    char* s01 = "Sn = save camera state n";
    char* s02 = "Rn = restore camera state n";
    char* s03 = "W = write camera states to file";
    char* s04 = "A = read camera states from file";
    char* s05 = "K and J = move on x-axis";
    char* s06 = "I and O = move on y-axis";
    char* s07 = "N and M = move on z-axis";
    char* s08 = "H and G = rotate horizontal";
    char* s09 = "Y and U = rotate vertical";
    char* s10 = "Q = quit program";
    float x = -0.6, y = -0.4;
    renderString(x,y-=0.1,s00);
    renderString(x,y-=0.1,s01);
    renderString(x,y-=0.1,s02);
    renderString(x,y-=0.1,s03);
    renderString(x,y-=0.1,s04);
    renderString(x,y-=0.1,s05);
    renderString(x,y-=0.1,s06);
    renderString(x,y-=0.1,s07);
    renderString(x,y-=0.1,s08);
    renderString(x,y-=0.1,s09);
    renderString(x,y-=0.1,s10);
#endif

	glutSwapBuffers();
	glFlush();
}

void RenderManager::initOW(){
	object_window = glutCreateSubWindow(main_window, 0, 0, 700, 700);
	if (camera != NULL) delete camera;
	if (input_processor != NULL) delete input_processor;
	camera = new Camera(object_window);
	input_processor = new InputProcessor(camera);
	glutDisplayFunc(renderSceneOW);

	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT, GL_FILL);
	glutKeyboardFunc(keyboardOW);
	//glutSpecialFunc(specialKey);
	glutMouseFunc(mouseOW);
	glutMotionFunc(motionOW);
	glutPassiveMotionFunc(passive_motionOW);
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	
	if(!bothFaces){
		//suppress backfaces
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	
	light_manager.setup_lightObject();
	light_manager.light0_distance(20.);
	//setMaterial();
	setMaterial_using_glColor();

	// flip the image upside-down at the start
	camera->doRotationAngleAxis(180.0,1.,0.,0.);
}

void RenderManager::cameraButton(int id) {
	if (input_processor == NULL) return;
	if (id == 1) {
		input_processor->keyboard('a',0,0);
	}
	else if (id == 2) {
		input_processor->keyboard('w',0,0);
	}
	else if ((id >=100) && (id < 200)) {
		if (input_processor != NULL) {
			input_processor->keyboard('r',0,0);
			input_processor->keyboard(char('0'+(id-100)),0,0);
		}
	}
	else if ((id >=200) && (id < 300)) {
		if (input_processor != NULL) {
			input_processor->keyboard('s',0,0);
			input_processor->keyboard(char('0'+(id-200)),0,0);
		}
	}
}

void RenderManager::markerButton(int id) {
	switch (id) {
	case 0:
		marker_x += marker_incr;
		marker_x_text->set_int_val(marker_x);
		break;
	case 1:
		marker_x -= marker_incr;
		marker_x_text->set_int_val(marker_x);
		break;
	case 2:
		marker_y += marker_incr;
		marker_y_text->set_int_val(marker_y);
		break;
	case 3:
		marker_y -= marker_incr;
		marker_y_text->set_int_val(marker_y);
		break;
	case 4:
		marker_z += marker_incr;
		marker_z_text->set_int_val(marker_z);
		break;
	case 5:
		marker_z -= marker_incr;
		marker_z_text->set_int_val(marker_z);
		break;
	case 10:
		marker_incr += 1;
		marker_incr_text->set_int_val(marker_incr);
		break;
	case 11:
		marker_incr -= 1;
		marker_incr_text->set_int_val(marker_incr);
		break;
	}
}

void RenderManager::initCPW(){
	controlPanel_window = glutCreateSubWindow(main_window, 700, 0, 300, 750);
	glutDisplayFunc(renderSceneCPW);
	glEnable(GL_DEPTH_TEST);
#ifdef USE_GLUI
	gluiCPW = GLUI_Master.create_glui_subwindow(controlPanel_window, GLUI_SUBWINDOW_TOP);
	GLUI_Master.set_glutKeyboardFunc(keyboardCPW);
	GLUI_Master.set_glutMouseFunc(mouseCPW);
	glutMotionFunc(motionCPW);
	glutPassiveMotionFunc(passive_motionCPW);

	GLUI_Panel* panel1 = new GLUI_Panel(gluiCPW, "", GLUI_PANEL_NONE);
	GLUI_EditText* isovalue_text = new GLUI_EditText(panel1, "isovalue:", &(HEADER.isoVal));
	isovalue_text->disable();
	// This is slightly dangerous. The addresses given to GLUI_EditText should
	// not be local variables, since edits from UI would be placed here.
	// In this case, it is okay, since the editors are disabled.
	// (Probably there is a non-editable widget that should be used here.)
	mouse_x_display = input_processor->getMouseX();
	mouse_y_display = input_processor->getMouseY();
	mouse_x_text = new GLUI_EditText(panel1, "mouse x:", &mouse_x_display);
	mouse_y_text = new GLUI_EditText(panel1, "mouse y:", &mouse_y_display);
	mouse_x_text->disable();
	mouse_y_text->disable();

	new GLUI_Checkbox( panel1, "Black background", &blackBG );
	new GLUI_Checkbox( panel1, "Show bounding cube", &showCube );
	new GLUI_Checkbox( panel1, "Show coordinate axes", &showAxes );

	GLUI_Panel* panel2 = new GLUI_Panel(gluiCPW, "", GLUI_PANEL_NONE);
	new GLUI_Button(panel2, "Load Camera Settings", 1, (GLUI_Update_CB)cameraButton);
	for (short i=0; i<10; i++) {
		char s[100];
		sprintf(s, "restore %d",i);
		camera_restore_buttons[i] = new GLUI_Button(panel2, s, 100+i, (GLUI_Update_CB)cameraButton);
	}
	new GLUI_Column(panel2);
	new GLUI_Button(panel2, "Save Camera Settings", 2, (GLUI_Update_CB)cameraButton);
	for (short i=0; i<10; i++) {
		char s[100];
		sprintf(s, "save %d",i);
		new GLUI_Button(panel2, s, 200+i, (GLUI_Update_CB)cameraButton);
	}
	GLUI_Panel* panel3 = new GLUI_Panel(gluiCPW, "", GLUI_PANEL_NONE);
	new GLUI_StaticText( panel3, "Marker:" );
	marker_x_text = new GLUI_EditText(panel3, "x", &marker_x);
	marker_y_text = new GLUI_EditText(panel3, "y", &marker_y);
	marker_z_text = new GLUI_EditText(panel3, "z", &marker_z);
	marker_incr_text = new GLUI_EditText(panel3, "incr", &marker_incr);
	new GLUI_Button(panel3, "+incr", 10, (GLUI_Update_CB)markerButton);
	new GLUI_Button(panel3, "-incr", 11, (GLUI_Update_CB)markerButton);
	new GLUI_Column(panel3);
	new GLUI_Button(panel3, "+x", 0, (GLUI_Update_CB)markerButton);
	new GLUI_Button(panel3, "-x", 1, (GLUI_Update_CB)markerButton);
	new GLUI_Button(panel3, "+y", 2, (GLUI_Update_CB)markerButton);
	new GLUI_Button(panel3, "-y", 3, (GLUI_Update_CB)markerButton);
	new GLUI_Button(panel3, "+z", 4, (GLUI_Update_CB)markerButton);
	new GLUI_Button(panel3, "-z", 5, (GLUI_Update_CB)markerButton);

	GLUI_Panel* panel9 = new GLUI_Panel(gluiCPW, "", GLUI_PANEL_NONE);
	new GLUI_Button(panel9, "quit", 0, (GLUI_Update_CB)exit);
	glClearColor(0.8, 0.8, 0.8, 0.0);
#else
	glutKeyboardFunc(keyboardCPW);
	glutMouseFunc(mouseCPW);
	glutMotionFunc(motionCPW);
	glutPassiveMotionFunc(passive_motionCPW);
	glClearColor(0.9, 0.3, 0.6, 0.0);
#endif

}

void RenderManager::setProjection(int window, int x, int y, int w, int h)
{
	glutSetWindow(window);
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

// reshape is called by GLUT when window is reshaped
void RenderManager::reshape(int w, int h){

	// determine dimensions of each sub-window, relative to the full window

	// object window
	int ow_x = 0;
	int ow_y = 0;
	int ow_w = 0.7*w;
	int ow_h = 0.93*h;
	// control panel
	int cp_x = ow_x + ow_w;
	int cp_y = ow_y;
	int cp_w = w - ow_w;
	int cp_h = h;
	// text window
	int tw_x = ow_x;
	int tw_y = ow_y + ow_h;
	int tw_w = ow_w;
	int tw_h = h - ow_h;

	// reposition and resize each sub-window.
	setProjection(object_window, ow_x, ow_y, ow_w, ow_h);
	setProjection(controlPanel_window, cp_x, cp_y, cp_w, cp_h);
	setProjection(text_window, tw_x, tw_y, tw_w, tw_h);

	// tell the camera about the new dimensions of the object window
	if (camera != NULL) camera->reshape(ow_w,ow_h);
}

// mouse is called by GLUT when a mouse button is pressed or released
void RenderManager::mouseOW(int button, int state, int x, int y) {
	if (input_processor != NULL) input_processor->mouse(button, state, x, y);
}
void RenderManager::mouseCPW(int button, int state, int x, int y) {
}


// motion is called by GLUT when mouse is moved within the window with a button pressed
void RenderManager::motionOW(int x, int y) {
	if (input_processor != NULL) input_processor->motion(x, y);
}
void RenderManager::motionCPW(int x, int y) {
}


// passive_motion is called by GLUT when mouse is moved within the window with no button pressed
void RenderManager::passive_motionOW(int x, int y) {
	if (input_processor != NULL) input_processor->passive_motion(x, y);
}
void RenderManager::passive_motionCPW(int x, int y) {
}


// keyboard is called by GLUT when an ASCII key is pressed
void RenderManager::keyboardOW(unsigned char key, int x, int y) {
	if (input_processor != NULL) input_processor->keyboard(key, x, y);
}
void RenderManager::keyboardCPW(unsigned char key, int x, int y) {
}


void RenderManager::initialize(int* pargc, char** argv) {

	glutInit(pargc, argv);
#ifdef USE_MULTISAMPLING
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
#else
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
#endif
	glutInitWindowSize(1000,750);
	glutInitWindowPosition(100, 50);
	main_window = glutCreateWindow("Volume Visualization using - Marina Doherty ( TL:MC July, 2013)");

	//Register callback routines for main window
	//glutDisplayFunc(RenderManager::displayFaces);
#ifdef USE_GLUI
	GLUI_Master.set_glutReshapeFunc(reshape);
	GLUI_Master.set_glutIdleFunc(idle);
#else
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
#endif

	// set up the sub-windows
	initOW();	// object window
	initCPW();	// control panel window
	text_window = new TextWindow();
	text_window->initTW(main_window, )
}

void RenderManager::renderBitmapString(float x, float y, float z, void *font, char *string)
{
  char *c;
  glColor3f(0.0, 0.0, 0.);
  glRasterPos3f(x,y,z);
  for (c=string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void RenderManager::renderString(float x, float y, char* string)
{
	glPushMatrix();		//save orientation of coordinate systems
	glLoadIdentity();
	renderBitmapString(x ,y, 0, GLUT_BITMAP_HELVETICA_18, string);
	glPopMatrix();		//restore orientation of coordinate systems
}
