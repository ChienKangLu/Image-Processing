#include "base.h"
#define  ME_PICTURE
//#define BIG_ONE
//#define KOALA
int** mask = new int*[MASKSIZE]; 
int main(int argc, char** argv) {
#ifdef BIG_ONE
	Mat* RGB = readImage("C:\\Users\\leo\\Pictures\\13256419_1750751421826050_2783467262528711038_n.jpg");//13256419_1750751421826050_2783467262528711038_n.jpg		M10509109.png
#endif // BIG_ONE
#ifdef ME_PICTURE
	Mat* RGB = readImage("C:\\Users\\leo\\Pictures\\M10509109.png");//M10509109.png
#endif // ME_PICTURE
#ifdef KOALA
	Mat* RGB = readImage("C:\\Users\\leo\\Pictures\\Koala.jpg");//Koala.jpg
#endif // KOALA
	height = RGB->rows;//���w�Ϥ�����
	width = RGB->cols;//���w�Ϥ��e��
	Mat* Greyimage = RGBtoGreyLevel(RGB);//RGB �� �Ƕ�
	show("RGB", RGB);//���RGB
	show("Greyimage", Greyimage);//��ܦǶ�
	initailmask(mask);//��l��3*3 mask

	Mat* D = newdraw();
	CreateD(Greyimage, D);
	show("D", D);//���RGB

	Mat* E = newdraw();
	CreateE(Greyimage, E);
	show("E", E);//���RGB

	Mat* S = subtract(D, E);
	show("S", S);//���RGB

	imwrite("C:\\Users\\leo\\Pictures\\D.jpg ", *D);
	imwrite("C:\\Users\\leo\\Pictures\\E.jpg ", *E);
	imwrite("C:\\Users\\leo\\Pictures\\S.jpg ", *S);

	waitKey(0);
	system("pause");
	
}