#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <string>


using namespace cv;
using namespace std;

float maximum(float array[]);
float *getHowManyTimesAColorIsRepeated(Mat image, float colorCount[]);
Mat getHistogram(Mat image, float howManyTimesAColorIsRepeated[]);
int getPosOfMaximum(float *array);
string getExposure(float howManyTimesAColorIsRepeated[], Mat image);
Mat getEqualizedImage(Mat image, float cdf[]);
float *getPMF(float array[], float howManyTimesAColorIsRepeated[], Mat image);
float *getCDF(float array[], float howManyTimesAColorIsRepeated[], float pmf[]);

int main(){
    //Open camera
    namedWindow("frame",WINDOW_AUTOSIZE);
    VideoCapture cap(0);

    if(!cap.isOpened()){
        cout << "No camera detected!!";
        return -1;
    }

    while(true){
        // Camera image
        Mat frame;
        cap >> frame;
        
        // Show camera image in gray scale
        Mat frameGray;
        cvtColor(frame, frameGray, COLOR_BGR2GRAY);
        imshow("Frame", frameGray);

        //Calculate color count
        float colorCount[256];
        float *howManyTimesAColorIsRepeated = getHowManyTimesAColorIsRepeated(frameGray,colorCount);

        //Display histogram
        namedWindow( "Display window1", WINDOW_AUTOSIZE  );
        imshow( "Display window1", getHistogram(frameGray, howManyTimesAColorIsRepeated) );
        
        //Calculate pmf and cdf
        float *pmf, *cdf;
        float pmfArray[256], cdfArray[256];
        pmf = getPMF(pmfArray, howManyTimesAColorIsRepeated, frameGray);
        cdf = getCDF(cdfArray, howManyTimesAColorIsRepeated, pmf);

        //Display equalizedimage
        namedWindow( "Display window2", WINDOW_AUTOSIZE  );
        imshow( "Display window2", getEqualizedImage(frameGray,cdf) );

        //Calculate exposure
        string exposure = getExposure(howManyTimesAColorIsRepeated, image);
        cout << exposure;
        waitKey(20);
        if(waitKey(30) >= 0) break;
        
    }


    

    return 0;
}

// Calculate histogram and return representing image
Mat getHistogram(Mat image, float howManyTimesAColorIsRepeated[]){
    float mostReaped = 1024/maximum(howManyTimesAColorIsRepeated);
    float histogram[256];
    for (int i = 0; i < 255; i++)
    {
        histogram[i] = howManyTimesAColorIsRepeated[i];
    }
    
    Mat histogramImage(1024,1024,CV_8U,Scalar(0));
    for (int i = 0; i < 255; i++)
    {
        int from = i*4;
        int y = (histogram[i] * mostReaped);
        rectangle(histogramImage,Point(from,histogramImage.rows),Point(from + 2,histogramImage.rows - y),Scalar(255,255,255), FILLED,LINE_8,0);
    }
    return histogramImage;
}

Mat getEqualizedImage(Mat image, float cdf[]){
    Mat equalizedImage = image.clone();
    for (int y = 0; y < equalizedImage.rows; y++)
    {
        for (int x = 0; x < equalizedImage.cols; x++)
        {
            equalizedImage.at<uchar>(y,x) = floor((255) * cdf[image.at<uchar>(y,x)]);
        }
        
    }
    return equalizedImage;
}

float *getPMF(float array[], float howManyTimesAColorIsRepeated[], Mat image){
    for (int i = 0; i < 255; i++)
    {
        array[i] = howManyTimesAColorIsRepeated[i] / (image.cols * image.rows);
        
    }
    float *pmf = array;
    return pmf;
}
float *getCDF(float array[],float howManyTimesAColorIsRepeated[], float pmf[]){
    array[0] = howManyTimesAColorIsRepeated[0];
    for (int i = 1; i < 255; i++)
    {
        array[i] = pmf[i - 1] + pmf[i];
        
    }
    float *cdf = array;
    return cdf;
}
string getExposure(float howManyTimesAColorIsRepeated[], Mat image){
    float *pmf, *cdf;
    float pmfArray[256], cdfArray[256];
    pmf = getPMF(pmfArray, howManyTimesAColorIsRepeated, image);
    cdf = getCDF(cdfArray, howManyTimesAColorIsRepeated, pmf);

    int maxPos = getPosOfMaximum(cdf);
    
    if(maxPos <= 85){
        return "Image is underexposure\n";
    }else if(maxPos <= 200){
        return "Image is well contrated\n";
    }else{
        return "Image is overexposure\n";
    }
    
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

//Search for the maximum value and return the position
int getPosOfMaximum(float *array){
    float *max = array;
    int xPos = 0;
    for (int i = 1; i < 255; i++)
    {
        if(*max < array[i]){
            max = &array[i];
            xPos = i;
        }
    }
    return xPos;
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