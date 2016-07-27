/*
 *  Color_volume.h
 *  Created by Marina Doherty on 2/10/11.
 */
#ifndef Color_h
#define Color_h

#include <cmath>
#include <GL/glut.h>

class Color
{
public:

	float r, g, b, a;
	
	Color() {
		r=1.0; g=1.0; b=1.0, a=1.0;
	}
	Color(float _r, float _g,float _b, float _a=1.0) {
		r=_r; g=_g; b=_b; a=_a;
	}
	float red() { return r; }
	float green() { return g; }
	float blue() { return b; }
	
	static Color RainbowColorMap(GLfloat f)
	{
		Color c;
		const float dx = 0.8;
		if (f<0) {
			f=0;
		}else if(f>1) {
			f=1;
		}
		GLfloat	g = (6.0-2.0*dx)*f +dx;
		c.r = fmax(0.0,(3.0-fabs(g-4.0)-fabs(g-5.0))/2.0);
		c.g = fmax(0.0,(4.0-fabs(g-2.0)-fabs(g-4.0))/2.0);
		c.b = fmax(0.0,(3.0-fabs(g-1.0)-fabs(g-2.0))/2.0);
		return c;
	}
	static Color CTHeadColorMap(short v)
	{
		Color c2;
		float m_red=0, m_green=0, m_blue=0, m_alpha=0;
		GLfloat x_scale_factor = 255./41.;
		GLfloat y_scale_factor = 255./25.5;
		
		GLfloat x1 = 8.*x_scale_factor;   //x1=50
		GLfloat x2 = 12.*x_scale_factor;  //x2=74.6
		GLfloat x3 = 15.*x_scale_factor;  //x3=93.3
		GLfloat x4 = 16.*x_scale_factor;  //x4=99.5
		GLfloat x5 = 18.*x_scale_factor;  //x5=111.9
		GLfloat x6 = 33.5*x_scale_factor; //x6=208.4
		GLfloat x7 = 41.*x_scale_factor;  //x7=255
		
		if (v<x1){		//background - black
			
			m_red =  (7.*y_scale_factor)/x1;
			m_green= (5.*y_scale_factor)/x1;
			m_alpha = (2.*y_scale_factor)/x1;
			
			c2.r = v*m_red;
			c2.g = v*m_green;
			c2.b = 0;	
			c2.a = v*m_alpha;
		}
		else if (v<x2){	//skin - black
			
			m_red =  (10.*y_scale_factor)/x2;
			
			c2.r = v * m_red;
			c2.g = 0;
			c2.b = 1.* y_scale_factor;	
			c2.a= 18.* y_scale_factor;									
		}
		else if (v<x3){	//soft tissue 1 
			
			m_red =  (12.5*y_scale_factor)/x3;
			m_green= ((12.5+35.)*y_scale_factor)/x3;
			m_blue = ((7.+25.)*y_scale_factor)/x3;
			
			c2.r  = v*m_red;
			c2.g  = v*m_green+(-31.* y_scale_factor);
			c2.b  =(v*m_blue)+(-25 * y_scale_factor);	
			c2.a  = 2.*y_scale_factor;									
		}
		else if (v<x4){	//soft tissue 2 
			
			m_blue= ((8.+4.)*y_scale_factor)/x4;
			
			c2.r= 24.5 * y_scale_factor;
			c2.g = 23.5 * y_scale_factor;
			c2.b = (v*m_blue)+ (-4.*y_scale_factor);	
			c2.a = 22.*y_scale_factor;									
		}
		else if (v<x5){	//soft bone
			
			m_red =  ((15.-7.)*y_scale_factor)/x5;
			m_green= ((13.-4.)*y_scale_factor)/x5;
			m_blue = ((10.+4.)*y_scale_factor)/x5;
			
			c2.r = (v*m_red)+(7.*y_scale_factor);
			c2.g = (v*m_green)+(4.*y_scale_factor);
			c2.b =(v*m_blue)+(-4.*y_scale_factor);	
			c2.a = 3.*y_scale_factor;								
		}
		else if (v<x6){	//hard tissue
			
			m_red =  ((22.-7.)*y_scale_factor)/x6;
			m_green= ((21.-4.)*y_scale_factor)/x6;
			m_blue = ((20.+4.)*y_scale_factor)/x6;
			
			c2.r = (v*m_red)+(7.*y_scale_factor);
			c2.g = (v*m_green)+(4.*y_scale_factor);
			c2.b = (v*m_blue)+(-4.*y_scale_factor);	
			c2.a = 3.*y_scale_factor;								
		}
		else {//if (v<x7){	//hard bone
			
			m_red  = ((25.5-7)*y_scale_factor)/x7;
			m_green= ((25.5-4)*y_scale_factor)/x7;
			m_blue = ((25.5+4)*y_scale_factor)/x7;
			m_alpha = ((25.5-24)*y_scale_factor)/x7;
			
			c2.r = (v*m_red)+(7.*y_scale_factor);
			c2.g = (v*m_green)+(4.*y_scale_factor);
			c2.b = (v*m_blue)+(-4.*y_scale_factor);	
			c2.a = (v*m_alpha)+(24.*y_scale_factor);								
		}
		// clamp colors to 255
		/*
		 if (c2.r > 255) c2.r = 255;
		 if (c2.g > 255) c2.g = 255;
		 if (c2.b > 255) c2.b = 255;
		 if (c2.a > 255) c2.a = 255;
		 */
		c2.a *= 0.1;
		return c2;
	}
	
};
#endif



