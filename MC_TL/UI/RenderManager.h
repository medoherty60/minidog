#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#define USE_GLUI
//#define USE_MULTISAMPLING    // multisampling doesn't work yet

// This include should be moved to RenderManager.cpp,
// which would require hiding the two GLUI window pointers.
#ifdef USE_GLUI
#include <GL/glui.h>
#endif

class Camera;
class InputProcessor;

class RenderManager {
public:
	// initialize sets everything up so that we're ready to enter the glut loop.
	static void initialize(int* pargc, char** argv);

	// parameters that control rendering in the object window
	static bool bothFaces;
	static bool drawOneFace;
	static bool outputOnce;
	static bool isDrawFaceNormal;
	static int displayAmt;
	static int displayIndex;
	static int blackBG;
	static int showCube;
	static int showAxes;

	// animation controls
	static clock_t last_frame_time;
	static float elapsed_frame_time;
	static bool first_animation_frame;
	static bool enable_animation;
	static int animation_face_code;

	static void resetAnimation() { animation_face_code = 0; }
private:
	// handles to GL windows
	static int main_window, object_window, controlPanel_window, text_window;
#ifdef USE_GLUI
	static GLUI *gluiCPW;
	static GLUI *gluiTW;
#endif

	// initialize the sub-windows
	static void initOW();
	static void initCPW();
	static void initTW();

	// reshape is called by GLUT when nothing is happening
	static void idle();

	// reshape is called by GLUT when window is reshaped
	static void reshape(int w, int h);

	// draw each of the sub-windows
	static void renderSceneOW();
	static void renderSceneCPW();
	static void renderSceneTW();
	static void updateAnimation();

	// mouse is called by GLUT when a mouse button is pressed or released
	static void mouseOW(int button, int state, int x, int y);
	static void mouseCPW(int button, int state, int x, int y);
	static void mouseTW(int button, int state, int x, int y);
	// motion is called by GLUT when mouse is moved within the window with a button pressed
	static void motionOW(int x, int y);
	static void motionCPW(int x, int y);
	static void motionTW(int x, int y);
	// passive_motion is called by GLUT when mouse is moved within the window with no button pressed
	static void passive_motionOW(int x, int y);
	static void passive_motionCPW(int x, int y);
	static void passive_motionTW(int x, int y);

	// keyboard is called by GLUT when an ASCII key is pressed
	static void keyboardOW(unsigned char key, int x, int y);
	static void keyboardCPW(unsigned char key, int x, int y);
	static void keyboardTW(unsigned char key, int x, int y);

	// private constructor to prevent instances of this class.
	RenderManager() { }

	// camera that controls the object window
	static Camera* camera;
	static void cameraButton(int id);

	static InputProcessor* input_processor;

	// functions for rendering in the object window.
	static void setProjection(int window, int x, int y, int w, int h);
	static void displayFaces();

	static void renderBitmapString(float x, float y, float z, void *font, char *string);
	static void renderString(float x, float y, char* string);
};
#endif
