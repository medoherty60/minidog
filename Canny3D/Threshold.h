//
//  Threshold.h
//  3D_Canny
//
//  Created by Marina Doherty on 9/4/15.
//
//
#ifndef _D_Canny_Threshold_h
#define _D_Canny_Threshold_h

class Threshold{
    
public: Threshold(float _es_range[], float  _image_range[], float  _relax_image_range[]){
    
    memcpy(edge_strength_range, _es_range, 2*sizeof(float));
    memcpy(curr_edge_strength_range, _es_range, 2*sizeof(float));
    memcpy(image_range, _image_range, 2*sizeof(float));
    memcpy(relax_image_range, _relax_image_range, 2*sizeof(float));
    
}
//get an element in array
float get_edgeStrength(int i){
    return edge_strength_range[i];
}
float get_density(int i){
    return image_range[i];
}
float get_relax_density(int i){
    return relax_image_range[i];
}
float get_curr_edgeStrength(int i){
    return curr_edge_strength_range[i];
}
    
//get array
float *get_density_range(){
    return image_range;
}
float *get_relax_density_range(){
    return relax_image_range;
}
    
//update
void update_curr_edgeStrength(int i, float val){
    curr_edge_strength_range[i] = val;
}
    
//print
void print_edgeStrength(){
    cout<<"Edge Strength Range: [High,Low]=["<<edge_strength_range[0]<<","<<edge_strength_range[1]<<"]"<<endl;
}
void print_curr_edgeStrength(){
    cout<<"Curr. Edge Strength Range: [High,Low]=["<<curr_edge_strength_range[0]<<","<<curr_edge_strength_range[1]<<"]"<<endl;
}
void print_density(){
    cout<<"Density Range: [High,Low]=["<<image_range[0]<<","<<image_range[1]<<"]"<<endl;
}
void print_relax_density(){
    cout<<"Relaxed Density Range: [High,Low]=["<<relax_image_range[0]<<","<<relax_image_range[1]<<"]"<<endl;
}
    
private:
    float edge_strength_range[2];
    float image_range[2];
    float relax_image_range[2];
    float curr_edge_strength_range[2];
};
#endif
