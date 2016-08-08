/*
 * UICommon.h
 *
 *  Created on: Aug 6, 2016
 *      Author: mdoherty
 */
/*
 * Various configuration parameters to control the build of the UI module.
 * Global data structure for information shared by different UI components.
 */
#ifndef UI_UICOMMON_H_
#define UI_UICOMMON_H_

// define these symbols to enable corresponding functionality
#define USE_GLUI				// use glui for controls and buttons
//#define USE_MULTISAMPLING		// multisampling doesn't work yet

struct UICommon {
	int blackBG;
	int showCube;
	int showAxes;
	bool drawOneFace;
	bool outputOnce;
	bool isDrawFaceNormal;
	int displayAmt;
	int displayIndex;
	bool bothFaces;
	bool enable_animation;
	bool reset_animation;

	int mouse_x;
	int mouse_y;

	int marker_x;
	int marker_y;
	int marker_z;
	bool marker_enabled;

	UICommon() {
		blackBG = 0;
		showCube = 1;
		showAxes = 1;
		drawOneFace = false;
		outputOnce = false;
		isDrawFaceNormal = false;
		displayAmt = 5;
		displayIndex = 0;
		bothFaces = true;
		enable_animation = true;
		reset_animation = false;

		mouse_x = 0;
		mouse_y = 0;

		marker_x = 0;
		marker_y = 0;
		marker_z = 0;
		marker_enabled = true;
	}
};

extern UICommon ui_common;

#endif /* UI_UICOMMON_H_ */
