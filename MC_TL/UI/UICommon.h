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


// structure to hold things that are shared by UI components
// or things that may be adjusted as globally before building
struct UICommon {
	// window logic
	int blackBG;
	int showCube;
	int showAxes;
	bool bothFaces;
	bool enable_animation;
	bool reset_animation;

	// renderer controls
	bool drawOneFace;
	bool outputOnce;
	bool isDrawFaceNormal;
	int displayAmt;
	int displayIndex;

	// input state
	int mouse_x;
	int mouse_y;

	// marker (3D cursor) state
	int marker_x;
	int marker_y;
	int marker_z;
	bool marker_enabled;

	UICommon() {
		// window logic
		blackBG = 0;
		showCube = 1;
		showAxes = 1;
		bothFaces = true;
		enable_animation = true;
		reset_animation = false;

		// renderer controls
		drawOneFace = false;
		outputOnce = false;
		isDrawFaceNormal = false;
		displayAmt = 5;
		displayIndex = 0;

		// input state
		mouse_x = 0;
		mouse_y = 0;

		// marker (3D cursor) state
		marker_x = 0;
		marker_y = 0;
		marker_z = 0;
		marker_enabled = true;
	}
};

extern UICommon ui_common;

#endif /* UI_UICOMMON_H_ */
