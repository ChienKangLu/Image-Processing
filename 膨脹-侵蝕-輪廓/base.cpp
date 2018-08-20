#include "base.h"
const int dmask[] = { 1,2,3,3,1,2,1,0,3 };
int height;
int width;

Mat* readImage(char*  image_local) {
	Mat* image = new Mat;

	 *image = imread(image_local, CV_LOAD_IMAGE_COLOR);//以BGR格式讀入
	if (!(*image).data)
	{
		cout << "NoNoNo" << endl;
		//return -1;
	}
	return image;
}

void show(char* title, Mat* image) {
	/*
	WINDOW_NORMAL和WINDOW_AUTOSIZE，當使用WINDOW_NORMAL時，可變更視窗的大小，當使用WINDOW_AUTOSIZE時，會產生剛好包含Mat大小的視窗，無法改變視窗大小。
	*/
	namedWindow(title, CV_WINDOW_AUTOSIZE);
	imshow(title, (Mat)(*image));
}
Mat* newdraw() {
	Mat* image=new Mat;
	(*image).create(height, width, CV_8U);//位元深度為8，無負號，通道數1
	return image;
}
Mat* RGBtoGreyLevel(Mat* Rgbimage) {
	Mat* Greyimage = newdraw();
	//cout << Greyimage->rows;
	//cout << Greyimage->cols;
	int totalpixel = 0;//記算全部有幾個pixel
	for (int height = 0; height < (*Greyimage).rows; height++) {
		for (int width = 0; width < (*Greyimage).cols; width++) {//b g r
			Vec3b bgrpixel = (*Rgbimage).at<Vec3b>(height, width);//取出原圖
			bgrpixel[0] *= 0.114;//b
			bgrpixel[1] *= 0.587;//g
			bgrpixel[2] *= 0.299;//r
			int black = bgrpixel[0] + bgrpixel[1] + bgrpixel[2];
			(*Greyimage).at<uchar>(height, width) = black;
			//cout << setw(10) << black;//印出每一個灰階值
			totalpixel++;
		}
		//cout << endl;//印出每一個灰階值
		//show("ff", greyimage);
		//cout << endl;
		//cout << setw(16) << "totalpixel(height*width):" << setw(7) << totalpixel << endl;
		//cout << endl;
	}
	return Greyimage;
}

void initailmask(int** mask) {
	for (int i = 0;i < MASKSIZE;i++) {
		mask[i] = new int[MASKSIZE];
	}
	for (int i = 0;i < MASKSIZE;i++) {
		for (int j = 0;j < MASKSIZE;j++) {
			mask[i][j] = dmask[ i*MASKSIZE + j];
			cout << setw(16) << mask[i][j];
		}
		cout << endl;
	}
}
void D(Mat* original, Mat* newOne,int i,int j, int** mask) {//2,3
	//int grey = static_cast<int>(Greyimage.at<uchar>(height, width));
	int Max= 0;
	Max = static_cast<int>((*original).at<uchar>(i ,j));
	for (int height = 0; height < MASKSIZE; height++) {
		for (int width = 0; width < MASKSIZE; width++) {
			int now= static_cast<int>((*original).at<uchar>(i+height,j+width));
			now = now + mask[height][width];
			if (now > Max) {
				Max = now;
			}
		}
	}
	if (Max > 255) {
		Max = 255;
	}
	newOne->at<uchar>(i + MASKSIZE / 2, j + MASKSIZE / 2)=Max;

}
void E(Mat* original, Mat* newOne, int i, int j, int** mask) {
	int Min = 0;
	Min = static_cast<int>((*original).at<uchar>(i, j));
	for (int height = 0; height < MASKSIZE; height++) {
		for (int width = 0; width < MASKSIZE; width++) {
			int now = static_cast<int>((*original).at<uchar>(i + height, j + width));
			now = now - mask[height][width];
			if (now < Min) {
				Min = now;
			}
		}
	}
	if (Min < 0) {
		Min = 0;
	}
	newOne->at<uchar>(i + MASKSIZE / 2, j + MASKSIZE / 2) = Min;
}
/*
10
0 1 2 3 4 5 6 7 8 9
0 1 2 3 4 5 6 7 8 9
0 1 2 3 4 5 6 7 8 9
0 1 2 3 4 5 6 7 8 9
0 1 2 3 4 5 6 7 8 9
0 1 2 3 4 5 6 7 8 9
0 1 2 3 4 5 6 7 8 9
0 1 2 3 4 5 6 7 8 9
0 1 2 3 4 5 6 7 8 9
*/
void  traversMat(Mat* original, Mat* newOne, filter filter) {
	cout << "traversMat start"<<endl;
	for (int height = 0; height < (*original).rows-MASKSIZE+1; height++) {
		for (int width = 0; width < (*original).cols - MASKSIZE + 1; width++) {//B G R
			filter(original, newOne, height, width, mask);
		}
	}
	cout << "traversMat end" << endl;
}

Mat* subtract(Mat* D,Mat* E) {
	Mat* newOne = newdraw();
	for (int height = 0; height < (*newOne).rows; height++) {
		for (int width = 0; width < (*newOne).cols; width++) {//B G R
			int temp= static_cast<int>((*D).at<uchar>(height, width)) - static_cast<int>((*E).at<uchar>(height, width));
			if (temp < 0)
				temp = 0;
			newOne->at<uchar>(height, width) = temp;
		}
	}
	return newOne;
}
void  CreateD(Mat* original, Mat* newOne) {
	traversMat(original, newOne, D);
}
void  CreateE(Mat* original, Mat* newOne) {
	traversMat(original, newOne, E);
}