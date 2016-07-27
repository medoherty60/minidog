#include "RenderManager.h"

int main_window=0, object_window=0, controlPanel_window=0, text_window=0;
int BothFaces=1;
int drawBadNormalCounter = 0, drawOneFace=0, outputOnce=0, isDrawFaceNormal=0, isBadNormal=0;
int displayAmt=5, displayIndex=0;
Cube* curr_cube;

Point translation, p1_prime, p2_prime, p3_prime;

int find_j=1;

void copyColor(float c1[4], float c2[4]){
	memcpy(c1, c2, 4*sizeof(float));
}
void setMaterial_Cube(){
	
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
void setMaterial_using_glColor(){
	
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
void setMaterial(){
	
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
}/*
void setMaterial(){
	
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
}*/

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

	if(outputOnce){
		cout<<"Display Normal: s="<<s<<endl;
		cout<<"ofsetX="<<osx<<","<<"offsetY="<<osy<<","<<"offsetZ="<<osz<<endl;
		cout<<"Point 1 (start point) =["<<p1.x()+osx<<", "<<p1.y()+osy<<", "<<p1.z()+osz<<"]"<<endl;
		cout<<"Point 1 (end point) =["<<(dn1.x()+osx)<<", "<<(dn1.y()+osy)<<", "<<(dn1.z()+osz)<<"]"<<endl;
		cout<<"Point 2 (start point) =["<<p2.x()+osx<<", "<<p1.y()+osy<<", "<<p1.z()+osz<<"]"<<endl;
		cout<<"Point 2 (end point) =["<<(dn2.x()+osx)<<", "<<(dn1.y()+osy)<<", "<<(dn1.z()+osz)<<"]"<<endl;
		cout<<"Point 3 (start point) =["<<p3.x()+osx<<", "<<p1.y()+osy<<", "<<p1.z()+osz<<"]"<<endl;
		cout<<"Point 3 (end point) =["<<(dn3.x()+osx)<<", "<<(dn1.y()+osy)<<", "<<(dn1.z()+osz)<<"]"<<endl;
		outputOnce=0;
	}
	
}

void drawCube(float dx, float dy, float dz, float osx, float osy, float osz){
	
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
void displayFaces(){
	
	float osx = CONF.cf_offsetX, osy = CONF.cf_offsetY, osz = CONF.cf_offsetZ;
	float dx = CONF.cf_deltaX, dy = CONF.cf_deltaY, dz = CONF.cf_deltaZ;
	
	drawBadNormalCounter=0;
	for (int j=0; j<global_facesVector.size(); j++) {
	//for (int j=0; j<100; j++) {
		
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
		/*
		 outfile<<" rendering vertex1: ["<<p1.x()+offsetx<<","<<p1.y()+offsety<<","<<p1.z()+offsetz<<"]"<<endl;
		 outfile<<" rendering vertex2: ["<<p2.x()+offsetx<<","<<p2.y()+offsety<<","<<p2.z()+offsetz<<"]"<<endl;
		 outfile<<" rendering vertex3: ["<<p3.x()+offsetx<<","<<p3.y()+offsety<<","<<p3.z()+offsetz<<"]"<<endl;
		 outfile<<" ------------------ "<<endl;	*/
	}//end for
	 // drawCube(dx,dy,dz, osx, osy, osz);
}

void idle(void){
	glutSetWindow(controlPanel_window);
	glutPostRedisplay();
	glutSetWindow(text_window);
	glutPostRedisplay();
	glutSetWindow(object_window);
	glutPostRedisplay();
}

//Display func for main window
void display(){
	
	glutSetWindow(main_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
	glFlush();
}

//Display func for Object Window 1
void renderSceneow() {
	
	glutSetWindow(object_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	//glPushMatrix();		//save previous matrix - contains settings for perspective projection
	glLoadIdentity();		//reset matrix  // glTranslatef(0., 0., -4.);
	gluPerspective(40., width/height, 0.1, 60.);   //set clipping volume
	glTranslatef(0., 0., -3.);
	
	//start_animation(40);                   //update a z-slice every 40 frames
	displayFaces();
	glDisable(GL_LIGHTING);
		//displayNormal();
	renderWireFrame(1.,1.,1.,1.,1.);       //borderwidth,r,g,b,a
	displayLines();
	glEnable(GL_LIGHTING);
	
	//renderLightObjects();
	//setOrthogrpahicProjection();
	glutSwapBuffers();
	glFlush();
}
//Display func for Control Panel Window
void renderScenecpw(){
	
	glClearColor(0.5, 0.5, 0.5, 1.);
	glutSetWindow(controlPanel_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//render Control Panel
	char s0[1000], s1[1000];
	sprintf(s0,"isovalue=%0.2f", CONF.cf_isoVal);
	renderString(-0.5,0.0,s0);
		//sprintf(s,"Opacity=%0.3f", effective_opacity);
	sprintf(s1,"x,y = %i,%i", screen_x, screen_y);
	renderString(-0.5,-0.1,s1);
	glutSwapBuffers();
	glFlush();
}

//Display func for text Window
void renderScenetw(){
	
	glClearColor(0.8, 0.8, 0.6, 1.);
	glutSetWindow(text_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    bool animateSlice = false;
	//render text
    renderText(animateSlice);
	glutSwapBuffers();
	glFlush();
}
void renderWireFrame(float borderwidth, float r, float g, float b, float a){
	
	glColor4f(r, g, b, a);
	glutWireCube(borderwidth);
	
}
void displayLines(){
	
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
void init(){
	
	glEnable(GL_DEPTH_TEST);
		//glPolygonMode(GL_FRONT, GL_FILL);
    //glutKeyboardFunc(keyboard);
	//glutSpecialFunc(specialKey);
	 
	 glutMouseFunc(mouse);
	 glutMotionFunc(motion);
	 glutPassiveMotionFunc(passive_motion);
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	
	if(!BothFaces){
		//suppress backfaces
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	
	lm.setup_lightObject();
	lm.light0_distance(20.);
		//setMaterial();
	setMaterial_using_glColor();
}
