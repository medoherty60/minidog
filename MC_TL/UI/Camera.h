#ifndef CAMERA_H
#define CAMERA_H
/*
 *  Camera.h
 *  Created by Marina Doherty on 7/27/16.
 *  Replaces Rotation.h (3/11/11)
 */

struct SavedCameraState;

class Camera {
public:
	// parameter _window must be a valid GLUT or GLUI window handle
	Camera(int _window);

	// reshape is called when the size of the window controlled by this camera changes
	void reshape(int _width, int _height);

	// rotate image at its center, angle degrees around axis <X,Y,Z>
	void doRotationAngleAxis(double angle, double X, double Y, double Z);
	// rotate image at its center, in a manner consistent with 2D screen vector <dX,dY>
	void doRotationScreenVector(double dX, double dY);
	// move the image in the direction indicated by vector <tX,tY,tZ>
	void doTranslation(int tX, int tY, int tZ);

	// methods to control camera state save/restore
	bool restoreStateAvailable(int i);
	bool saveState(int i);
	bool restoreState(int i);
	void writeCameraStatesToFile();
	void readCameraStatesFromFile();

	// current width and height window controlled by this camera
	int getWindowWidth() { return width; }
	int getWindowHeight() { return height; }

private:
	// handle to GLUT or GLUI window controlled by this camera
	int camera_window;

	// current window dimensions
	int width, height;

	// ttX, ttY, ttZ: accumulated translation, used so that rotations
	//                are around object center rather than screen center
	int ttX, ttY, ttZ;

	// transfac: scaling factor for translations
	//           this must state constant for the lifetime of a camera,
	//           or the accumulated translations will be incorrect
	double transfac;

	// winRadius: half-width or half-height of the window
	//            used to determine angle of rotation
	double winRadius;

	// methods and attributes to create/restore camera state objects
	SavedCameraState* saveCamera();
	void restoreCamera(SavedCameraState* scs);
	SavedCameraState* saved_camera[10];
};

#endif

