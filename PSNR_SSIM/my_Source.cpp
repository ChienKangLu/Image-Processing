#include <iostream>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <iomanip>
#include<cmath>
using namespace std;
using namespace cv;

int height;
int width;
Mat readImage(char*  image_local);//回傳物件不用傳回指標??
void show(char* title, Mat image);
Mat newdraw();
void showImgdetail(Mat image);
void RGBtoGreyLevel(Mat Rgbimage,Mat Greyimage);
void translation(Mat image,int move);
double MSE(Mat Greyimage, Mat grey_translation);
double PSNR(Mat Greyimage, Mat grey_translation);
double Mean(Mat image);
double Variance(Mat image);
double Standard_Deviation(Mat image);
double Covariance(Mat imageA, Mat imageB);
double SSIM(Mat imageA, Mat imageB);
void drawHist(Mat raw, Mat showhist,int showHistsize,int histheight,int margin);
int main(int argc, char** argv)
{
	/*	讀取圖片*/
	char* dir1 = "13256419_1750751421826050_2783467262528711038_n.jpg";
	char* dir2 = "M10509109.png";
	Mat RGB = readImage("C:\\Users\\leo\\Pictures\\13256419_1750751421826050_2783467262528711038_n.jpg" );//strcat("C:\\Users\\leo\\Pictures\\",dir1) 
	/*	顯試圖片*/
	show("RGB", RGB);
	/*	顯試資訊*/
	showImgdetail(RGB);
	/*	創建新的圖*/
	Mat Grey =newdraw();
	/*	彩色轉為灰階*/
	RGBtoGreyLevel(RGB, Grey);
	/*	顯試圖片*/
	show("Greylevel", Grey);
	/*儲存Grey*/
	//imwrite("C:\\Users\\leo\\Pictures\\Grey.jpg ", Grey);
	/*	創建新的圖*/
	//Mat grey_translation= Grey;//複製錯誤
	Mat grey_translation;
	Grey.copyTo(grey_translation);//複製
	/*右移1格*/
	translation(grey_translation,1);
	/*	顯試圖片*/
	show(" grey_translation", grey_translation);
	/*儲存grey_translation*/
	//imwrite("C:\\Users\\leo\\Pictures\\grey_translation.jpg ", grey_translation);
	/*計算PSNR(MSE)*/
	PSNR(Grey, grey_translation);
	/*計算SSIM*/
	cout << setw(16) << "[SSIM]:" <<endl;
	SSIM(Grey, grey_translation);
	/*繪製直方圖*/
	int showHistsize = 1000;
	int histheight = 300;
	int margin = 50;
	Mat showHist(histheight,showHistsize , CV_8UC3, Scalar(255,255,255));  //把直方圖秀在一個256*256大的影像上
	drawHist(Grey, showHist, showHistsize,histheight, margin);
	show(" showHist", showHist);
	

	waitKey(0);
	return 0;

}
Mat readImage(char*  image_local) {
	 image_local =image_local;
	Mat image;
	image = imread(image_local, CV_LOAD_IMAGE_COLOR);//以BGR格式讀入
	if (!image.data)
	{
		cout << "NoNoNo" << endl;
		//return -1;
	}
	return image;
}
void show(char* title,Mat image) {
	/*
	WINDOW_NORMAL和WINDOW_AUTOSIZE，當使用WINDOW_NORMAL時，可變更視窗的大小，當使用WINDOW_AUTOSIZE時，會產生剛好包含Mat大小的視窗，無法改變視窗大小。
	*/
	namedWindow(title, CV_WINDOW_AUTOSIZE);
	imshow(title, image);
}
Mat newdraw() {
	Mat image;
	image.create(height, width, CV_8U);//位元深度為8，無負號，通道數1
	return image;
}
void showImgdetail(Mat image) {
	height = image.rows;
	width = image.cols;
	cout << left << showpoint << fixed;
	cout << setw(16) << "圖片高度:" << setw(7) << height << endl;
	cout << endl;
	cout << setw(16) << "圖片寬度:" << setw(7) << width << endl;
	cout << endl;
	cout << setw(16) << "圖片channel:" << setw(7) << image.channels() << endl;
}
void RGBtoGreyLevel(Mat Rgbimage, Mat Greyimage) {
	int totalPixel = 0;//記算全部有幾個pixel
	for (int height = 0; height<Greyimage.rows; height++) {
		for (int width = 0; width<Greyimage.cols; width++) {//B G R
			Vec3b bgrPixel = Rgbimage.at<Vec3b>(height, width);//取出原圖
			bgrPixel[0] *= 0.114;//B
			bgrPixel[1] *= 0.587;//G
			bgrPixel[2] *= 0.299;//R
			int black = bgrPixel[0] + bgrPixel[1] + bgrPixel[2];

			Greyimage.at<uchar>(height, width) = black;
			//cout<<setw(10) << black ;//印出每一個灰階值
			totalPixel++;
		}
		//cout << endl;//印出每一個灰階值
	}
	cout << endl;
	cout << setw(16) << "totalPixel(height*width):" << setw(7) << totalPixel << endl;
	cout << endl;
}

