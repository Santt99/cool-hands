#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>


using namespace cv;
using namespace std;

float maximum(float array[]);
float *getHowManyTimesAColorIsRepeated(Mat image, float colorCount[]);
float *getHistogram(Mat image);

int main(){
    namedWindow("frame",WINDOW_AUTOSIZE);
    VideoCapture cap(0);

    if(!cap.isOpened()){
        cout << "Cam no";
        return -1;
        }

    while(true){
        Mat frame;
        cap >> frame;
        getHistogram(frame);
        imshow("frame", frame);
        waitKey(20);
        if(waitKey(30) >= 0) break;
        
    }



    return 0;
}

float *getHowManyTimesAColorIsRepeated(Mat image,  float colorCount[]){
    for (int i = 0; i < 255; i++)
    {
        colorCount[i] = 0;
    }
    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            
            colorCount[image.at<uchar>(y,x)]++;
        }
        
    }

    return colorCount;
}
float *getHistogram(Mat image){
    float colorCount[256];
    float *howManyTimesAColorIsRepeated = getHowManyTimesAColorIsRepeated(image,colorCount);
    float mostReaped = maximum(howManyTimesAColorIsRepeated);
    // float histogram[256];
    cout << mostReaped << endl;
    // for (int i = 0; i < 255; i++)
    // {
    //     histogram[i] = *howManyTimesAColorIsRepeated/mostReaped;
    // }
    
    return NULL;
}

//Search fot the maximum value 
float maximum(float *array){
    float *max = array;
    for (int i = 1; i < 255; i++)
    {
        if(*max < array[i]){
            max = &array[i];
        }
    }
    return *max;
}