#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

/*
 *  LightManager.h
 *  Marina Doherty on 6/19/12.
 *
 */

class LightManager{
public:
	LightManager(){
		
		lightparam[0] = 0.0;
		
		//Light 0
		light0_position[0] = 1; 
		light0_position[1] = 1; 
		light0_position[2] = 1; 
		
        /*
		light0_diffuse[0] = 1.0f; 
		light0_diffuse[1] = 1.0f; 
		light0_diffuse[2] = 1.0f; 
		light0_diffuse[3] = 1.0f;
        */
        
        light0_diffuse[0] = 0.8f;
        light0_diffuse[1] = 0.5f;
        light0_diffuse[2] = 0.3f;
        light0_diffuse[3] = 1.0f;
		
		light0_specular[0] = 0.3f;
		light0_specular[1] = 0.3f;
		light0_specular[2] = 0.3f;
		light0_specular[3] = 1.0f;
		
		light0_ambient[0]=0.2f;
		light0_ambient[1]=0.2f;
		light0_ambient[2]=0.2f;
		light0_ambient[3]=1.0f;
		
		//Light 1
        
		light1_diffuse[0] = 0.7f; 
		light1_diffuse[1] = 0.5f; 
		light1_diffuse[2] = 0.3f; 
		light1_diffuse[3] = 1.0f;
		
		light1_specular[0] = 0.0f;
		light1_specular[1] = 0.0f; 
		light1_specular[2] = 0.0f; 
		light1_specular[3] = 1.0f;
		
		white_light[0]=1.0f; 
		white_light[1]=1.0f; 
		white_light[2]=1.0f; 
		white_light[3]=1.0f;
		
		//Global Ambient
		lmodel_ambient[0]=0.5f;
		lmodel_ambient[1]=0.5f;
		lmodel_ambient[2]=0.5f;
		lmodel_ambient[3]=1.0f;
	}
	LightManager(float _x, float _y, float _z){
		light0_position[0] = _x; 
		light0_position[1] = _y; 
		light0_position[2] = _z; 
	}
	
	void setup_light0_Position(float _x, float _y, float _z){
		light0_position[0] = _x; 
		light0_position[1] = _y; 
		light0_position[2] = _z; 
	}
	void setup_light1_Position(float _x, float _y, float _z){
		light1_position[0] = _x; 
		light1_position[1] = _y; 
		light1_position[2] = _z; 
	}
	void light0_distance(float ld){
		light0_position[0] *= ld; 
		light0_position[1] *= ld; 
		light0_position[2] *= (ld/2.); 
	}
	void setup_lightObject();

private:
	float light0_position[3], light1_position[3];
	float white_light[4], light0_diffuse[4], light0_specular[4], light0_ambient[4];
	float light1_diffuse[4], light1_specular[4];
	float lmodel_ambient[4];
	float lightparam[1];
};

extern LightManager light_manager;
#endif