void translation(Mat image, int move){//往右移move格，從第move開始
	
	for (int height = 0; height<image.rows; height++) {
		for (int width = image.cols-1; width>=0; width--) {//B G R//從後面開始搬
			//Vec3b now = image.at<Vec3b>(height, width);
			if (width < move) {
				//image.at<uchar>(height, width) = 0;//前move個補到前面，換言之，不更動原來的
			}
			else {
				image.at<uchar>(height, width) = image.at<uchar>(height, width - move);//(從最後)把前面的放到後面
			}
																												
		}
		//cout << endl;//印出每一個灰階值
	}
}
double MSE(Mat Greyimage, Mat grey_translation) {
	int error=0;
	for (int height = 0; height < Greyimage.rows; height++) {
		for (int width = 0; width < Greyimage.cols; width++) {//B G R
			int grey = static_cast<int>(Greyimage.at<uchar>(height, width));
			int grey_move = static_cast<int>(grey_translation.at<uchar>(height, width));
			//cout << setw(5) << grey_move - grey;
			error +=pow(grey_move - grey,2);
			//cout <<setw(5)<< static_cast<int>(grey_translation.at<uchar>(height, width)) ;
		}
	}
	return error/(height*width);
}
double PSNR(Mat Greyimage, Mat grey_translation) {
	double mse = MSE(Greyimage, grey_translation);
	cout << setw(16) << "MSE :" << setw(7) << mse << endl << endl;
	double psnr = 10 * log10(255 * 255 / mse);
	cout << setw(16) << "PSNR :" << setw(7) << psnr << endl << endl;

	return psnr;
}
double Mean(Mat image) {
	double sum = 0;
	for (int height = 0; height < image.rows; height++) {
		for (int width = 0; width < image.cols; width++) {//B G R
			int grey = static_cast<int>(image.at<uchar>(height, width));
			sum = sum + grey;
		}
	}
	return sum / (height*width);
} 
double Variance(Mat image) {
	double mean = Mean(image);
	double sum = 0;
	for (int height = 0; height < image.rows; height++) {
		for (int width = 0; width < image.cols; width++) {//B G R
			int grey = static_cast<int>(image.at<uchar>(height, width));
			sum += pow(grey - mean, 2); 
		}
	}
	return sum / (height*width);
}
double Standard_Deviation(Mat image) {
	return  pow(Variance(image), 0.5);
}
double Covariance(Mat imageA, Mat imageB) {
	double meanA = Mean(imageA);
	double meanB = Mean(imageB);
	double sum = 0;
	for (int height = 0; height < imageA.rows; height++) {
		for (int width = 0; width < imageA.cols; width++) {//B G R
			int greyA = static_cast<int>(imageA.at<uchar>(height, width));
			int greyB = static_cast<int>(imageB.at<uchar>(height, width));
			sum += (greyA - meanA)*(greyB - meanB);
		}
	}
	return sum/( (height*width)-1);
}
double SSIM(Mat imageA, Mat imageB) {
	int c1=1;
	int c2=1;
	int c3=c2/2;
	double a=1;
	double b=1;
	double c=1;
	double meanA = Mean(imageA);
	double meanB = Mean(imageB);
	double VarianceA = Variance(imageA);
	double VarianceB = Variance(imageB);
	double Standard_DeviationA = pow(VarianceA, 0.5);
	double Standard_DeviationB = pow(VarianceB, 0.5);
	double  CovarianceAB = Covariance(imageA, imageB);
	cout << setw(16) << "meanA:"<< setw(7) << meanA <<endl << endl;
	cout << setw(16) << "meanB:" << setw(7) << meanB << endl << endl;
	cout << setw(16) << "VarianceA:" << setw(7) << VarianceA << endl << endl;
	cout << setw(16) << "VarianceB:" << setw(7) << VarianceB << endl << endl;
	cout << setw(32) << "Standard_DeviationA:" << setw(7) << Standard_DeviationA << endl << endl;
	cout << setw(32) << "Standard_DeviationB:" << setw(7) << Standard_DeviationB << endl << endl;
	cout << setw(16) << "CovarianceAB:" << setw(7) << CovarianceAB << endl << endl;
	double L = (2 * meanA*meanB + c1) / (pow(meanA, 2) + pow(meanB, 2) + c1);
	double C = (2 * Standard_DeviationA*Standard_DeviationB + c2) / (VarianceA + VarianceB + c2);
	double S= ( CovarianceAB + c3) / (Standard_DeviationA*Standard_DeviationB + c3);
	cout << setw(16) << "L:" << setw(7) << L << endl << endl;
	cout << setw(16) << "C:" << setw(7) << C << endl << endl;
	cout << setw(16) << "S:" << setw(7) << S << endl << endl;
	double ssim = pow(L, a)*pow(C, b)*pow(S, c);
	cout << setw(16) << "SSIM[0-1] * :" << setw(7) << ssim << endl << endl;
	return ssim;
}

