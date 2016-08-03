/*
 *  main_canny.cpp
 *  Created by: Marina Doherty on 5/9/13.
 
	command line usage: canny width height depth base-filename [base-directory]
                        [width, height, depth]: volume dimension
	                    [base-filename]: the common part of input file (no extension)
	                    [base-directory (optional)]: If it is included it the program will switch 
                                                     to that directory before reading or writting 
                                                     any files. It should be set to the directory 
                                                     that contains the input files. Output directories 
                                                     will be created in that same location.
	Open Argument Window: [shift,command,<]
    Build: [command, B], 
    Run: [command, R]
 
    Inputs:
    Dimension3D(width, height, depth)	(argv[1],argv[2],argv[3])
    base_file_name						(argv[4])
    es_threshold_range(upper, lower)	(argv[5],argv[6])
    image_range(high, low)				(argv[7],argv[8])
    relax_image_range(high, low)		(argv[9],argv[10])
    number_of_experiments			 	(argv[11])
    step_size							(argv[12])
    base_directory                      (argv[13]) (option) may be necessary if input files are
                                                            not in same location as this program
 
    read directory:
    write director:
*/

// Turn off visual studio warnings about safety of C functions like fopen and fprintf.
#ifdef _MSC_VER // only for Visual Studio
#define _CRT_SECURE_NO_WARNINGS 
#endif

#include "ReadFile.h"
#include "Matrix3D.h"
#include "Experiments.h"
#include "unistd.h"
#include "stdlib.h"

bool changeDirectory(const string& dirname)
{
    #if defined(_WIN32)
        return _chdir(dirname.c_str()) == 0;
    #else
        return chdir(dirname.c_str()) == 0;
    #endif
}

/*
 * 5 = threshold_range high
 * 6 = threshold_range low
 * 7 = image_range high
 * 8 = image_range low
 * 9 = relax_image_range high
 * 10 = relax_image_range low
 * 11 = num_experiments
 * 12 = step_size
 */
// arguments 5-12 for meristem: 6 0 120 90 120 75 1 1
// arguments 5-12 for sphere: 6 0 1000 -1000 1000 -1000 1 1

int main(int argc, char** argv) {

	//===== Process command line arguments ======//
    cout <<"argc_size="<<argc<<endl;
    for(int i=1; i<argc; i++)
        cout <<i<<". "<<argv[i]<<endl;
    
	if (argc < 13) {
		cerr << "Usage b: width height depth base-filename upThreshold "<<
                "lowThreshold high_image low_image num_experiments step_size "
                "[base-directory]\n" << endl;
		return 0;
	}
    
    // base_directory (optional)
	if (argc > 13){
		string base_directory(argv[13]);
		if (!changeDirectory(base_directory))
		{
			cerr << "cannot start in base directory "
                 << base_directory << " -- aborting program" << endl;
			exit(1);
		}
	}
	//==== Experiment configuration parameters =====
    string base_fname(argv[4]);
    int dimension3D[3] = {atoi(argv[1]), atoi(argv[2]), atoi(argv[3])};
    float threshold_range[2] = {atoi(argv[5]), atoi(argv[6])};
    float image_range[2] = {atoi(argv[7]), atoi(argv[8])};
    float relax_image_range[2]= {atoi(argv[9]), atoi(argv[10])};
    int num_experiments = atoi(argv[11]);
    int step_size = atoi(argv[12]);
    
    //======== User-specific variables ======
    char write = 1;
    bool useSmoothData = true;
    bool noDensityTest = false;
    int  useRelax = 1;
    int isSyntheticData = 0;
	float img_small=0.0, img_large=0.0;
	Matrix3D<float> m3D_Data;
    FILTER_TYPE fsize;

	if(useSmoothData) fsize = FILTER9;
	else fsize = FILTER0;
    
    if(image_range[0] == 0.0 && image_range[1] == 0.0) noDensityTest=true;
    if(relax_image_range[0] == 0.0 && relax_image_range[1] == 0.0) useRelax=0;
    
    cout<<"(check rebuild = a)"<<endl;
    printf("test");

	
	//==== File Reading Types = useSmoothData or useOriginalData =====//
	
    stringstream fname1;
	if(useSmoothData){
		//string filtered_fname = exp->get_SmoothFile_Name();
		fname1 << base_fname + "_[" + filter_stamps[fsize] + "]"+".raw";
	}
    else fname1 << base_fname << ".raw";

	cout << "data file name is " << fname1.str() << endl;

    ReadFile *r = new ReadFile(fname1.str().c_str(), dimension3D);
    
    //==== loadData(num_bytes,swapBytes) ======/
    if(useSmoothData){
        r->loadData(m3D_Data, 4, 0);         //meristem, Lobster (no swap byte)
        //r->loadData(m3D_Data, 4, 1);       //CThead2
    }else{
        //[useOriginalData]
        //r->loadData(m3D_Data, 2, 1);      //meristem, tooth
        //r->loadData(m3D_Data, 3, 0);		//neuron
        //r->loadData(m3D_Data, 3, 1);		//neuron, meristem_1440p
        r->loadData(m3D_Data, 1, 0);		//lobster, MRI-head, Foot, CThead2
    }
    r->image_small(img_small);
    r->image_large(img_large);
    r->print();
    delete r;
    
	//========= Run Experiment on filter size ==========
	//ADJUST_UP_THRESHOLD  ADJUST_LOW_THRESHOLD
    
	Experiments *exp = new Experiments(base_fname, noDensityTest, isSyntheticData, 
									   fsize, ADJUST_LOW_THRESHOLD, write, img_small, img_large,
									   num_experiments, step_size, dimension3D, 
									   threshold_range, image_range, relax_image_range,
									   useRelax);
	exp->runExperiment(m3D_Data);
     
	return 0;
}
