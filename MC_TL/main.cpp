#include "FileManager.h"
#include "RenderManager.h"
/*
 Project Name: MC_Algorithm_TL
 Creation Date: Dec 29, 2012
 Created By: Marina S. Doherty
 Description: - read the file format, which contains (x,y,z) dimensions
                and the largest and smallest density values
			  - read the density data file and max data file into the Matrix3D m3D_dataPointsData
              - 
 isDensitySurface      isZeroCrossing
      F                    F              do nothing
      T                    F              red face
      T                    T              blue face
      F                    T              green dot
 */
static float getTime(){
	float t = (float)clock()/CLOCKS_PER_SEC;
	cout<< "curr_time: "<<t<<" sec"<<endl;
	return (t);
}
int main(int argc, char** argv) {
	
	//QApplication* app = new QApplication(argc, argv);
	Configurations& conf = CONF;
	FileManager fm;
	Matrix3D<float> dataPointsObj;
	Matrix3D<float> originalDensityVolObj;
	Matrix3D<float> binaryVolObj;
	MarchingCubes* mcs = new MarchingCubes;
	char* fileName = (char*)"dataFile/DFORMAT_MeristemSmooth.txt";
	float start_time=0.0;
	float end_time=0.0;
	
	//-- Step 1: Read Data --
	//Output: Matrix3D m3D_imageData holding density values
	if (fm.readDataFormat(fileName, conf)) {
		
		if(!fm.readVolumeData(dataPointsObj, conf, 1))
		 { cout<<"read data file 1 error"; return 0; } 
		
		if(conf.cf_numFiles==2 && !fm.readVolumeData(originalDensityVolObj, conf, 2))
		 { cout<<"read data file 2 error"; return 0; } 
		
		if(conf.cf_numFiles==3){
			
			bool rf2 = fm.readVolumeData(originalDensityVolObj, conf, 2);
			bool rf3 = fm.readVolumeData(binaryVolObj, conf, 3);
			
			if(!rf2) { cout<<"Reading Error: data file 2"; return 0; } 
			if(!rf3) { cout<<"Reading Error: data file 3"; return 0; } 
		}
				
	}else { cout<<"read format file error"; return 0; }	

	//-- Step 2: Create Active Cubes --
	cout<<"Make Cubes begins... "<<endl;
	start_time = getTime();
	mcs->set_resolution(conf.cf_xdim, conf.cf_ydim, conf.cf_zdim);
	mcs->init_all();
	mcs->set_ext_data(dataPointsObj.getPointer());
	
	if(conf.cf_numFiles==1)
		mcs->run( conf, conf.cf_isoVal );
	else if(conf.cf_numFiles==2){
		mcs->run( conf,  conf.cf_isoVal, &originalDensityVolObj, &binaryVolObj );		//Gradients on original density volume
		originalDensityVolObj.resize(1,81, 1);
		binaryVolObj.resize(1, 1, 1); 
	}
	else if(conf.cf_numFiles==3){
		mcs->run( conf,  conf.cf_isoVal, &originalDensityVolObj, &binaryVolObj );		//Gradients on original density volume
		originalDensityVolObj.resize(1, 1, 1); 
		binaryVolObj.resize(1, 1, 1); 
	}
	end_time = getTime();
	cout<<"## Time for running Marching Cubes: "<<end_time-start_time<<" seconds"<<endl;
	cout<<"num of triangles="<<mcs->ntrigs()<<", number of vertices="<<mcs->nverts()<<endl;
	
	mcs->clean_temps();
		//mcs->writePLY("test.ply");
	mcs->clean_all();
	
	//sliderWindow *sw = new sliderWindow(app); 
	
	//-- Step 7: Rendering --
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 50);
	main_window = glutCreateWindow("Volume Visualization using - Marina Doherty ( TL:MC July, 2013)");
	
	//Register callback routines for main window
	glutDisplayFunc(displayFaces);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	
	object_window = glutCreateSubWindow(main_window, 0, 0, 700, 700);
	glutDisplayFunc(renderSceneow);
	init();
	
	controlPanel_window = glutCreateSubWindow(main_window, 700, 0, 300, 750);
	glutDisplayFunc(renderScenecpw);
	
	text_window = glutCreateSubWindow(main_window, 0, 700, 700, 50);
	glutDisplayFunc(renderScenetw);

	glutMainLoop();
	//app->exec();
	return 0;
}



