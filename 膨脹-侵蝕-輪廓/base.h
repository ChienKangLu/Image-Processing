#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <iomanip>
#include<cmath>
using namespace cv;
using namespace std;

#define MASKSIZE 3
extern const int dmask[];
extern int** mask;
typedef void (*filter)(Mat*, Mat*, int,int ,int**);
extern int height;
extern int width;


Mat* readImage(char*  image_local);
void show(char* title, Mat* image);
Mat* newdraw();
Mat* RGBtoGreyLevel(Mat* Rgbimage);
void initailmask(int** mask);
void D(Mat* original, Mat* newOne,int i,int j,int** mask);
void E(Mat* original, Mat* newOne, int i, int j,int** mask);
void  traversMat(Mat* original,Mat* newOne,filter filter);

void  CreateD(Mat* original, Mat* newOne);
void  CreateE(Mat* original, Mat* newOne);
Mat* subtract(Mat* D, Mat* E);