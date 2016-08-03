#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include "ReadFile.h"
#include "WriteFile.h"
#include "Matrix3D.h"
#include "Threshold.h"
#include "EdgeStrength3D.h"
#include "Differentiate3D.h"
#include "GaussianFilter3D.h"
#include "AcceptanceTest.h"
#include <iomanip>
#include <time.h>
#include <sstream>
#include "Canny3Dabstract.h"
#include "Canny3D.h"
#include "Canny3Dv2.h"
#if defined(_WIN32)
#include <direct.h>
#else
#include <sys/stat.h>
#endif

const bool use_Canny3Dv2 = false;

/* 
 Parameters that vary throughout an experiment:
 filter size = size of the Gaussian smoothing filter
 upper threshold for 3rd derivative strength
 Parameters that vary depending on the input data:
 range of values for density filtering
 lower and upper threshold for 3rd derivative strength
 Information that should be recorded with an experiment:
 date/time
 base file name of input data
 real or synthetic data?
 threshold ranges
 density filter ranges (or indication that density filter is disabled)
 Input files:
 {base_filename}.raw (original volume data)
 {base_filename}_[0.333,3].raw (original data filtered by a size 3 Gaussian filter)
 {base_filename}_[0.667,5].raw (original data filtered by a size 5 Gaussian filter)
 {base_filename}_[1.000,7].raw (original data filtered by a size 7 Gaussian filter)
 Output files (dependent on experiment):
 experiment description (txt)
 Output files (dependent on filter size):
 3rd derivative magnitude (raw)
 2nd derivative magnitude (raw)
 cube statistics (csv)
 3rd derivative x,y,z (edge) strengths (csv)
 Output files (dependent on filter size and threshold):
 cubes (raw)
 FILE STRUCTURE:
 ./ 
 (input files)
 ./experiment[timestamp][filename]/
 [timestamp][filename]_description.txt (experiment description, 1 file)
 ./experiment[timestamp][filename]/fsize?/
 [timestamp][filename][filterstamp]_second_derivative.raw (2nd derivative magnitude, 1 file)
 [timestamp][filename][filterstamp]_third_derivative.raw (3rd derivative magnitude, 1 file)
 [timestamp][filename][filterstamp]_cubeStats.csv (cube statistics, 1 file)
 [timestamp][filename][filterstamp]_strengths.csv (edge strengths, 1 file)
 [timestamp][filename][filterstamp][thresholdstamp][densitystamp]_cubes.raw (cube markings, multiple files)		
 */
/* NOTE 1: Since the directories now contain a date and time stamp, data from prior 
 experiments will never be overwritten. Necessary directories are now created 
 by the program and do not need to be created before the program runs.
 NOTE 2: Some file names have been changed to use appropriate file extensions (.raw, .csv or .txt).
 To be consistent, the end of the method Canny3D::writeStrength_And_MagnitudeFiles() 
 should also be changed as follows:
 //WriteFile wm(_fname.c_str(), "magnitude");
 string magnitude_filename = _fname+"_third_derivative";
 WriteFile wm(magnitude_filename.c_str(), "raw");
 wm.writeFloat(m3D_magnitude, width, height, slice);
 */

#define NUM_FILTERS 6
#define TIME_TEST 1
string filter_labels[NUM_FILTERS] = {"fsize0", "fsize3", "fsize5", "fsize7", "fsize9", "fsize11"};
string filter_stamps[NUM_FILTERS] = {"0,0", "0.333,3", "0.667,5", "1.000,7", "1.333,9", "1.667,11"};
enum FILTER_TYPE { FILTER0, FILTER3, FILTER5, FILTER7, FILTER9, FILTER11 };
enum THRESHOLD_TYPE { ADJUST_UP_THRESHOLD, ADJUST_LOW_THRESHOLD };
enum IMAGE_RANGE_TYPE { ADJUST_UP_IMAGE,ADJUST_LOW_IMAGE };