void drawHist(Mat raw, Mat showhist,int showHistsize,int histheight,int margin) {
	int count[256];
	for (int i = 0;i < 256;i++) {
		count[i]=0;
	}
	for (int height = 0; height <raw.rows; height++) {//算出每一個灰階的數量
		for (int width = 0; width < raw.cols; width++) {//B G R
			int grey = static_cast<int>(raw.at<uchar>(height, width));
			count[grey]++;
		}
	}
	int Max=0;
	for (int i = 0;i < 256;i++) {//找出最大的數量
		if (Max < count[i]) {
			Max = count[i];
		}
	}
	double interval = double(showHistsize-2*margin)/256;//500/256
	int realsize = histheight - margin;
	float scale = (0.9 * realsize) / Max;//設定最多數量高度為視窗的0.9倍
	double fontsize = 0.3;
	for (int i = 0; i<256; i++) {
		int height = static_cast<int>( count[i]*scale);
		int width = i*interval;
		int pointpos = width + margin;
		line(showhist, Point(pointpos, realsize), Point(pointpos, realsize - height), Scalar(0));
		if(i%15==0)
		putText(showhist, to_string(i), Point(pointpos, realsize+margin/2), 0, fontsize, Scalar(0, 0, 0), fontsize);
		
	}

	//cout << setw(16) << "Max :" << setw(7) << Max << endl << endl;
	int heightMax = static_cast<int>(Max * scale);
	/*平均值線*/
	double mean = Mean(raw);//250
	int width = mean*interval;
	int pointpos = width + margin;
	/*string str = "mean";
	str += mean;*/
	line(showhist, Point(pointpos, realsize), Point(pointpos, realsize - heightMax), Scalar(0,0,255));//平均值
	putText(showhist, string("mean=")+=to_string(mean), Point(pointpos, margin/3), 0, fontsize, Scalar(0, 0, 255), fontsize);


	line(showhist, Point(margin, realsize), Point(showHistsize - margin, realsize), Scalar(0));//x 軸
	line(showhist, Point(margin, realsize), Point(margin, realsize - heightMax), Scalar(0));//y 軸

	//putText(showhist, to_string(Max), Point(margin/2, realsize - heightMax), 0, fontsize, Scalar(0, 0, 0), fontsize);
	//cout << "interval: " << interval<<endl;
	//cout << "showHistsize: " << showHistsize << endl;

}