class Experiments{

public: Experiments(string _fn, bool _dt, int _sd, FILTER_TYPE _f, THRESHOLD_TYPE _t,
					char& _w, float& _small, float& _large,
					const int& _numExp, const int& _stepSize, int _dim3D[],
					float _es_threshold_range[], float _image_range[], float _relax_image_range[], int _isRelax)
                    : threshold(_es_threshold_range, _image_range, _relax_image_range)
    {
	base_filename = _fn;
	no_density_test = _dt;
	isSyntheticData = _sd;
	filter_type = _f;
	threshold_type = _t;
	write = _w;
	img_small = _small;
	img_large = _large;
	
	numExperiments = _numExp;
	stepSize = _stepSize;
	memcpy(dimension3D, _dim3D, 3*sizeof(int));
	useRelax = _isRelax;

	//============ Define experiment name/labels, create experiment directory ==========
	string timeStamp = getTimeStamp();
	experiment_directory = makeExperimentLabel(timeStamp);
	if (!makeDirectory(experiment_directory))
		cerr << "failed to create directory " << experiment_directory << endl;
	
	EdgeStrengthTime=0.0;
	EdgeCubeDetectionTime=0.0;
}
string get_SmoothFile_Name(){
	
	return base_filename + "_[" + filter_stamps[filter_type] + "]"+".raw";
}
string get_Full_BaseFileName(){
	
	// Define and create a subdirectory for results base on current filter
	string subdir = experiment_directory + "/" +filter_labels[filter_type] + "/";
	if (!makeDirectory(subdir))
		cerr << "Failed to make subdirectory " << subdir << endl;
	
	// result files will have same base name as input files
	return subdir + base_filename + "_[" + filter_stamps[filter_type] + "]";
}
void runExperiment(Matrix3D<float>& m3D_Data){
	
	    string fname = get_Full_BaseFileName();

	    //data structure for storing values at grid points
	    Matrix3D<Vector> gradient;			//gradient (Lx,Ly,Lz)
		Matrix3D<float> Lvv;				//2nd derivatives in the direction of gradient
		Matrix3D<float> Lvvv;				//3rd derivatives in the direction of gradient

		//data structure for storing values at surface-crossing points
		Matrix3D<float> edge_strengthX;
		Matrix3D<float> edge_strengthY;
		Matrix3D<float> edge_strengthZ;
		Matrix3D<Vector> surfaceCrossing_gradient;

		int minEdgeStrength=0, maxEdgeStrength=0, strengthCount=0;
		int totalEdges=0, zc_count=0;
		int numPasses=0, edge_marks=0, cube_marks=0;
		int cubeCountFinalTotal=0, cubeCountFinal0=0, cubeCountFinal1=0, cubeCountFinal2=0, cubeCountFinal3=0;
	
		float start=0.0;
		//============ Compute differential values at grid points ============
		
		if(TIME_TEST) start = getTime();
		Differentiate3D *df = new Differentiate3D();
		df->computeDerivatives(&m3D_Data, &gradient, &Lvv, &Lvvv);
		//compute Dev time
		if(TIME_TEST) DevTime = computeTime(start, getTime());
	
	   if(TIME_TEST){ df->writeToDevFile(fname+"_second_derivative", &m3D_Data, &Lvv, "raw", write);
                      //this file is for Marching cubes
					  df->writeLvv_File(fname+"_Lvv", Lvv, "raw", write);
					  
		   zc_count = df->getZeroCrossingCount();
		   totalEdges = df->getGrandTotalEdges()-df->getUnusedEdges();
	   }
		//m3D_rData.clean();
		delete df;
	
		if(TIME_TEST) start = getTime();
		//============ Compute edge strengths at surface-crossing points ============
		EdgeStrength3D *es = new EdgeStrength3D();
		es->ComputeEdgeStrengths(edge_strengthX, edge_strengthY, edge_strengthZ, surfaceCrossing_gradient,
							     &gradient, &Lvv, &Lvvv, dimension3D);

		//compute edge strength time
		if(TIME_TEST) EdgeStrengthTime = computeTime(start, getTime());
	    if(TIME_TEST){
	       es->writeStrength_And_MagnitudeFiles(fname, "_strength", &edge_strengthX, &edge_strengthY, &edge_strengthZ);
		   maxEdgeStrength = es->getMaxStrength();
		   minEdgeStrength = es->getMinStrength();
		   strengthCount = es->getStrengthCount();			
		}
	
		//es->estimate_density_range(threshold_range[HIGH], image_range, &m3D_Data, &edge_strengthX, &edge_strengthY, &edge_strengthZ);
		delete es;

		Lvv.clean();
		Lvvv.clean();
	
		//============ Filtering and Thresholding ============
		float upperBound=0.0, lowerBound=0.0;
		float thresholdVal=0.0;
	    float curr_threshold_range[2];
		
		switch(threshold_type){
		
			case ADJUST_UP_THRESHOLD:
                upperBound= threshold.get_edgeStrength(HIGH)+1;
				lowerBound = threshold.get_edgeStrength(HIGH)-(stepSize*numExperiments);
                thresholdVal = threshold.get_edgeStrength(HIGH); //decrement in exps
				break;	
			
			case ADJUST_LOW_THRESHOLD:
				upperBound = threshold.get_edgeStrength(LOW)+(stepSize*numExperiments);
				lowerBound = threshold.get_edgeStrength(LOW)-1;
				thresholdVal = threshold.get_edgeStrength(LOW);				//increment in exps
				break;											
		}

		// iterate though range of ThresholdValues in descenting order with a step size
		while(thresholdVal>lowerBound && thresholdVal<upperBound){  
			
			switch(threshold_type){
                case ADJUST_UP_THRESHOLD: threshold.update_curr_edgeStrength(HIGH, thresholdVal); break;
                case ADJUST_LOW_THRESHOLD: threshold.update_curr_edgeStrength(LOW, thresholdVal); break;
            }
			cout<<endl<<"*************************************"<<endl;
			cout<<"start experiment:"<<endl;
            threshold.print_curr_edgeStrength();
			
			// generate a filename stamp for current strength threshold and density filter range
			stringstream threshold_stamp_ss;
            threshold_stamp_ss<<"["<<threshold.get_curr_edgeStrength(HIGH)<< ","<<threshold.get_curr_edgeStrength(LOW)<< "]_[";
			threshold_stamp_ss << threshold.get_density(HIGH)<< "," << threshold.get_density(LOW) << "]_[";
			threshold_stamp_ss << threshold.get_relax_density(HIGH)<< "," << threshold.get_relax_density(LOW) << "]";
			string threshold_stamp = threshold_stamp_ss.str();
			
			//============ Boundary Tracking & Expansion ============
			if(TIME_TEST) start = getTime();
			Canny3Dabstract *cy = NULL;
			if (use_Canny3Dv2)
				cy = new Canny3Dv2(no_density_test, dimension3D, &m3D_Data, &surfaceCrossing_gradient,
						&edge_strengthX, &edge_strengthY, &edge_strengthZ, &threshold);
			else
				cy = new Canny3D(no_density_test, dimension3D, &m3D_Data, &surfaceCrossing_gradient,
						&edge_strengthX, &edge_strengthY, &edge_strengthZ, &threshold);
            
			//*****
			//my previous experiments: use m3D_rData on first pass and m3D_Data on second pass
			//*****
			cy->Find_Initial_Seed_EdgeCubes();
			if(TIME_TEST) {
				EdgeCubeDetectionTime1 = computeTime(start, getTime());
				start = getTime();
			}
			cy->Hysteresis(useRelax);
			if(TIME_TEST) {
				EdgeCubeDetectionTime2 = computeTime(start, getTime());
				EdgeCubeDetectionTime = EdgeCubeDetectionTime1 + EdgeCubeDetectionTime2;
			}
			
			if(TIME_TEST){ cy->writeToCubesFile(fname+"_"+threshold_stamp+"_cubes", "raw",  write);
						   cy->writeToCubesStatisticFile(fname+"_cubeStats", "csv", thresholdVal, write);
						   numPasses = cy->get_num_passes();
							//print for surface expansion
							if(useRelax) cy->print_relax();
							else cy->print();

							//Candidate markers
						    edge_marks = cy->get_num_edgeMarks();
						    cube_marks = cy->get_num_cubeMarks();

						    //Final Cube marks
						    cubeCountFinal0 = cy->get_cubeMarks0_Final();
						    cubeCountFinal1 = cy->get_cubeMarks1_Final();
						    cubeCountFinal2 = cy->get_cubeMarks2_Final();
							if(useRelax) cubeCountFinal3 = cy->get_cubeMarks3_Final();
							cubeCountFinalTotal = cy->get_num_markedCubes_Final();
				
			}

			delete cy;
			//Acceptance test
			/*
			Point points[7] = { Point(79,82,4), Point(79,83,4), Point(100,39,4), 
								Point(72,41,4), Point(73,41,4), Point(95,26,4), Point(96,26,4)};
			
			int size= sizeof(points)/sizeof(Point);  
			
			if(f==FILTER5)
				for(int i=0; i<size; i++){
					AcceptanceTest t1(points[i], &edge_strengthX, &edge_strengthY, &edge_strengthZ, &m3D_Data, image_range,
									  cy->get_edgeMarks_Martrix(), cy->get_cubes_Martrix());
			}*/
			
			
			switch(threshold_type){
				case ADJUST_UP_THRESHOLD: thresholdVal-=stepSize; break;
				case ADJUST_LOW_THRESHOLD: thresholdVal+=stepSize;break;
			}
		} // end threshold loop.
	
		cout<<"scale level: "<<filter_labels[filter_type]<<endl;
        threshold.print_curr_edgeStrength();
        threshold.print_density();
        threshold.print_relax_density();
		cout<<"isSyntheticData:"<<isSyntheticData<<endl;
		cout<<"Density Filter: "<<no_density_test<<endl;
		cout<<"-------------------------"<<endl;
	
		//============ Write experiment information to text file in experiment directory ==========
		stringstream description_ss;
		stringstream str1, str2;
		description_ss << "Filename: " << base_filename << endl;
		description_ss << "Date: " << getFormattedDateTime() << endl;
	
		description_ss << "Data source: ";
		if (isSyntheticData) description_ss << "synthetic"; else description_ss << "CT Scanner";
		description_ss << endl<<endl;

		description_ss <<"Filter Size: " << filterType_to_filterSize(filter_type)<<endl;
	
		description_ss << "Edge Strength Threshold: ";

		switch(threshold_type){
			case ADJUST_UP_THRESHOLD:	description_ss << "Upper threshold range: "
                                        <<threshold.get_curr_edgeStrength(HIGH)
										<< " to " <<  threshold.get_edgeStrength(HIGH)
                                        << " by " << stepSize << endl;
								   	    description_ss <<"(Edge Strength) Lower threshold: "
                                        <<threshold.get_edgeStrength(LOW)<< endl; break;
				
			case ADJUST_LOW_THRESHOLD:	description_ss << "Upper threshold: "
                                        << threshold.get_edgeStrength(HIGH) << endl;
										description_ss << "Lower threshold range: "
                                        <<threshold.get_curr_edgeStrength(LOW)
										<< " to " << threshold.get_edgeStrength(LOW)
                                        << " by " << stepSize << endl; break;
		}
		description_ss << "Density range Threshold: ";
		if (no_density_test) description_ss << "disabled"<<endl; 
		else description_ss << "( "<<threshold.get_density(LOW) << " to " << threshold.get_density(HIGH) << ") "<<endl;
	
		description_ss << "Relaxed Density range Threshold: ";
	    if(!useRelax) description_ss << "disabled";
	    else description_ss << "( "<< threshold.get_relax_density(LOW) << " to " << threshold.get_relax_density(HIGH) << ") ";
	
		description_ss << endl<<endl;
		description_ss << "============================"<<endl;
	    description_ss << "Edge Strength Range (from File): ("<< minEdgeStrength<<", "<<maxEdgeStrength<<")"<<endl;
	    description_ss <<"Density Range (from File): ("<<img_small<<", "<<img_large<<") ";
	
		description_ss << endl<<endl;

		str1 << zc_count<<"("<<(float)(zc_count/(float)(totalEdges))*100<<"%)";
	    str2<<strengthCount<<"("<<(float)(strengthCount/(float)(totalEdges))*100<<"%)";

	    description_ss.imbue(locale(""));
	    description_ss << "============================"<<endl;
	    description_ss << "Edge Detection Result:"<<endl;
	    description_ss << setw(15)<<"Total Edges"<<setw(20)<<"ZC Edges"<<setw(30)<<"Edges w/Strengths>0"<<endl;
	    description_ss << setw(15)<< totalEdges<<setw(20)<<str1.str()<< setw(30)<<str2.str()<<endl<<endl;

	    description_ss << "Identified Strong & Weak Edges:"<<endl;
		description_ss << "Edge Marks: "<<edge_marks<<endl;
		description_ss << "Cube Marks: "<<cube_marks<<endl<<endl;

		description_ss << "Final Result:"<<endl;
		description_ss << "Final Cube: "<<cubeCountFinalTotal<<"("<<((float)cubeCountFinalTotal/(float)cube_marks)*100<<"%)"<<endl;

	    if(useRelax) description_ss << "Cubes: count[1,2,3,0] = ["<<cubeCountFinal1<<", "<<cubeCountFinal2<<", "<<cubeCountFinal3<<", "<<cubeCountFinal0<<"]=["<<cubeCountFinal0+cubeCountFinal1+cubeCountFinal2+cubeCountFinal3<<"]"<<endl;
		else description_ss << "Cubes: count[1,2,0] = ["<<cubeCountFinal1<<", "<<cubeCountFinal2<<", "<<cubeCountFinal0<<"]=["<<cubeCountFinal0+cubeCountFinal1+cubeCountFinal2<<"]"<<endl<<endl;
		description_ss << "percentage of marked cubes: "<<(float)(cube_marks/(float)(dimension3D[0]*dimension3D[1]*dimension3D[2]))*100<<"%"<<endl;
		description_ss << "percentage of (final) marked cubes: "<<(float)(cubeCountFinalTotal/(float)(dimension3D[0]*dimension3D[1]*dimension3D[2]))*100<<"%"<<endl;
		description_ss << "Total Cubes:" << (dimension3D[0]*dimension3D[1]*dimension3D[2]);

		description_ss << endl<<endl;
		description_ss << "============================"<<endl;
		description_ss << "Number of Passes: "<<numPasses<<endl;
	    description_ss << "Compute Dev time: "<<DevTime<<endl;
		description_ss << "Compute edge strength time: "<<EdgeStrengthTime<<endl;
		description_ss << "Edge Cube Detection time (seeds): "<<EdgeCubeDetectionTime1<<endl;
		description_ss << "Edge Cube Detection time (expansion): "<<EdgeCubeDetectionTime2<<endl;
		description_ss << "Edge Cube Detection time (total): "<<EdgeCubeDetectionTime<<endl;
		description_ss << "Total running time: "<<DevTime+EdgeStrengthTime+EdgeCubeDetectionTime<<endl;
		
		string description = description_ss.str();
		writeExperimentDescriptionFile(experiment_directory, experiment_directory, description);
}
int filterType_to_filterSize(FILTER_TYPE _t){
	
	switch(_t){
		case FILTER0: return 0;
		case FILTER3: return 3;
		case FILTER5: return 5;
		case FILTER7: return 7;
		case FILTER9: return 9;
		case FILTER11: return 11;
	}
}
	
float getTime(){
	float t = (float)clock()/CLOCKS_PER_SEC;
	cout<< "curr_time: "<<t<<" sec"<<endl;
	return (t);
}
float computeTime(float start, float end){
	float d = (end-start); 
	cout<< "time spent: "<<d<<" sec"<<endl;
	return d;
	
}
string getTimeStamp(){
	
	stringstream timestamp;
	time_t timer;
	struct tm* timeinfo;
	time(&timer);
	timeinfo = localtime(&timer);
	timestamp << (1900+timeinfo->tm_year)
	<< "-" << setw(2) << setfill('0') << timeinfo->tm_mon+1
	<< "-" << setw(2) << setfill('0') << timeinfo->tm_mday
	<< "-" << setw(2) << setfill('0') << timeinfo->tm_hour 
	<< "-" << setw(2) << setfill('0') << timeinfo->tm_min;
	return timestamp.str();
}
	
string getFormattedDateTime(){
	
	stringstream datetime;
	time_t timer;
	struct tm* timeinfo;
	time(&timer);
	timeinfo = localtime(&timer);
	datetime  << setw(2) << setfill('0') << timeinfo->tm_mday
	<< "/" << setw(2) << setfill('0') << timeinfo->tm_mon+1
	<< "/" << (1900+timeinfo->tm_year)
	<< " " << setw(2) << setfill('0') << timeinfo->tm_hour 
	<< ":" << setw(2) << setfill('0') << timeinfo->tm_min;
	return datetime.str();
}
	
string makeExperimentLabel(string& time_stamp)
{
	stringstream exprLabel;
	exprLabel << "experiment[" << time_stamp << "][" << base_filename << "]";
	return exprLabel.str();
}
	
bool makeDirectory(const string& dirname){
#if defined(_WIN32)
	return _mkdir(dirname.c_str()) == 0;
#else
	return mkdir(dirname.c_str(), 0755) == 0;
#endif
}
	
// create an ASCII .txt file to write basic experiment descriptions
bool writeExperimentDescriptionFile(const string& directory, 
									const string& filenamebase, 
									const string& contents){
	
	stringstream filename;
	filename << directory << "/" << filenamebase << "_description.txt";
	FILE* fp = fopen(filename.str().c_str(), "w");
	if (fp != NULL){
		
		fprintf(fp, "%s", contents.c_str());
		fclose(fp);
		return true;
	}
	return false;
}
	
private:
	string base_filename;
	string experiment_directory;
	bool no_density_test;
	int isSyntheticData;
	FILTER_TYPE filter_type;
	char threshold_type;
	char write;
	float img_small; 
	float img_large;
	
	int numExperiments;
	int stepSize;
	int dimension3D[3];
	int useRelax;
	
	float DevTime;
	float EdgeStrengthTime;
	float EdgeCubeDetectionTime, EdgeCubeDetectionTime1, EdgeCubeDetectionTime2;
    
    Threshold threshold;
	
};
#endif
